## **Flags文件说明**

### **前言**
- 所有flag文件创建或删除后，都需要**重新刷一次模块压缩包**才能生效。
- 别问为什么，问就是设计如此。
- 只有卸载需要重启，其他操作不需要。

### ①`update_config`：
- 每次刷入都强制用模块自带的配置覆盖你改过的配置。
- 让你放弃思考直接用我的配置。
- **默认开启**（即模块里默认就有这个flag的效果，想关闭就删掉这个空文件）。

### ②`modtify_config`：
- 机械性适配不同核心配置(比如4+3+1/3+4+1/6+2/2+3+2+1……)。
- 自动改规则里的核心编号，需要搭配`update_config`使用，否则规则可能会乱。
- **默认开启**，想关就删它。

### ③`keep_custom_rule`：
- 增量更新用的。
- 每次刷模块时，保留你之前自己添加或修改过的规则
- 只把开发者自带的新规则加进去，你的规则优先级更高。
- 不懂就搜/问一下什么是**增量更新**。

### ④`delete_game_config`：
- 直接删掉模块自带的所有游戏配置，一个不留。

### ⑤`keep_Asoulopt`：
- 只删除和[A-soul模块](https://github.com/nakixii/Magisk_AsoulOpt)冲突的游戏包名配置
- 不影响Asoulopt运行，也不删其他游戏规则。

### ⑥`dexota_modtify`：
- 修改dex2oat的编译线程数和绑核(尽量用性能核心)
- 顺便调一下虚拟机内存参数(heapstartsize、heapminfree等)
- 具体看Google官方文档。

### ⑦`disable_program` / `enable_program`
- 控制是否禁用Joyose、oiface这类可能冲突的系统进程。
- 默认啥也不干，你创建哪个就执行哪个，具体看`program_ctrl.sh`脚本。

### ⑧`zip_first`：
- **只能在压缩包里创建** (放`Flags/zip_first`)。
- 刷模块时会优先用压缩包里的flag文件覆盖模块目录里的flag文件。
- 相当于强制重置你的flag设置。