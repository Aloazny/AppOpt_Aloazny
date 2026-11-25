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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include "uthash.h"

#define VERSION            "1.6.3"
#define BASE_CPUSET        "/dev/cpuset/AppOpt"
#define MAX_PKG_LEN        128
#define MAX_THREAD_LEN     32
#define INITIAL_PKG_CAPACITY 2560
#define INITIAL_RULE_CAPACITY 2560
#define INITIAL_WILDCARD_CAPACITY 128
#define DENT_BUF_SIZE (128 * 1024)

// 简单日志，用于检测错误规则提示
#define LOG_I(fmt, ...) do { write_log("[I] " fmt, ##__VA_ARGS__); } while (0)
#define LOG_W(fmt, ...) do { write_log("[W] " fmt, ##__VA_ARGS__); } while (0)
#define LOG_E(fmt, ...) do { write_log("[E] " fmt, ##__VA_ARGS__); } while (0)

typedef struct {
    char pkg[MAX_PKG_LEN];
    char thread[MAX_THREAD_LEN];
    char cpuset_dir[256];
    cpu_set_t cpus;
    bool is_wildcard;
    int priority;
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

typedef struct PackageEntry {
    char pkg[MAX_PKG_LEN];
    UT_hash_handle hh;
} PackageEntry;

typedef struct {
    atomic_int ref_count;
    AffinityRule* rules;
    size_t num_rules;
    AffinityRule** wildcard_rules;
    size_t num_wildcard_rules;
    time_t mtime;
    CpuTopology topo;
    char** pkgs;
    size_t num_pkgs;
    struct PackageEntry* pkg_table;
    char config_file[4096];
    char cpuset_base[256];
} AppConfig;

typedef struct {
    ProcessInfo* procs;
    size_t num_procs;
    size_t procs_cap;
    int last_proc_count;
    int last_proc_total;
    bool scan_all_proc;
    pid_t* tracked_pids;
    size_t num_tracked_pids;
    size_t tracked_pids_cap;
} ProcCache;

struct linux_dirent64 {
    ino64_t d_ino;
    off64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

static atomic_int config_updated = ATOMIC_VAR_INIT(0);
static int inotify_fd = -1;
static int inotify_wd = -1;
static int inotify_supported = 0;
static _Atomic(AppConfig*) current_config = NULL;

// 简单日志输出
static void write_log(const char *fmt, ...) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(stderr, "[%s] ", time_str);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fflush(stderr);
}

static char* strtrim(char* s) {
    char* end;
    while (isspace(*s)) s++;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace(*end)) end--;
    *(end + 1) = 0;
    return s;
}

