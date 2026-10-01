#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define VERSION            "1.5.8"
#define BASE_CPUSET        "/dev/cpuset/AppOpt"
#define MAX_PKG_LEN        128
#define MAX_THREAD_LEN     32

typedef struct {
    char pkg[MAX_PKG_LEN];
    char thread[MAX_THREAD_LEN];
    char cpuset_dir[256];
    cpu_set_t cpus;
} AffinityRule;

typedef struct {
    pid_t tid;
    char name[MAX_THREAD_LEN];
    char cpuset_dir[256];
    cpu_set_t cpus;
} ThreadInfo;

typedef struct {
    pid_t pid;
    char pkg[MAX_PKG_LEN];
    char base_cpuset[128];
    cpu_set_t base_cpus;
    ThreadInfo* threads;
    size_t num_threads;
    size_t threads_cap;
    AffinityRule** thread_rules;
    size_t num_thread_rules;
    size_t thread_rules_cap;
} ProcessInfo;

typedef struct {
    cpu_set_t present_cpus;
    char present_str[128];
    char mems_str[32];
    bool cpuset_enabled;
    int base_cpuset_fd;
} CpuTopology;

typedef struct {
    atomic_int ref_count;
    AffinityRule* rules;
    size_t num_rules;
    time_t mtime;
    CpuTopology topo;
    char** pkgs;
    size_t num_pkgs;
    char config_file[4096];
} AppConfig;

typedef struct {
    ProcessInfo* procs;
    size_t num_procs;
    size_t procs_cap;
    int last_proc_count;
    bool scan_all_proc;
    pid_t* tracked_pids;
    size_t num_tracked_pids;
    size_t tracked_pids_cap;
} ProcCache;

static atomic_int config_updated = ATOMIC_VAR_INIT(0);
static int inotify_fd = -1;
static int inotify_wd = -1;
static int inotify_supported = 0;
static _Atomic(AppConfig*) current_config = NULL;

static char* strtrim(char* s) {
    char* end;
    while (isspace(*s)) s++;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace(*end)) end--;
    *(end + 1) = 0;
    return s;
}

static bool read_file(int dir_fd, const char* filename, char* buf, size_t buf_size) {
    int fd = openat(dir_fd, filename, O_RDONLY | O_CLOEXEC);
    if (fd == -1) return false;
    ssize_t n = read(fd, buf, buf_size - 1);
    close(fd);
    if (n <= 0) return false;
    buf[n] = '\0';
    return true;
}

static bool write_file(int dir_fd, const char* filename, const char* content, int flags) {
    int fd = openat(dir_fd, filename, flags | O_CLOEXEC, 0644);
    if (fd == -1) return false;
    ssize_t n = write(fd, content, strlen(content));
    close(fd);
    return (n == (ssize_t)strlen(content));
}

static int build_str(char *dest, size_t dest_size, ...) {
    va_list args;
    const char *segment;
    char *p = dest;
    size_t remaining = dest_size - 1;
    va_start(args, dest_size);
    while ((segment = va_arg(args, const char *)) != NULL) {
        size_t len = strlen(segment);
        if (len > remaining) {
            va_end(args);
            return 0;
        }
        memcpy(p, segment, len);
        p += len;
        remaining -= len;
    }
    *p = '\0';
    va_end(args);
    return 1;
}

static void parse_cpu_ranges(const char* spec, cpu_set_t* set, const cpu_set_t* present) {
    if (!spec) return;
    char* copy = strdup(spec);
    if (!copy) return;
    char* s = copy;

    while (*s) {
        char* end;
        unsigned long a = strtoul(s, &end, 10);
        if (end == s) {
            s++;
            continue;
        }

        unsigned long b = a;
        if (*end == '-') {
            s = end + 1;
            b = strtoul(s, &end, 10);
            if (end == s) b = a;
        }

        if (a > b) { unsigned long t = a; a = b; b = t; }
        for (unsigned long i = a; i <= b && i < CPU_SETSIZE; i++) {
            if (present && !CPU_ISSET(i, present)) continue;
            CPU_SET(i, set);
        }

        s = (*end == ',') ? end + 1 : end;
    }
    free(copy);
}

