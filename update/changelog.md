### 蓝奏云下载地址
- [点击转跳 密码:111](https://aloazny.lanzouo.com/b00je9nu1i)
- [1.3.5版本 密码: 111](https://aloazny.lanzouo.com/b00jeipeeb)
- [实时模式 密码:111](https://aloazny.lanzouo.com/b00jeku6cd)
- [Ebpf版本(测试) 密码:111](https://aloazny.lanzouv.com/b00jf0lz0h)
- [小飞机盘](https://share.feijipan.com/s/AN2lFUeN)

### 注意
- **模块更新一般无需重启**。
- [Github地址](https://github.com/Aloazny/AppOpt_Aloazny)
- [点击查看适配应用列表](https://aloazny.github.io/AppOpt_Aloazny/#%E9%80%82%E9%85%8D%E5%88%97%E8%A1%A8)
- **Flags文件**创建或者删除后，需要**重新刷模块压缩包入生效**，例如我下载了`线程优化二改211.zip`刷入后，想要实现(取消)增量更新，那么我在创建(删除)`/data/adb/modules/AppOpt_Aloazny/Flags/keep_custom_rule`后，**需要再次重新刷模块压缩包(`线程优化二改211.zip`)入生效**。

### 更新日志
> 28.4
- 修复一个`Asoulpackage.sh`脚本未迁移的bug。
- 紧急修复一个MT管理器(`26040453`)版本无法仔压缩包运行`uninstall.sh`脚本的bug，这个版本没法运行后台脚本，只能运行前台。
> 28.3
- 添加`keep_Asoulopt`和`zip_first`两个flag文件。
- 创建`/data/adb/modules/AppOpt_Aloazny/Flags/keep_Asoulopt`用于处理和[Asoulopt](https://github.com/nakixii/Magisk_AsoulOpt)冲突的包名，会删掉和**Asoulopt**冲突的规则，不影响**Asoulopt**运行。
- 在zip压缩包添加空文件`/Flags/zip_first`，**会优先使用模块压缩包的flag文件，覆盖模块目录的flag文件**。