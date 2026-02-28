## AppOpt(二改)

> ### 介绍
- 一个使用C语言编写，支持定义规则的安卓应用与游戏线程优化程序，SutoLiu大佬原创，Aloazny二改。
- 使用GPL3.0开源协议，自行去[原仓库](https://gitee.com/sutoliu/AppOpt)查看。
- 二改需公开源代码，并署名原作者[@coolapk SutoLiu 大佬](https://www.coolapk1s.com/u/SutoLiu)


> ### 二改部分参数以及Flags说明
- 支持指定`AppOpt`配置文件，使用命令参数`-c 文件路径`，不指定则和原模块`applist.conf`一致。
- `-s`指定扫描`/proc`的时间间隔。
- `uthash.h`管理配置文件数据。
- **进程名**支持通配符，**线程名**支持优先级排序，全词(精确)匹配优于通配符。
- 支持**机械性适配**`6+2` `3+4+1` `2+3+2+1` `4+4`核心配置，**`4+3+1` 的配置是默认配置(同核心配置无需修改)**，通过创建`/data/adb/modules/AppOpt_Aloazny/modtify_config`空文件启用机械性配置修改，**默认启用**。
- 支持**配置文件优先级**，创建`/data/adb/modules/AppOpt_Aloazny/update_config`空文件，就会使用模块的配置，而不是读取用户上一次自定义的配置，**默认使用模块配置**。
- 创建`/data/adb/modules/AppOpt_Aloazny/delete_game_config`空文件，**就会删除自带的游戏配置**。
- 创建`/data/adb/modules/AppOpt_Aloazny/dexota_modtify`空文件，用于修改`dex2oat`的prop值，具体请参考[Google官方文档 ART](https://source.android.com/docs/core/runtime/configure?hl=zh-cn#compiler_filters)。
- 创建`/data/adb/modules/AppOpt_Aloazny/{disable_program/enable_program}`空文件，用于控制是否禁用一些冲突(**如果存在**)的系统进程，具体请自行查看`program_ctrl.sh`脚本，默认不做任何修改。
- 创建`/data/adb/modules/AppOpt_Aloazny/keep_custom_rule`用于安装时，增量更新用户修改/添加的规则。

> ### 提示(Tips)
- **Flags文件**创建或者删除后，需要**重新刷模块压缩包入生效**。
- 例如我下载了`线程优化二改211.zip`刷入后。
- 想要实现(取消)增加更新，那么我在创建(删除)`/data/adb/modules/AppOpt_Aloazny/keep_custom_rule`后，**需要再次重新刷模块压缩包(`线程优化二改211.zip`)入生效**。
- **模块更新一般无需重启**。

> ### 模块适配列表
- [点击查看](./update/%E9%80%82%E9%85%8D%E5%BA%94%E7%94%A8.md)

> ### 模块历史更新日志
- [点击查看](./update/update.md)

> ### 蓝奏云下载地址
- [点击转跳 密码:111](https://aloazny.lanzouo.com/b00je9nu1i)
- [1.3.5版本 密码: 111](https://aloazny.lanzouo.com/b00jeipeeb)
- [实时模式 密码:111](https://aloazny.lanzouo.com/b00jeku6cd)
- [Ebpf版本(测试) 密码:111](https://aloazny.lanzouv.com/b00jf0lz0h)

> ### 线程配置理念和教程
- [教程和理念](https://www.coolapk.com/feed/63785290)
- [获取线程](https://www.coolapk.com/feed/63763679)

> ### 感谢
- [AppOpt (GPL3.0) ](https://gitee.com/sutoliu/AppOpt) 
- [Uthash  (BSD2.0) ](https://troydhanson.github.io/uthash/)
- [DeamOpt (GPL3.0) ](https://github.com/yeg278/DeamOpt)
- [markedjs (MIT) ](https://github.com/markedjs/marked)
- [Highlight.js (BSD3.0)](https://highlightjs.org/)