static char* cpu_set_to_str(const cpu_set_t *set) {
    size_t buf_size = 8 * CPU_SETSIZE;
    char *buf = malloc(buf_size);
    if (!buf) return NULL;
    int start = -1, end = -1;
    char *p = buf;
    size_t remain = buf_size - 1;
    bool first = true;

    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, set)) {
            if (start == -1) {
                start = end = i;
            } else if (i == end + 1) {
                end = i;
            } else {
                int needed;
                if (start == end) {
                    needed = snprintf(p, remain + 1, "%s%d", first ? "" : ",", start);
                } else {
                    needed = snprintf(p, remain + 1, "%s%d-%d", first ? "" : ",", start, end);
                }
                if (needed < 0 || (size_t)needed > remain) {
                    free(buf);
                    return NULL;
                }
                p += needed;
                remain -= needed;
                start = end = i;
                first = false;
            }
        }
    }
    if (start != -1) {
        int needed;
        if (start == end) {
            needed = snprintf(p, remain + 1, "%s%d", first ? "" : ",", start);
        } else {
            needed = snprintf(p, remain + 1, "%s%d-%d", first ? "" : ",", start, end);
        }
        if (needed < 0 || (size_t)needed > remain) {
            free(buf);
            return NULL;
        }
        p += needed;
    }
    *p = '\0';
    return buf;
}

static bool create_cpuset_dir(const char *path, const char *cpus, const char *mems) {
    if (mkdir(path, 0755) != 0 && errno != EEXIST) return false;
    if (chmod(path, 0755) != 0) return false;
    if (chown(path, 0, 0) != 0) return false;

    char cpus_path[256];
    build_str(cpus_path, sizeof(cpus_path), path, "/cpus", NULL);
    if (!write_file(AT_FDCWD, cpus_path, cpus, O_WRONLY | O_CREAT | O_TRUNC)) return false;

    char mems_path[256];
    build_str(mems_path, sizeof(mems_path), path, "/mems", NULL);
    return write_file(AT_FDCWD, mems_path, mems, O_WRONLY | O_CREAT | O_TRUNC);
}

static CpuTopology init_cpu_topo(void) {
    CpuTopology topo = { .cpuset_enabled = false, .base_cpuset_fd = -1 };
    CPU_ZERO(&topo.present_cpus);

    if (read_file(AT_FDCWD, "/sys/devices/system/cpu/present", topo.present_str, sizeof(topo.present_str))) {
        strtrim(topo.present_str);
    }
    parse_cpu_ranges(topo.present_str, &topo.present_cpus, NULL);

    if (access("/dev/cpuset", F_OK) != 0) return topo;

    if (create_cpuset_dir(BASE_CPUSET, topo.present_str, "0")) {
        topo.base_cpuset_fd = open(BASE_CPUSET, O_RDONLY | O_DIRECTORY);
        if (topo.base_cpuset_fd != -1) topo.cpuset_enabled = true;
    }

    char mems_path[256];
    build_str(mems_path, sizeof(mems_path), BASE_CPUSET, "/mems", NULL);
    if (!read_file(AT_FDCWD, mems_path, topo.mems_str, sizeof(topo.mems_str))) {
        build_str(topo.mems_str, sizeof(topo.mems_str), "0", NULL);
    } else {
        strtrim(topo.mems_str);
    }

    return topo;
}

