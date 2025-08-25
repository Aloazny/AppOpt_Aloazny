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
> 23.1
- 添加Animius (`com.lanlinju.animius`)，Koodo Reader (`com.koodoreader.expo`)适配。
- 优化AppOpt对错误规则的检测。
> 23.0
- **本次更新建议重启！**
- cpuset目录改成`/dev/cpuset/system-control-apps`，也不知道Native Detector检测这个做啥？
- 不再挂载`/system/vendor/bin/msm_irqbalance`文件，因为`cpu_control.sh`也有关闭`msm_irqbalance`服务的脚本。
- 调整闲鱼线程。
> 22.9
- 添加金山文档 (`cn.wps.yun`)，Xmind (`net.xmind.doughnut`)适配。
- 修正一个`dalvik.vm.jitmaxsize`的**prop**[错误解读](https://android.googlesource.com/platform/art/+/refs/heads/main/runtime/jit/jit_code_cache.h)，之前搞反了，这个值最多是`64`MB。