static char* strtrim_line(char* s) {
    char* start = s;
    while (isspace((unsigned char)*start)) start++;
    if (!*start) return start;
    char* end = start + strlen(start) - 1;
    while (end > start && (isspace((unsigned char)*end) || *end == '#')) end--;
    end[1] = '\0';
    return start;
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

static bool parse_cpu_ranges(const char* spec, cpu_set_t* set, const cpu_set_t* present, char* invalid_range, size_t invalid_range_size) {
    if (!spec) return true;
    char* copy = strdup(spec);
    if (!copy) return false;
    char* s = copy;
    bool valid = true;

    while (*s) {
        char* end;
        unsigned long a = strtoul(s, &end,0);
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

        if (a > b) {
            if (invalid_range && invalid_range_size > 0) {
                snprintf(invalid_range, invalid_range_size, "%lu-%lu", a, b);
            }
            valid = false;
            s = (*end == ',') ? end + 1 : end;
            continue;
        }
        
        for (unsigned long i = a; i <= b && i < CPU_SETSIZE; i++) {
            if (present && !CPU_ISSET(i, present)) {
                if (invalid_range && invalid_range_size > 0) {
                    if (a == b) {
                        snprintf(invalid_range, invalid_range_size, "%lu", i);
                    } else {
                        snprintf(invalid_range, invalid_range_size, "%lu-%lu", a, b);
                    }
                }
                valid = false;
                break;
            }
            CPU_SET(i, set);
        }

        s = (*end == ',') ? end + 1 : end;
    }
    free(copy);
    return valid;
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
    parse_cpu_ranges(topo.present_str, &topo.present_cpus, NULL, NULL, 0);

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

static int calculate_rule_priority(const char* thread_pattern) {
    if (!thread_pattern || !*thread_pattern) {
        return 200;
    }
    int priority = 0;
    size_t len = strlen(thread_pattern);
    const char* p = thread_pattern;

    if (strchr(p, '*') == NULL && strchr(p, '?') == NULL && strchr(p, '[') == NULL) {
        return 1000 + len;
    }

    int non_wildcard_chars = 0;
    bool has_range = false;
    bool has_single_wildcard = false;
    bool has_star = false;

    while (*p) {
        if (*p == '[') has_range = true;
        else if (*p == '?') has_single_wildcard = true;
        else if (*p == '*') has_star = true;
        else non_wildcard_chars++;
        p++;
    }

    if (has_range) {
        priority = 500 + non_wildcard_chars;
    } else if (has_single_wildcard) {
        priority = 300 + non_wildcard_chars;
    } else if (has_star) {
        priority = 100 + non_wildcard_chars;
    }

    return priority;
}

static void cleanup_temp_resources(AffinityRule** rules, size_t num_rules, AffinityRule*** wildcard_rules, size_t num_wildcard_rules, PackageEntry** pkg_table) {
    if (rules && *rules) {
        free(*rules);
        *rules = NULL;
    }
    if (wildcard_rules && *wildcard_rules) {
        free(*wildcard_rules);
        *wildcard_rules = NULL;
    }
    if (pkg_table && *pkg_table) {
        PackageEntry* entry, *tmp;
        HASH_ITER(hh, *pkg_table, entry, tmp) {
            HASH_DEL(*pkg_table, entry);
            free(entry);
        }
        *pkg_table = NULL;
    }
}

static AppConfig* load_config(const char* config_file, const CpuTopology* topo, time_t* last_mtime) {
    struct stat st;
    if (stat(config_file, &st) != 0) {
        const char* initial_content = "# 规则编写与使用说明请参考 http://AppOpt.suto.top\n\n";
        if (write_file(AT_FDCWD, config_file, initial_content, O_WRONLY | O_CREAT | O_TRUNC)) {
            LOG_W("配置文件不存在，创建空的配置文件: %s\n", config_file);
        }
        return NULL;
    }

    if (last_mtime && *last_mtime == st.st_mtime && *last_mtime != -1) {
        return NULL;
    }

    int fd = open(config_file, O_RDONLY);
    if (fd < 0) {
        LOG_E("无法打开配置文件 %s: %s\n", config_file, strerror(errno));
        return NULL;
    }

    char* data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (data == MAP_FAILED) {
        LOG_E("无法映射配置文件 %s: %s\n", config_file, strerror(errno));
        return NULL;
    }

    AppConfig* cfg = calloc(1, sizeof(AppConfig));
    if (!cfg) {
        munmap(data, st.st_size);
        return NULL;
    }
    cfg->ref_count = 1;
    cfg->topo = *topo;
    build_str(cfg->config_file, sizeof(cfg->config_file), config_file, NULL);
    build_str(cfg->cpuset_base, sizeof(cfg->cpuset_base), BASE_CPUSET, NULL);

    AffinityRule* rules = malloc(INITIAL_RULE_CAPACITY * sizeof(AffinityRule));
    size_t rules_capacity = INITIAL_RULE_CAPACITY;
    size_t num_rules = 0;
    AffinityRule** wildcard_rules = malloc(INITIAL_WILDCARD_CAPACITY * sizeof(AffinityRule*));
    size_t wildcard_capacity = INITIAL_WILDCARD_CAPACITY;
    size_t num_wildcard_rules = 0;
    PackageEntry* pkg_table = NULL;

    if (!rules || !wildcard_rules) {
        munmap(data, st.st_size);
        free(rules);
        free(wildcard_rules);
        free(cfg);
        return NULL;
    }

    char line[256];
    char* line_ptr = data;
    char* end = data + st.st_size;
    size_t line_number = 0;

    while (line_ptr < end) {
        char* newline = memchr(line_ptr, '\n', end - line_ptr);
        if (!newline) newline = end;
        size_t line_len = newline - line_ptr;
        line_number++;

        if (line_len >= sizeof(line)) {
            LOG_W("第 %zu 行过长，跳过\n", line_number);
            line_ptr = newline + 1;
            continue;
        }

        strncpy(line, line_ptr, line_len);
        line[line_len] = '\0';
        line_ptr = newline + 1;

        char* p = strtrim_line(line);
        if (!*p || *p == '#') continue;

        char* eq = strchr(p, '=');
        if (!eq) {
            LOG_W("第 %zu 行无效规则：缺少 '=': %s\n", line_number, p);
            continue;
        }
        *eq++ = '\0';

        char* key = strtrim(p);
        char* value = strtrim(eq);

        char* comment = strchr(value, '#');
        if (comment) *comment = '\0';
        value = strtrim(value);

        if (!*key || !*value) {
            LOG_W("第 %zu 行无效规则：键或值为空: %s\n", line_number, p);
            continue;
        }

        char* br = strchr(key, '{');
        char* thread = "";
        if (br) {
            *br++ = '\0';
            char* eb = strchr(br, '}');
            if (!eb) {
                LOG_W("第 %zu 行无效规则：缺少闭合 '}': %s\n", line_number, p);
                continue;
            }
            *eb = '\0';
            thread = strtrim(br);
        }

        char* pkg = strtrim(key);
        if (strlen(pkg) >= MAX_PKG_LEN || strlen(thread) >= MAX_THREAD_LEN) {
            LOG_W("第 %zu 行无效规则：包名或线程名过长: %s\n", line_number, p);
            continue;
        }

        bool is_duplicate = false;
        for (size_t i = 0; i < num_rules; i++) {
            if (!strcmp(rules[i].pkg, pkg) && !strcmp(rules[i].thread, thread)) {
                LOG_W("第 %zu 行重复规则：%s{%s}=%s，请检查配置\n", line_number, pkg, thread, value);
                is_duplicate = true;
                break;
            }
        }
        if (is_duplicate) continue;

        if (num_rules >= rules_capacity) {
            rules_capacity *= 2;
            AffinityRule* temp_rules = realloc(rules, rules_capacity * sizeof(AffinityRule));
            if (!temp_rules) {
                munmap(data, st.st_size);
                cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
                free(cfg);
                return NULL;
            }
            rules = temp_rules;
        }

        AffinityRule* rule = &rules[num_rules];
        strncpy(rule->pkg, pkg, MAX_PKG_LEN - 1);
        rule->pkg[MAX_PKG_LEN - 1] = '\0';
        strncpy(rule->thread, thread, MAX_THREAD_LEN - 1);
        rule->thread[MAX_THREAD_LEN - 1] = '\0';
        CPU_ZERO(&rule->cpus);

        char invalid_range[64] = {0};
        if (!parse_cpu_ranges(value, &rule->cpus, &cfg->topo.present_cpus, invalid_range, sizeof(invalid_range))) {
            LOG_W("第 %zu 行无效 CPU 范围：%s 在规则 %s{%s}=%s，超出可用 CPU (%s)\n",
                  line_number, invalid_range, pkg, thread, value, cfg->topo.present_str);
            continue;
        }

        if (CPU_COUNT(&rule->cpus) == 0) {
            LOG_W("第 %zu 行无效 CPU 范围：%s 在规则 %s{%s}=%s，无有效 CPU\n",
                  line_number, value, pkg, thread, value);
            continue;
        }

        char* dir_name = cpu_set_to_str(&rule->cpus);
        if (!dir_name) {
            LOG_W("第 %zu 行无法将 CPU 集合转换为字符串\n", line_number);
            continue;
        }
        char cpuset_path[256];
        build_str(cpuset_path, sizeof(cpuset_path), cfg->cpuset_base, "/", dir_name, NULL);
        if (!create_cpuset_dir(cpuset_path, dir_name, cfg->topo.mems_str)) {
            LOG_W("第 %zu 行无法创建 cpuset 目录 %s\n", line_number, cpuset_path);
            free(dir_name);
            continue;
        }
        strncpy(rule->cpuset_dir, dir_name, sizeof(rule->cpuset_dir) - 1);
        rule->cpuset_dir[sizeof(rule->cpuset_dir) - 1] = '\0';
        free(dir_name);

        rule->is_wildcard = (strchr(pkg, '*') != NULL || strchr(pkg, '?') != NULL || strchr(pkg, '[') != NULL);
        rule->priority = calculate_rule_priority(thread[0] ? thread : pkg);

        num_rules++;

        if (!rule->is_wildcard) {
            PackageEntry* pkg_entry;
            HASH_FIND_STR(pkg_table, pkg, pkg_entry);
            if (!pkg_entry) {
                pkg_entry = malloc(sizeof(PackageEntry));
                if (!pkg_entry) {
                    munmap(data, st.st_size);
                    cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
                    free(cfg);
                    return NULL;
                }
                strncpy(pkg_entry->pkg, pkg, MAX_PKG_LEN - 1);
                pkg_entry->pkg[MAX_PKG_LEN - 1] = '\0';
                HASH_ADD_STR(pkg_table, pkg, pkg_entry);
            }
        } else {
            if (num_wildcard_rules >= wildcard_capacity) {
                wildcard_capacity *= 2;
                AffinityRule** temp_wildcard_rules = realloc(wildcard_rules, wildcard_capacity * sizeof(AffinityRule*));
                if (!temp_wildcard_rules) {
                    munmap(data, st.st_size);
                    cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
                    free(cfg);
                    return NULL;
                }
                wildcard_rules = temp_wildcard_rules;
            }
            wildcard_rules[num_wildcard_rules++] = rule;
        }
    }

    munmap(data, st.st_size);

    if (!num_rules) {
        LOG_W("从 %s 未加载有效规则\n", config_file);
        cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
        free(cfg);
        return NULL;
    }

    size_t num_pkgs = HASH_COUNT(pkg_table);
    char** pkgs = malloc(INITIAL_PKG_CAPACITY * sizeof(char*));
    size_t pkgs_capacity = INITIAL_PKG_CAPACITY;
    if (!pkgs) {
        cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
        free(cfg);
        return NULL;
    }
    PackageEntry* entry, *tmp;
    size_t i = 0;
    HASH_ITER(hh, pkg_table, entry, tmp) {
        if (i >= pkgs_capacity) {
            pkgs_capacity *= 2;
            char** temp_pkgs = realloc(pkgs, pkgs_capacity * sizeof(char*));
            if (!temp_pkgs) {
                for (size_t j = 0; j < i; j++) free(pkgs[j]);
                free(pkgs);
                cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
                free(cfg);
                return NULL;
            }
            pkgs = temp_pkgs;
        }
        pkgs[i] = strdup(entry->pkg);
        if (!pkgs[i]) {
            for (size_t j = 0; j < i; j++) free(pkgs[j]);
            free(pkgs);
            cleanup_temp_resources(&rules, num_rules, &wildcard_rules, num_wildcard_rules, &pkg_table);
            free(cfg);
            return NULL;
        }
        i++;
    }

    if (cfg->rules) free(cfg->rules);
    if (cfg->wildcard_rules) free(cfg->wildcard_rules);
    if (cfg->pkgs) {
        for (size_t j = 0; j < cfg->num_pkgs; j++) free(cfg->pkgs[j]);
        free(cfg->pkgs);
    }
    HASH_CLEAR(hh, cfg->pkg_table);
    HASH_ITER(hh, cfg->pkg_table, entry, tmp) {
        HASH_DEL(cfg->pkg_table, entry);
        free(entry);
    }

    cfg->rules = rules;
    cfg->num_rules = num_rules;
    cfg->wildcard_rules = wildcard_rules;
    cfg->num_wildcard_rules = num_wildcard_rules;
    cfg->pkgs = pkgs;
    cfg->num_pkgs = num_pkgs;
    cfg->pkg_table = pkg_table;
    cfg->mtime = st.st_mtime;

    if (last_mtime) *last_mtime = st.st_mtime;
    LOG_I("配置文件解析完成，共加载 %zu 条规则，%zu 个应用，%zu 条通配符规则\n", num_rules, num_pkgs, num_wildcard_rules);
    return cfg;
}

static int compare_rules(const void* a, const void* b) {
    AffinityRule* ra = *(AffinityRule**)a;
    AffinityRule* rb = *(AffinityRule**)b;
    return rb->priority - ra->priority;
}

static void proc_collect(const AppConfig* cfg, ProcCache* cache, size_t* count) {
    char* dent_buf = malloc(DENT_BUF_SIZE);
    int proc_fd = open("/proc", O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    int current_proc_total = 0;
    *count = 0;
    if (cache->procs == NULL) {
        cache->procs_cap = 2048;
        cache->procs = calloc(cache->procs_cap, sizeof(ProcessInfo));
        if (!cache->procs) {
            close(proc_fd);
            free(dent_buf);
            return;
        }
    }

    time_t current_time = time(NULL);
    while (1) {
        int nread = syscall(__NR_getdents64, proc_fd, (struct linux_dirent64*)dent_buf, DENT_BUF_SIZE);
        if (nread == -1) {
            break;
        }
        if (nread == 0) break;

        for (int pos = 0; pos < nread;) {
            struct linux_dirent64* ent = (struct linux_dirent64*)(dent_buf + pos);
            if (ent->d_type != DT_DIR || !isdigit(ent->d_name[0])) {
                pos += ent->d_reclen;
                continue;
            }

            char *end;
            long pid = strtol(ent->d_name, &end, 10);
            if (*end != '\0') {
                pos += ent->d_reclen;
                continue;
            }
            current_proc_total++;

            bool is_tracked = false;
            for (size_t i = 0; i < cache->num_tracked_pids; i++) {
                if (cache->tracked_pids[i] == pid) {
                    is_tracked = true;
                    break;
                }
            }

            if (!cache->scan_all_proc && !is_tracked) {
                struct stat statbuf;
                if (fstatat(proc_fd, ent->d_name, &statbuf, AT_SYMLINK_NOFOLLOW) != 0) {
                    pos += ent->d_reclen;
                    continue;
                }
                if (current_time - statbuf.st_mtime > 60) {
                    pos += ent->d_reclen;
                    continue;
                }
            }

            int pid_fd = openat(proc_fd, ent->d_name, O_RDONLY | O_DIRECTORY);
            if (pid_fd == -1) {
                pos += ent->d_reclen;
                continue;
            }

            char cmd[MAX_PKG_LEN] = {0};
            if (!read_file(pid_fd, "cmdline", cmd, sizeof(cmd))) {
                close(pid_fd);
                pos += ent->d_reclen;
                continue;
            }
            char* name = strrchr(cmd, '/');
            name = name ? name + 1 : cmd;

            if (*count >= cache->procs_cap) {
                size_t new_cap = cache->procs_cap * 2;
                ProcessInfo* new_procs = realloc(cache->procs, new_cap * sizeof(ProcessInfo));
                if (!new_procs) {
                    close(pid_fd);
                    pos += ent->d_reclen;
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
                    pos += ent->d_reclen;
                    continue;
                }
                proc->thread_rules = tmp;
                proc->thread_rules_cap = new_cap;
            }

            PackageEntry* pkg_entry;
            HASH_FIND_STR(cfg->pkg_table, name, pkg_entry);
            bool matched = (pkg_entry != NULL);
            if (!matched) {
                for (size_t i = 0; i < cfg->num_wildcard_rules; i++) {
                    const AffinityRule* rule = cfg->wildcard_rules[i];
                    if (fnmatch(rule->pkg, name, 0) == 0) {
                        if (proc->num_thread_rules >= proc->thread_rules_cap) {
                            size_t new_cap = proc->thread_rules_cap * 2;
                            AffinityRule** tmp = realloc(proc->thread_rules, new_cap * sizeof(AffinityRule*));
                            if (!tmp) break;
                            proc->thread_rules = tmp;
                            proc->thread_rules_cap = new_cap;
                        }
                        proc->thread_rules[proc->num_thread_rules++] = (AffinityRule*)rule;
                        matched = true;
                    }
                }
            }
            if (!matched) {
                close(pid_fd);
                pos += ent->d_reclen;
                continue;
            }

            if (pkg_entry) {
                for (size_t i = 0; i < cfg->num_rules; i++) {
                    const AffinityRule* rule = &cfg->rules[i];
                    if ((strcmp(rule->pkg, proc->pkg) == 0) ||
                        (rule->is_wildcard && fnmatch(rule->pkg, proc->pkg, 0) == 0)) {
                        if (proc->num_thread_rules >= proc->thread_rules_cap) {
                            size_t new_cap = proc->thread_rules_cap * 2;
                            AffinityRule** tmp = realloc(proc->thread_rules, new_cap * sizeof(AffinityRule*));
                            if (!tmp) break;
                            proc->thread_rules = tmp;
                            proc->thread_rules_cap = new_cap;
                        }
                        proc->thread_rules[proc->num_thread_rules++] = (AffinityRule*)rule;
                    }
                }
            } else {
                for (size_t i = 0; i < cfg->num_rules; i++) {
                    const AffinityRule* rule = &cfg->rules[i];
                    if (!rule->is_wildcard && strcmp(rule->pkg, proc->pkg) == 0) {
                        if (proc->num_thread_rules >= proc->thread_rules_cap) {
                            size_t new_cap = proc->thread_rules_cap * 2;
                            AffinityRule** tmp = realloc(proc->thread_rules, new_cap * sizeof(AffinityRule*));
                            if (!tmp) break;
                            proc->thread_rules = tmp;
                            proc->thread_rules_cap = new_cap;
                        }
                        proc->thread_rules[proc->num_thread_rules++] = (AffinityRule*)rule;
                    }
                }
            }

            if (proc->num_thread_rules > 1) {
                qsort(proc->thread_rules, proc->num_thread_rules, sizeof(AffinityRule*), compare_rules);
            }

            for (size_t i = 0; i < proc->num_thread_rules; i++) {
                const AffinityRule* rule = proc->thread_rules[i];
                if (!rule->thread[0]) {
                    CPU_OR(&proc->base_cpus, &proc->base_cpus, &rule->cpus);
                    build_str(proc->base_cpuset, sizeof(proc->base_cpuset), rule->cpuset_dir, NULL);
                    break;
                }
            }

            if (CPU_COUNT(&proc->base_cpus) == 0 && proc->num_thread_rules == 0) {
                close(pid_fd);
                pos += ent->d_reclen;
                continue;
            }

            int task_fd = openat(pid_fd, "task", O_RDONLY | O_DIRECTORY);
            close(pid_fd);
            if (task_fd == -1) {
                pos += ent->d_reclen;
                continue;
            }

            DIR* task_dir = fdopendir(task_fd);
            if (!task_dir) {
                close(task_fd);
                pos += ent->d_reclen;
                continue;
            }

            if (!proc->threads || proc->threads_cap < 512) {
                size_t new_cap = proc->threads_cap ? proc->threads_cap * 2 : 64;
                ThreadInfo* tmp = realloc(proc->threads, new_cap * sizeof(ThreadInfo));
                if (!tmp) {
                    closedir(task_dir);
                    pos += ent->d_reclen;
                    continue;
                }
                proc->threads = tmp;
                proc->threads_cap = new_cap;
            }

            struct dirent* tent;
            while ((tent = readdir(task_dir))) {
                char *end2;
                long tid = strtol(tent->d_name, &end2, 10);
                if (*end2 != '\0') continue;
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
                int highest_priority = -1;
                size_t best_rule_idx = 0;

                for (size_t i = 0; i < proc->num_thread_rules; i++) {
                    const AffinityRule* rule = proc->thread_rules[i];
                    const char* rule_thread = rule->thread;

                    if (strcmp(rule_thread, tname) == 0 ||
                        (rule_thread[0] == '\0' && (tname[0] == '\0' || strcmp(tname, " ") == 0))) {
                        CPU_OR(&ti->cpus, &ti->cpus, &rule->cpus);
                        matched = rule->cpuset_dir;
                        break;
                    } else if (rule->priority < 1000 && fnmatch(rule_thread, tname, 0) == 0) {
                        if (rule->priority > highest_priority) {
                            highest_priority = rule->priority;
                            best_rule_idx = i;
                        }
                    }
                }

                if (!matched && highest_priority >= 0) {
                    CPU_OR(&ti->cpus, &ti->cpus, &proc->thread_rules[best_rule_idx]->cpus);
                    matched = proc->thread_rules[best_rule_idx]->cpuset_dir;
                }

                if (matched) {
                    build_str(ti->cpuset_dir, sizeof(ti->cpuset_dir), matched, NULL);
                } else {
                    CPU_OR(&ti->cpus, &ti->cpus, &proc->base_cpus);
                    build_str(ti->cpuset_dir, sizeof(ti->cpuset_dir), proc->base_cpuset, NULL);
                }

                proc->num_threads++;
            }

            closedir(task_dir);
            (*count)++;
            pos += ent->d_reclen;
        }
    }
    free(dent_buf);
    close(proc_fd);
    if (current_proc_total > cache->last_proc_total || atomic_load(&config_updated)) {
        cache->scan_all_proc = true;
    } else {
        cache->scan_all_proc = false;
    }
    cache->last_proc_total = current_proc_total;
}

static void update_cache(ProcCache* cache, const AppConfig* cfg, int* affinity_counter) {
    bool need_reload = atomic_load(&config_updated);
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        need_reload = true;
    } else {
        int current_proc_count = info.procs;
        if (current_proc_count > cache->last_proc_count + 10) {
            need_reload = true;
        } else if (current_proc_count > cache->last_proc_count) {
            *affinity_counter = 0;
        }
        cache->last_proc_count = current_proc_count;
    }
    if (cache->procs != NULL && !need_reload) {
        for (size_t i = 0; i < cache->num_procs; i++) {
            if (kill(cache->procs[i].pid, 0) != 0) {
                need_reload = true;
                break;
            }
        }
    }

    if (need_reload || cache->scan_all_proc ) {
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
                cache->last_proc_count = 0;
            }
        }
    }
}