static AppConfig* load_config(const char* config_file, const CpuTopology* topo, time_t* last_mtime) {
    struct stat st;
    if (stat(config_file, &st)) return NULL;
    AppConfig* cfg = calloc(1, sizeof(AppConfig));
    if (!cfg) return NULL;
    cfg->ref_count = 1;
    cfg->topo = *topo;
    build_str(cfg->config_file, sizeof(cfg->config_file), config_file, NULL);

    if (last_mtime && *last_mtime == st.st_mtime && *last_mtime != -1) {
        free(cfg);
        return NULL;
    }

    FILE* fp = fopen(config_file, "r");
    if (!fp) {
        free(cfg);
        return NULL;
    }

    AffinityRule* new_rules = NULL;
    char** new_pkgs = NULL;
    size_t rules_cnt = 0, pkgs_cnt = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char* p = strtrim(line);
        if (*p == '#' || !*p) continue;

        char* eq = strchr(p, '=');
        if (!eq) continue;
        *eq++ = 0;

        char* br = strchr(p, '{');
        char* thread = "";
        if (br) {
            *br++ = 0;
            char* eb = strchr(br, '}');
            if (!eb) continue;
            *eb = 0;
            thread = strtrim(br);
        }

        char* pkg = strtrim(p);
        char* cpus = strtrim(eq);
        if (strlen(pkg) >= MAX_PKG_LEN || strlen(thread) >= MAX_THREAD_LEN) continue;

        cpu_set_t set;
        CPU_ZERO(&set);
        parse_cpu_ranges(cpus, &set, &cfg->topo.present_cpus);
        if (CPU_COUNT(&set) == 0) continue;

        char* dir_name = cpu_set_to_str(&set);
        if (!dir_name) continue;

        char path[256];
        build_str(path, sizeof(path), BASE_CPUSET, "/", dir_name, NULL);
        if (!create_cpuset_dir(path, dir_name, cfg->topo.mems_str)) {
            free(dir_name);
            continue;
        }

        AffinityRule rule = {0};
        build_str(rule.pkg, sizeof(rule.pkg), pkg, NULL);
        build_str(rule.thread, sizeof(rule.thread), thread, NULL);
        build_str(rule.cpuset_dir, sizeof(rule.cpuset_dir), dir_name, NULL);
        rule.cpus = set;
        free(dir_name);

        AffinityRule* tmp_rules = realloc(new_rules, (rules_cnt + 1) * sizeof(AffinityRule));
        if (!tmp_rules) goto error;
        new_rules = tmp_rules;
        memcpy(&new_rules[rules_cnt], &rule, sizeof(AffinityRule));
        rules_cnt++;

        bool exists = false;
        if (new_pkgs != NULL) {
            for (size_t i = 0; i < pkgs_cnt; i++) {
                if (strcmp(new_pkgs[i], pkg) == 0) {
                    exists = true;
                    break;
                }
            }
        }
        if (!exists) {
            char** tmp_pkgs = realloc(new_pkgs, (pkgs_cnt + 1) * sizeof(char*));
            if (!tmp_pkgs) goto error;
            new_pkgs = tmp_pkgs;
            new_pkgs[pkgs_cnt] = strdup(pkg);
            if (!new_pkgs[pkgs_cnt]) goto error;
            pkgs_cnt++;
        }
    }

    if (cfg->rules) free(cfg->rules);
    if (cfg->pkgs) {
        for (size_t i = 0; i < cfg->num_pkgs; i++) free(cfg->pkgs[i]);
        free(cfg->pkgs);
    }

    if (last_mtime) *last_mtime = st.st_mtime;
    cfg->rules = new_rules;
    cfg->num_rules = rules_cnt;
    cfg->pkgs = new_pkgs;
    cfg->num_pkgs = pkgs_cnt;
    cfg->mtime = st.st_mtime;

    fclose(fp);
    printf("配置文件解析完成，共加载 %zu 条规则\n", rules_cnt);
    return cfg;

error:
    if (new_rules) free(new_rules);
    if (new_pkgs) {
        for (size_t i = 0; i < pkgs_cnt; i++) free(new_pkgs[i]);
        free(new_pkgs);
    }
    fclose(fp);
    free(cfg);
    return NULL;
}

