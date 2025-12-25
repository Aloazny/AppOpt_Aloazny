### 蓝奏云下载地址
- [点击转跳 密码:111](https://aloazny.lanzouo.com/b00je9nu1i)
- [1.3.5版本 密码: 111](https://aloazny.lanzouo.com/b00jeipeeb)
- [实时模式 密码:111](https://aloazny.lanzouo.com/b00jeku6cd)

### 注意
- **模块更新一般无需重启**。
- [Github地址](https://github.com/Aloazny/AppOpt_Aloazny)
- [点击查看适配应用列表](https://aloazny.github.io/AppOpt_Aloazny/#%E9%80%82%E9%85%8D%E5%88%97%E8%A1%A8)
- **Flags文件**创建或者删除后，需要**重新刷模块压缩包入生效**，例如我下载了`线程优化二改211.zip`刷入后，想要实现(取消)增量更新，那么我在创建(删除)`/data/adb/modules/AppOpt_Aloazny/keep_custom_rule`后，**需要再次重新刷模块压缩包(`线程优化二改211.zip`)入生效**。

### 更新日志
> 25.8
- 添加诅咒之岛(`com.jurassic.world.the.cursed.isle.dinosaurs.carnivores.dino.hunter.dinos.online.trex.tyrannosaurus.simulator`)适配。
- 调整`inotify`部分代码，尝试解决部分移植包设备可能出现软重启的情况。
> 25.7
- 修复`dalvik.vm.heapstartsize`/`dalvik.vm.heapminfree`/`dalvik.vm.heapmaxfree`调整失效，不知为何`magisk30200`以后，有的`[[]]`没法用，必须带上`if`。
> 25.6
- 调整`Joiplay`/`Maldives`/`Aopaop`模拟器相关线程。
- 调整`cpu_control.sh`，函数`set_target_cpuaffinity`可以单独提取出来自己写脚本用。