static void config_release(AppConfig* cfg) {
    if (!cfg) return;
    if (atomic_fetch_sub(&cfg->ref_count, 1) == 1) {
        if (cfg->rules) free(cfg->rules);
        if (cfg->wildcard_rules) free(cfg->wildcard_rules);
        if (cfg->pkgs) {
            for (size_t i = 0; i < cfg->num_pkgs; i++) free(cfg->pkgs[i]);
            free(cfg->pkgs);
        }
        PackageEntry* entry, *tmp;
        HASH_ITER(hh, cfg->pkg_table, entry, tmp) {
            HASH_DEL(cfg->pkg_table, entry);
            free(entry);
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
                        if (old_config) {
                            usleep(10000);
                            config_release(old_config);
                        }
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
                    if (old_config) {
                        usleep(10000);
                        config_release(old_config);
                    }
                }
                config_release(cfg);
            }
            sleep(interval);
        }
    }
    return NULL;
}

static void print_help(const char* prog_name) {
    printf("用法: %s [选项]\n", prog_name);
    printf("选项:\n");
    printf("  -c <配置文件>   指定配置文件 (默认: ./applist.conf)\n");
    printf("  -s <间隔>      设置检查间隔(秒) (必须>=1, 默认: 2)\n");
    printf("  -v             显示程序版本\n");
    printf("  -h             显示帮助信息\n");
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
            LOG_W("配置文件不存在，重建一个空的配置文件: %s\n", config_file);
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
            LOG_I("启用inotify监控配置文件变更\n");
        } else {
            close(inotify_fd);
            inotify_fd = -1;
            LOG_W("inotify初始化失败，使用轮询模式\n");
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
    int affinity_counter = 0;
    LOG_I("启动AppOpt服务 v%s [PID:%d]\n", VERSION, getpid());

    for (;;) {
    
        if (atomic_exchange(&config_updated, 0)) {
          cache.scan_all_proc = true;
          cache.last_proc_count = 0;
        }

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