static void proc_collect(const AppConfig* cfg, ProcCache* cache, size_t* count) {
    DIR* proc_dir = opendir("/proc");
    if (!proc_dir) return;
    int proc_fd = dirfd(proc_dir);
    *count = 0;

    if (cache->procs == NULL) {
        cache->procs_cap = 2048;
        cache->procs = calloc(cache->procs_cap, sizeof(ProcessInfo));
        if (!cache->procs) {
            closedir(proc_dir);
            return;
        }
    }

    struct dirent* ent;
    time_t current_time = time(NULL);
    while ((ent = readdir(proc_dir))) {
        char *end;
        long pid = strtol(ent->d_name, &end, 10);
        if (*end != '\0')  continue;

        if (!cache->scan_all_proc) {
            bool is_tracked = false;
            for (size_t i = 0; i < cache->num_tracked_pids; i++) {
                if (cache->tracked_pids[i] == pid) {
                    is_tracked = true;
                    break;
                }
            }
            if (!is_tracked) {
                struct stat statbuf;
                if (fstatat(proc_fd, ent->d_name, &statbuf, AT_SYMLINK_NOFOLLOW) != 0) continue;
                if (current_time - statbuf.st_mtime > 60) continue;
            }
        }

        int pid_fd = openat(proc_fd, ent->d_name, O_RDONLY | O_DIRECTORY);
        if (pid_fd == -1) continue;

        char cmd[MAX_PKG_LEN] = {0};
        if (!read_file(pid_fd, "cmdline", cmd, sizeof(cmd))) {
            close(pid_fd);
            continue;
        }
        char* name = strrchr(cmd, '/');
        name = name ? name + 1 : cmd;

        bool found = false;
        for (size_t j = 0; j < cfg->num_pkgs; j++) {
            if (strcmp(name, cfg->pkgs[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            close(pid_fd);
            continue;
        }

        if (*count >= cache->procs_cap) {
            size_t new_cap = cache->procs_cap * 2;
            ProcessInfo* new_procs = realloc(cache->procs, new_cap * sizeof(ProcessInfo));
            if (!new_procs) {
                close(pid_fd);
                continue;
            }
            memset(new_procs + cache->procs_cap, 0, (new_cap - cache->procs_cap) * sizeof(ProcessInfo));
            cache->procs = new_procs;
            cache->procs_cap = new_cap;
        }

        ProcessInfo* proc = &cache->procs[*count];

        proc->pid = pid;
        build_str(proc->pkg, sizeof(proc->pkg), name, NULL);
        CPU_ZERO(&proc->base_cpus);
        proc->base_cpuset[0] = '\0';
        proc->num_threads = 0;
        proc->num_thread_rules = 0;

        if (!proc->thread_rules || proc->thread_rules_cap < 8) {
            size_t new_cap = proc->thread_rules_cap ? proc->thread_rules_cap * 2 : 8;
            AffinityRule** tmp = realloc(proc->thread_rules, new_cap * sizeof(AffinityRule*));
            if (!tmp) {
                close(pid_fd);
                continue;
            }
            proc->thread_rules = tmp;
            proc->thread_rules_cap = new_cap;
        }

        for (size_t i = 0; i < cfg->num_rules; i++) {
            const AffinityRule* rule = &cfg->rules[i];
            if (strcmp(rule->pkg, proc->pkg) != 0) continue;

            if (rule->thread[0]) {
                if (proc->num_thread_rules >= proc->thread_rules_cap) {
                    size_t new_cap = proc->thread_rules_cap * 2;
                    AffinityRule** tmp = realloc(proc->thread_rules, new_cap * sizeof(AffinityRule*));
                    if (!tmp) break;
                    proc->thread_rules = tmp;
                    proc->thread_rules_cap = new_cap;
                }
                proc->thread_rules[proc->num_thread_rules++] = (AffinityRule*)rule;
            } else {
                CPU_OR(&proc->base_cpus, &proc->base_cpus, &rule->cpus);
                build_str(proc->base_cpuset, sizeof(proc->base_cpuset), rule->cpuset_dir, NULL);
            }
        }

        if (CPU_COUNT(&proc->base_cpus) == 0 && proc->num_thread_rules == 0) {
            close(pid_fd);
            continue;
        }

        int task_fd = openat(pid_fd, "task", O_RDONLY | O_DIRECTORY);
        close(pid_fd);
        if (task_fd == -1) {
            continue;
        }

        DIR* task_dir = fdopendir(task_fd);
        if (!task_dir) {
            close(task_fd);
            continue;
        }

        if (!proc->threads || proc->threads_cap < 512) {
            size_t new_cap = proc->threads_cap ? proc->threads_cap * 2 : 64;
            ThreadInfo* tmp = realloc(proc->threads, new_cap * sizeof(ThreadInfo));
            if (!tmp) {
                closedir(task_dir);
                continue;
            }
            proc->threads = tmp;
            proc->threads_cap = new_cap;
        }

        struct dirent* tent;
        while ((tent = readdir(task_dir))) {
            char *end2;
            long tid = strtol(tent->d_name, &end2, 10);
            if (*end2 != '\0')  continue;
            char tname[MAX_THREAD_LEN] = {0};

            int tid_fd = openat(task_fd, tent->d_name, O_RDONLY | O_DIRECTORY);
            if (tid_fd == -1) continue;

            if (!read_file(tid_fd, "comm", tname, sizeof(tname))) {
                close(tid_fd);
                continue;
            }
            close(tid_fd);

            strtrim(tname);

            if (proc->num_threads >= proc->threads_cap) {
                size_t new_cap = proc->threads_cap * 2;
                ThreadInfo* tmp = realloc(proc->threads, new_cap * sizeof(ThreadInfo));
                if (!tmp) continue;
                proc->threads = tmp;
                proc->threads_cap = new_cap;
            }

            ThreadInfo* ti = &proc->threads[proc->num_threads];
            ti->tid = tid;
            build_str(ti->name, sizeof(ti->name), tname, NULL);
            CPU_ZERO(&ti->cpus);
            const char* matched = NULL;

            for (size_t i = 0; i < proc->num_thread_rules; i++) {
                const AffinityRule* rule = proc->thread_rules[i];
                if (fnmatch(rule->thread, ti->name, FNM_NOESCAPE) == 0) {
                    CPU_OR(&ti->cpus, &ti->cpus, &rule->cpus);
                    matched = rule->cpuset_dir;
                }
            }

            if (matched) {
                build_str(ti->cpuset_dir, sizeof(ti->cpuset_dir), matched, NULL);
            } else {
                ti->cpus = proc->base_cpus;
                build_str(ti->cpuset_dir, sizeof(ti->cpuset_dir), proc->base_cpuset, NULL);
            }

            proc->num_threads++;
        }

        closedir(task_dir);
        (*count)++;
    }
    closedir(proc_dir);
}

static void update_cache(ProcCache* cache, const AppConfig* cfg, int* affinity_counter) {
    bool need_reload = false;
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        need_reload = true;
    } else {
        int current_proc_count = info.procs;
        if (current_proc_count > cache->last_proc_count + 11) {
            need_reload = true;
        } else if (current_proc_count > cache->last_proc_count) {
            *affinity_counter = 0;
        }
        cache->last_proc_count = current_proc_count;
    }
    if (cache->procs != NULL && !cache->scan_all_proc) {
        for (size_t i = 0; i < cache->num_procs; i++) {
            if (kill(cache->procs[i].pid, 0) != 0) {
                cache->scan_all_proc = true;
                break;
            }
        }
    }
    if (need_reload || cache->scan_all_proc) {
        size_t new_count = 0;
        proc_collect(cfg, cache, &new_count);

        if (new_count > cache->tracked_pids_cap) {
            size_t new_cap = cache->tracked_pids_cap ? cache->tracked_pids_cap * 2 : new_count;
            pid_t* new_pids = realloc(cache->tracked_pids, new_cap * sizeof(pid_t));
            if (new_pids) {
                cache->tracked_pids = new_pids;
                cache->tracked_pids_cap = new_cap;
            }
        }

        if (cache->tracked_pids) {
            cache->num_tracked_pids = 0;
            for (size_t i = 0; i < new_count; i++) {
                if (cache->num_tracked_pids < cache->tracked_pids_cap) {
                    cache->tracked_pids[cache->num_tracked_pids++] = cache->procs[i].pid;
                }
            }
        }
        
        cache->num_procs = new_count;
        *affinity_counter = 0;
        if (cache->scan_all_proc) cache->scan_all_proc = false;
    }
}

static void apply_affinity(ProcCache* cache, const CpuTopology* topo) {
    for (size_t i = 0; i < cache->num_procs; i++) {
        const ProcessInfo* proc = &cache->procs[i];
        for (size_t j = 0; j < proc->num_threads; j++) {
            const ThreadInfo* ti = &proc->threads[j];
            if (topo->cpuset_enabled && topo->base_cpuset_fd != -1) {
                char tid_str[32];
                snprintf(tid_str, sizeof(tid_str), "%d\n", ti->tid);
                if (CPU_COUNT(&ti->cpus) == 0) {
                    cpu_set_t curr;
                    if (sched_getaffinity(ti->tid, sizeof(curr), &curr) == -1) continue;
                    if (CPU_EQUAL(&topo->present_cpus, &curr)) continue;
                    write_file(topo->base_cpuset_fd, "tasks", tid_str, O_WRONLY | O_APPEND);
                } else {
                    cpu_set_t curr;
                    if (sched_getaffinity(ti->tid, sizeof(curr), &curr) == -1) continue;
                    if (CPU_EQUAL(&ti->cpus, &curr)) continue;
                    if (ti->cpuset_dir[0]) {
                        int fd = openat(topo->base_cpuset_fd, ti->cpuset_dir, O_RDONLY | O_DIRECTORY);
                        if (fd != -1) {
                            write_file(fd, "tasks", tid_str, O_WRONLY | O_APPEND);
                            close(fd);
                        }
                    }
                }
            }
            if (CPU_COUNT(&ti->cpus) == 0) continue;
            if (sched_setaffinity(ti->tid, sizeof(ti->cpus), &ti->cpus) == -1 && errno == ESRCH) {
                cache->scan_all_proc = true;
            }
        }
    }
}

static void config_release(AppConfig* cfg) {
    if (!cfg) return;
    if (atomic_fetch_sub(&cfg->ref_count, 1) == 1) {
        if (cfg->rules) free(cfg->rules);
        if (cfg->pkgs) {
            for (size_t i = 0; i < cfg->num_pkgs; i++) free(cfg->pkgs[i]);
            free(cfg->pkgs);
        }
        free(cfg);
    }
}

static AppConfig* get_config() {
    AppConfig* cfg = atomic_load_explicit(&current_config, memory_order_acquire);
    if (!cfg) return NULL;
    int old_ref = atomic_fetch_add_explicit(&cfg->ref_count, 1, memory_order_acq_rel);
    if (old_ref <= 0) {
        atomic_fetch_sub_explicit(&cfg->ref_count, 1, memory_order_release);
        return NULL;
    }
    if (atomic_load_explicit(&current_config, memory_order_acquire) != cfg) {
        atomic_fetch_sub_explicit(&cfg->ref_count, 1, memory_order_release);
        return NULL;
    }
    return cfg;
}

static void* config_loader_thread(void* arg) {
    int interval = *(int*)arg;
    free(arg);
    pthread_setname_np(pthread_self(), "ConfigLoader");

    time_t last_mtime = -1;
    while (1) {
        if (inotify_supported) {
            fd_set rfds;
            struct timeval tv;
            FD_ZERO(&rfds);
            FD_SET(inotify_fd, &rfds);
            tv.tv_sec = interval;
            tv.tv_usec = 0;

            int ret = select(inotify_fd + 1, &rfds, NULL, NULL, &tv);
            if (ret < 0) {
                if (errno == EINTR) continue;
                inotify_supported = 0;
                close(inotify_fd);
                inotify_fd = -1;
                continue;
            } else if (ret == 0) {
                continue;
            }

            char buf[4096] __attribute__((aligned(8)));
            ssize_t len = read(inotify_fd, buf, sizeof(buf));
            if (len <= 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    inotify_supported = 0;
                    close(inotify_fd);
                    inotify_fd = -1;
                }
                continue;
            }

            bool reload_needed = false;
            for (char* p = buf; p < buf + len;) {
                struct inotify_event* event = (struct inotify_event*)p;
                if (event->mask & (IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MOVE_SELF)) {
                    reload_needed = true;

                    if (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF)) {
                        sleep(interval);
                        AppConfig* cfg = get_config();
                        if (cfg) {
                            inotify_rm_watch(inotify_fd, inotify_wd);
                            inotify_wd = inotify_add_watch(inotify_fd, cfg->config_file, IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MOVE_SELF);
                            last_mtime = -1;
                            config_release(cfg);
                        }
                        if (inotify_wd < 0) {
                            inotify_supported = 0;
                            close(inotify_fd);
                            inotify_fd = -1;
                            break;
                        }
                    }
                }
                p += sizeof(struct inotify_event) + event->len;
            }

            if (reload_needed) {
                AppConfig* cfg = get_config();
                if (cfg) {
                    AppConfig* new_config = load_config(cfg->config_file, &cfg->topo, &last_mtime);
                    if (new_config) {
                        AppConfig* old_config = atomic_exchange(&current_config, new_config);
                        atomic_store(&config_updated, 1);
                        if (old_config) config_release(old_config);
                    }
                    config_release(cfg);
                }
            }
        } else {
            AppConfig* cfg = get_config();
            if (cfg) {
                AppConfig* new_config = load_config(cfg->config_file, &cfg->topo, &last_mtime);
                if (new_config) {
                    AppConfig* old_config = atomic_exchange(&current_config, new_config);
                    atomic_store(&config_updated, 1);
                    if (old_config) config_release(old_config);
                }
                config_release(cfg);
            }
            sleep(interval);
        }
    }
    return NULL;
}

static void print_help(const char* prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  -c <config_file>   指定配置文件 (默认: ./applist.conf)\n");
    printf("  -s <interval>      设置检查间隔(秒) (必须>=1, 默认: 2)\n");
    printf("  -v                 显示程序版本\n");
    printf("  -h                 显示帮助信息\n");
    printf("\n示例:\n");
    printf("  %s -c /data/applist.conf -s 3\n", prog_name);
}

int main(int argc, char **argv) {
    CpuTopology topo = init_cpu_topo();
    char config_file[4096] = "./applist.conf";
    int sleep_interval = 2;
    int opt;
    while ((opt = getopt(argc, argv, "c:s:hv")) != -1) {
        switch (opt) {
            case 'c':
                build_str(config_file, sizeof(config_file), optarg, NULL);
                printf("配置文件: %s\n", config_file);
                break;
            case 's':
            {
                char *endptr;
                long val = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0' || val < 1) {
                    fprintf(stderr, "无效的时间间隔: %s\n", optarg);
                    fprintf(stderr, "间隔必须是 >=1 的整数\n");
                    exit(EXIT_FAILURE);
                }
                sleep_interval = (int)val;
                printf("检查间隔: %d 秒\n", sleep_interval);
                break;
            }
            case 'v':
                printf("AppOpt 版本 %s\n", VERSION);
                exit(EXIT_SUCCESS);
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                print_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    struct stat st;
    if (stat(config_file, &st) != 0) {
        const char* initial_content = "# 规则编写与使用说明请参考 http://AppOpt.suto.top\n\n";
        if (write_file(AT_FDCWD, config_file, initial_content, O_WRONLY | O_CREAT | O_TRUNC)) {
            printf("配置文件不存在，重建一个空的配置文件: %s\n", config_file);
        }
    }

    AppConfig* initial_config = load_config(config_file, &topo, NULL);
    if (!initial_config) {
        fprintf(stderr, "初始配置加载失败\n");
        exit(EXIT_FAILURE);
    }
    atomic_store(&current_config, initial_config);
    atomic_store(&config_updated, 1);

    inotify_fd = inotify_init1(IN_CLOEXEC);
    if (inotify_fd >= 0) {
        int flags = fcntl(inotify_fd, F_GETFL);
        if (flags >= 0) fcntl(inotify_fd, F_SETFL, flags | O_NONBLOCK);
        inotify_wd = inotify_add_watch(inotify_fd, config_file, IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MOVE_SELF);
        if (inotify_wd >= 0) {
            inotify_supported = 1;
            printf("启用inotify监控配置文件变更\n");
        } else {
            close(inotify_fd);
            inotify_fd = -1;
            printf("inotify初始化失败，使用轮询模式\n");
        }
    }

    pthread_t loader_thread;
    int* interval_ptr = malloc(sizeof(int));
    if (!interval_ptr) {
        config_release(initial_config);
        if (inotify_supported) close(inotify_fd);
        exit(EXIT_FAILURE);
    }
    *interval_ptr = sleep_interval;

    if (pthread_create(&loader_thread, NULL, config_loader_thread, interval_ptr) != 0) {
        perror("配置加载器线程创建失败");
        free(interval_ptr);
        config_release(initial_config);
        if (inotify_supported) close(inotify_fd);
        exit(EXIT_FAILURE);
    }
    pthread_detach(loader_thread);

    ProcCache cache = {0};
    cache.scan_all_proc = true;
    int affinity_counter = 0;
    printf("启动AppOpt服务 v%s\n", VERSION);

    for (;;) {
        if (atomic_exchange(&config_updated, 0)) cache.scan_all_proc = true;

        AppConfig* cfg = get_config();
        if (cfg) {
            update_cache(&cache, cfg, &affinity_counter);
            affinity_counter--;
            if (affinity_counter < 1) {
                apply_affinity(&cache, &cfg->topo);
                affinity_counter = 5;
            }
            config_release(cfg);
        }
        sleep(sleep_interval);
    }
}
