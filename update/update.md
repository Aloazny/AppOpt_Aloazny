### 感谢[@coolapk SutoLiu 大佬](https://www.coolapk1s.com/u/SutoLiu)
### 原模块地址: https://gitee.com/sutoliu/AppOpt
#### 二改模块日志

> 27.0
- 添加星际战甲(Warframe) (`com.digitalextremes.warframemobile`)，osu! (lazer) (`sh.ppy.osulazer`)，Subway City(地铁跑酷: 城市)(`com.sybogames.subway.surfers.game`)适配。
- 调整小猿搜题线程`shHandlerThread`，避免占用超大核。
> 26.9
- 添加优享乐视频(`com.yishu.ylxsp`)、vivo浏览器(`com.vivo.browser`)适配。
> 26.8
- 添加Readest(`com.bilingify.readest`)， AB DM(`com.abdownloadmanager`)适配。
- 调整绝区零线程。
> 26.7
- 补充适配阅读(`com.legado.app.release`)/王牌竞速(`com.netease.aceracer.huawei`)。
- 尝试修复实时模式版本的一些Bug，感谢[@coolapk 随心随心](http://www.coolapk.com/u/40536502)测试。
> 26.6
- 添加Github(`com.github.android`)，floccus同步 (`org.handmadeideas.floccus`)，蓝云(`com.tooyoung.lanzou`)适配。
- 添加明日方舟终末地 (`com.hypergryph.endfield`)适配。
> 26.5
- 补全Fulguris (`net.slions.fulguris.full.download`, `net.slions.fulguris.full.fdroid`, `net.slions.fulguris.full.playstore`)适配。
- 添加逆战:未来 (`com.tencent.tmgp.nz`)适配。
> 26.4
- 添加Gemini(Google)(`com.google.android.googlequicksearchbox`, `com.google.android.apps.bard`)和Fulguris(`net.slions.fulguris.full.download`)适配。
- 调整`IDM+`和`ThreadPoolForeg`线程。
> 26.3
- 调整`MTOOLS`的`Web Content`线程。
- 调整`ThreadPoolForeg`线程，轻榴浏览器单独调为`大核+小核`，其他丢给小核心。
> 26.2
- 调整`ThreadPoolForeg`线程。
- 为`MTK`平台`Zygisknext`用户，未开启`排除列表策略`的，自动开启。
> 26.1
- 添加Retro Player (`code.name.monkey.retromusic`)，BiliPai(`com.android.purebilibili`)，PiliPro(`com.example.pilipro`)适配。
- 调整QQ线程。
> 26.0
- 添加轻榴浏览器(`com.mingzhisoft.browser`)和Stargon(`net.onecook.browser`)适配。
- 调整`2+3+2+1`/`6+2`线程和微信小程序线程。
- `AppOpt`去掉通过`mmap`读取配置文件。
> 25.9
- 添加efootball™(`jp.konami.pesam`)适配，感谢[Github@wang42310-sudo](https://github.com/wang42310-sudo)提供线程图。
- 添加Phocid(`org.sunsetware.phocid`)，Gramophone(`org.akanework.gramophone`)，光锥音乐(`ink.trantor.coneplayer`)，NBA2K26MyTEAM(`com.t2ksports.myteam2k26v2`,`com.t2ksports.myteam2k25`)，贴吧lite(第三方)(`com.huanchengfly.tieba.post`)适配。
> 25.8
- 添加诅咒之岛(`com.jurassic.world.the.cursed.isle.dinosaurs.carnivores.dino.hunter.dinos.online.trex.tyrannosaurus.simulator`)适配。
- 调整`inotify`部分代码，尝试解决部分移植包设备可能出现软重启的情况。
> 25.7
- 修复`dalvik.vm.heapstartsize`/`dalvik.vm.heapminfree`/`dalvik.vm.heapmaxfree`调整失效，不知为何`magisk30200`以后，有的`[[]]`没法用，必须带上`if`。
> 25.6
- 调整`Joiplay`/`Maldives`/`Aopaop`模拟器相关线程。
- 调整`cpu_control.sh`，函数`set_target_cpuaffinity`可以单独提取出来自己写脚本用。
> 25.5
- 适配书旗小说(`com.shuqi.controller`)和坎特伯雷公主与骑士(`com.bilibili.snake`,`com.tencent.tmgp.bilibili.snake`,`com.bilibili.snake.mi`,`com.bilibili.snake.aligames`,`com.bilibili.snake.vivo`,`com.bilibili.snake.huawei`)，黑神话悟空(像素版)(`com.DefaultCompany.heimalouxiangsutest`)，植物大战僵尸(娘版)(`net.pvz.pgvz.zbcteam`)，剑剑剑(`com.Lonerangerix.ArrowGame`)。
> 25.4 
- 适配魅族桌面(`com.meizu.flyme.launcher`)。
- 调整`3+4+1`线程。
> 25.3
- 适配荒野大镖客(`com.rockstargames.rdr`)和边缘视频(`com.hjmore.wallpaper`)。
- 调整`Aloazny.sh`对`A-soul-opt`的冲突检测。
> 25.2
- 安装zygisk_next版本号超过`648`时，不再阻止挂载`/system`。
- 适配好好看(新版)和Reddit(`com.reddit.frontpage`,`com.rvx.reddit`)。
- 调整M浏览器线程和**Android system Webview**。
> 25.1
- 调整`2+3+2+1`线程。
- 调整`system_server`线程**日用应用**线程。
> 25.0
- 感谢酷友[@你在你家还是说](http://www.coolapk.com/u/31414838)测试，修正`program_ctrl.sh`脚本。
> 24.9
- 感谢酷友[@刘子涵不搞机](http://www.coolapk.com/u/23950763)测试，更新`program_ctrl.sh`脚本对`oiface`进程控制代码，之前有这问题的OPPO/一加设备可以试试。
- 调整`ThreadPool`线程。
> 24.8
- 添加无尽对决(决胜巅峰)(`com.dfjz.moba`,`com.mobile.legends`,`gg.com.mobile.legends.lite`,`com.dfjz.moba.aligames`,`com.dfjz.moba.mi`)，Sparkle(`com.sparkle.tool`)，即看影视 (`skymac.kmapp.app`)，超凡先锋 (`com.tencent.tmgp.cfxf`,`com.netease.cfxf.huawei`,`com.netease.cfxf.mi`)适配。
> 24.7
- 适配PipePipe(beta)(`project.pipepipe.app`)，YouTube(`com.google.android.youtube`)。
- 修正`anti_evil_zygisk.sh`的错误，适配删除`bin.mt.plus.termex`。
> 24.6
- 添加洛克王国世界(`com.tencent.nrc`)，欢乐钓鱼大师 (`com.xuejing.smallfish.official`,`com.xuejing.smallfish.mi`,`com.tencent.tmgp.hldyds`,`com.xuejing.smallfish.huawei`,`gg.com.fishgame.fishon`)，omofun(开发版)(`com.future.anime`)，动漫共和国(`com.shizi.tool.p3`)适配。
- 调整部分日用软件线程。
> 24.5
- 添加Komikku(`app.komikku`,`app.komikku.beta`)，星露谷(Stardew Valley)(`com.zane.stardewvalley`)适配。
> 24.4
- 添加小猿搜题(`com.fenbi.android.solar`)，Face book(脸书)(`com.facebook.katana`)，囧次元(`com.llmm.huiyuanuxiang`)，omofun(`com.omofuns.com`)，二重螺旋(`com.hero.dna.gf`)适配。
- 完善`拼多多`线程，`淘宝`，`饿了么`线程。
> 24.3
- 添加Myplayer(`com.ltj.myplayer`)，Arcplayer (`com.videoplayer.arcplayer`)，枪火重生(`com.duoyi.m2m1`)，多乐够级 (`com.k7k7.goujihd`,`com.k7k7.goujihd.mi`,`com.k7k7.goujihd.huawei`)，Xplayer(`video.player.videoplayer`)适配。
- 修复`anti_evil_zygisk.sh`的一个bug。
> 24.2
- 添加中国联通(`com.sinovatech.unicom.ui`)，同程旅行(`com.tongcheng.android`)，方舟生存进化(`com.studiowildcard.arkuse`)适配。
> 24.1
- 添加铁路12306 (`com.MobileTicket`)适配。
- 限制`KB视频工厂`的`cling`线程到小核心。
> 24.0
- 添加Next player (`dev.anilbeesetti.nextplayer`)，VLC player (`org.videolan.vlc`)，Pixel桌面 (`com.google.android.apps.nexuslauncher`)，launcher3桌面 (`com.android.launcher3`)，OneUI桌面 (`com.sec.android.app.launcher`)，NOVA桌面(`com.teslacoilsw.launcher`)，Square Home(`com.ss.squarehome2`)，微软桌面(microsoft launcher)(`com.microsoft.launcher`)适配。
- 完善蛋仔派对其他渠道服匹配，添加`Blibili服`/`小米服`/`vivo服`……。
- 移除`ColorsOS`部分系统应用线程规则。
> 23.9
- 添加Reex(`xyz.re.player.ex`)适配。
- 适配`KB视频工厂`老版本的一个线程。
> 23.8
- 添加格式工厂(`com.geshi.formatfactory`)，可口的比萨(`com.tapblaze.pizzabusiness`)，风云格式工厂(`net.windcloud.explorer`)适配。
- 适配`idm+(1dm+)`新版本线程。
> 23.7
- 添加雀魂麻将(`com.soulgamechst.majsoul`)，MMRL (`com.dergoogler.mmrl`)，线程优化管理(`com.AppOptManager`)适配。
- 调整MIUI的CPU亲和系统`prop`值。
> 23.6
- 修复`AppOpt`意外终止的bug。
> 23.5
- 适配Vie浏览器 (`kawaii.vie`)，美团小游戏，小米浏览器国际版。
- 调整`cpu_control.sh`，减少对`migt`的修改。
- 修改**poweramp线程**的一个bug。
> 23.4
- 添加空洞骑士 (`com.TeamCherry.HollowKnight`)，以闪亮之名 (`com.tencent.tmgp.yslzm`,`com.zulong.yslzm.mi`,`com.archosaur.sea.yslzm.gp`)，Kotatsu(`org.koitharu.kotatsu`)适配。
> 23.3
- 添加最终幻想14水晶世界(`com.tencent.tmgp.fmgame`)，时空猎人(`com.yinhan.hunter.yh`,`com.yinhan.hunter.mi`,`com.yinhan.hunter.uc`,`com.yinhan.hunter.huawei`,`com.yinhan.hunter.tx`,`com.yinhan.hunter.qihoo`)，射雕(`com.netease.sdsbq`,`com.netease.sdsbq.mi`,`com.netease.sdsbq.huawei`)，照片编辑器(photo editor) (`com.iudesk.android.photo.editor`)，GreenTuber(`by.green.tuber`)适配。
> 23.2
- 添加悠悠有品 (`com.uu898.uuhavequality`)，Han1meviewer (`com.yenaly.han1meviewer`)，有道云笔记 (`com.youdao.note`)适配。
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
> 22.8
- 调整`cpu_control.sh`运行节点，避免开机后不能绑定部分线程。
> 22.7
- 添加ProxyPin (`com.network.proxy`)，Reqable (`com.reqable.android`)适配。
- 调整豆包垃圾线程`flow-audio-play`到小核簇。
- 调整`2+3+2+1`线程分配。
> 22.6
- 修改`dalvik.vm.heap{minfree/maxfree/startsize}`的prop值，加快冷启动。
> 22.5
- 紧急删除一个`Fold Craft Launcher`错误线程。
> 22.4
- 添加应用隐藏列表(HMAL开源) (`com.google.android.hmal`)适配。
- 调整`pubg`和`Fold Craft Launcher`部分线程。
> 22.3
- 添加七猫小说 (`com.kmxs.reader`)适配。
- 调整`cpu_control.sh`脚本，使得调整内核线程的代码更方便些。
> 22.2
- 添加飞机大厨：空中烹饪 (`com.nordcurrent.flyingfever`)适配，感谢酷友[奶一口睡觉](http://www.coolapk.com/u/18777042)。
- 调整Nagram线程，`decodeQueueorg.`线程到小核簇。
> 22.1
- 调整`3+4+1`和`2+3+2+1`部分线程，感谢酷友[iMKion](http://www.coolapk.com/u/1072357)建议。
> 22.0
- 添加QuickEdit(`com.rhmsoft.edit.pro`)适配。
- [1.3.5](https://aloazny.lanzouo.com/b00jeipeeb)和[实时模式](https://aloazny.lanzouo.com/b00jeku6cd)版本加入云更新。
- 添加哔哩哔哩动画线程。
> 21.9
- 添加Microsoft 365 (Office) (`com.microsoft.office.officehub`,`com.microsoft.office.officehubrow`)适配。
- 调整媒体存储设备。
> 21.8
- 尝试修改`AppOpt`的60秒过滤机制。
- 添加Bromite Webview (`org.bromite.webview`)适配。
- 调整KB视频工厂线程。
> 21.7
- 尝试修复`Android 15`报错，`AppOpt`意外退出，感谢[Broneily](http://www.coolapk.com/u/24964487)测试。
- 添加WPS (`cn.wps.moffice_eng`)适配。
- 添加kazumi新版本一个`RenderThread`线程。
> 21.6
- 调整内存分配，感谢酷友[糕手吃蟹堡](http://www.coolapk.com/u/32321784)和[老张子z](http://www.coolapk.com/u/10543701)测试。
> 21.5
- 添加Fold Craft Launcher (`com.tungsten.fcl`)，炽焰天穹(绯染天空) (`com.bilibili.heaven`,`com.heavenburnsred.kbinstaller`,`com.heavenburnsred`)，创造与魔法 (`com.hero.sm.bz`,`com.hero.sm.android.hero`,`com.hero.sm.mi`,`com.hero.sm.aligames`,`com.hero.sm.huawei`,`com.tencent.tmgp.sm`)，数据备份 (`com.xayah.databackup.premium`,`com.xayah.databackup.foss`)适配。
> 21.4
- 添加航海王：燃烧意志 (`com.pkwan.op.toufang.dy`,`com.tencent.tmgp.pkwan.op`)适配。
- 添加DNF一个新线程`LogicThread`，感谢酷友[墨色mosse](http://www.coolapk.com/u/27642366)分享。
> 21.3
- 添加阅读星(iBookstar) (`com.iBookStar.activity`)，Lithium (`com.faultexception.reader`)适配。
- 为`cpu_control.sh`添加部分注释。
- 调整`6+2`线程分配。
> 21.2
- 添加掌阅(iReader) (`com.chaozh.iReaderFree`,`com.chaozh.reader`,`com.mci.smagazine`)适配。
> 21.1
- 添加Nullgram (`top.qwq2333.nullgram`)，Cherrygram (`uz.unnarsx.cherrygram`)适配。
- 限制Tg第三方客户端，Neko/Nag一个异常负载的线程`decodeQueueorg.`。
- 调整**MIUI桌面线程**。
- 调整`Aloazny.sh`脚本。
> 21.0
- 添加Tubular (`org.polymorphicshade.tubular`)，Google play商店 (`com.android.vending`)，Aurora (`com.aurora.store`)，Fdroid (`org.fdroid.fdroid`)，Droidfy (`com.looker.droidify`)适配。
- 优化内存占用。
> 20.9
- 优化内存占用。
> 20.8
- 添加`persist.sys.miui_animator_sched.enabled=false`值，**用于禁用MIUI自带线程绑定**。
- 调整网易云/`lspd`线程。
> 20.7
- 添加飞书(Lark) (`com.larksuite.suite`,`com.ss.android.lark`)，招商银行 (`cmb.pb`)适配。
> 20.6
- 添加小黑盒(`com.max.xiaoheihe`)，Steam(`com.valvesoftware.android.steam.community`)适配。
> 20.5
- 调整`6+2`核心设计修改，区分高性能和中低端性能的处理器，高性能处理器减少跨簇的情况。
- 调整`哔哩哔哩`和`永劫无间`线程。
> 20.4
- 添加百度贴吧(`com.baidu.tieba`)，Google谷歌相机(AGC) (`com.agc.gcam92`,`com.samsung.agc.gcam92`)适配。
- 完善影之诗(ShadowverseWB) (`com.netease.yzs`,`jp.co.cygames.ShadowverseWorldsBeyond`)适配。
> 20.3
- 修正`program_ctrl.sh`对`oiface`的运行检测。
- 调整`uninstall.sh`脚本，添加`Joyose`云控配置下发广播命令。
- 调整`Apple Music`线程和更新`Spotify`线程。
> 20.2
- 添加Lanerc(`com.xuzly.hy.lanerc.app`)，LX Music(`cn.toside.music.mobile`)适配。
- 调整**番茄小说**线程。
> 20.1
- 优化上个版本通配符的性能消耗。
> 20.0
- 添加**进程通配符优先级排序**，和线程优先级逻辑一致。
> 19.9
- 删除了`riscv64`架构(**毕竟也没有人用**)，节省模块体积。
- 修复`AppOpt`一个配置文件更新可能存在的bug。
- 调整**三角洲行动、我的世界线程**。
> 19.8
- 修改`Aloazny.sh`对`/`或者`\/`开头的不做`#`注释。
- 调整`cpu_control.sh`，将`magiskd`/`zygiskd`/`charge_logger`/`mdnsd`/`logd`放到小核簇。
> 19.7
- 完善初音未来: 缤纷舞台 (`com.hermes.mk`,`com.sega.pjsekai`,`com.hermes.mk.asia`,`com.hermes.mk.bilibili`)，BanG Dream! 少女樂團派對 (`com.bilibili.star.bili`,`jp.co.craftegg.band`,`com.bushiroad.en.bangdreamgbp`,`net.gamon.bdTW`)适配。
- 添加学园偶像大师(`com.bandainamcoent.idolmaster_gakuen`)适配。
> 19.6
- 限制`增量更新`时，刷入时规则变动显示内容。
- 添加BanGdream(日服)匹配。
> 19.5
- 添加Github云更新地址。
> 19.4
- 添加王者荣耀国际服(`com.levelinfinite.sgameGlobal`,`com.levelinfinite.sgameGlobal.midaspay`)，全明星街球派对(`com.netease.allstar`)，美职篮全明星(NBA2K All Star) (`com.tencent.nba2kx`)适配。
> 19.3
- 适配**MTOOLS模拟器新版本线程**。
> 19.2
- 调整线程`6+2`部分线程。
- 调整MIUI相册后台的`video_tag`和`faceCluster`线程，避免使用超大核造成后台耗电严重和发烫。
> 19.1
- 修复启用`增量更新`时，线程包含空格的问题。
> 19.0
- 添加刷入时，启用`增量更新`时，**规则变动和用户规则**显示。
> 18.9
- 修正`Aloazny.sh`执行增量更新无意义报错的bug。
> 18.8
- 添加QQ炫舞 (`com.tencent.tmgp.qqx5`)，节奏大师 (`com.tencent.game.rhythmmaster`)适配。
- **注意！不想动手改规则的，一般不要用增量更新，不然你的规则可能永远停留在旧版本**
- 添加`增量更新`状态显示。
> 18.7
- 添加增量更新模式(**实验**)，在启用`update_config`和`modtify_config`情况下，用户自行添加的规则会被添加到配置文件最底部的`#用户规则`类别，**规则不一致时以用户修改的规则为主**。
- flag文件以`/data/adb/modules/AppOpt_Aloazny/keep_custom_rule`空文件作为标识。
> 18.6
- 添加Firefox(火狐)浏览器 (`org.mozilla.firefox`,`org.mozilla.fenix`)适配。
- 添加安装时`program_ctrl.sh`热更新。
- 更新**原神、星穹铁道、绝区零线程**。
> 18.5
- 添加AppList配置管理器(`com.reddoctor.threadui`)，Ksu webui (`io.github.a13e300.ksuwebui`)适配。
- 调整`Aloazny.sh`，`program_ctrl.sh`，`service.sh`脚本。
> 18.4
- 添加vivaldi浏览器 (`com.vivaldi.browser`)，三星浏览器( Samsung Internet Browser) (`com.sec.android.app.sbrowser`,`com.sec.android.app.sbrowser.beta`)适配。
- 新增通过创建`/data/adb/modules/AppOpt_Aloazny/{disable_program/enable_program}`空文件，用于控制是否禁用一些冲突(**如果存在**)的系统进程，具体请自行查看`program_ctrl.sh`脚本。
> 18.3
- 调整`Aloazny.sh`和`uninstall.sh`脚本。
> 18.2
- 添加Brave浏览器(`com.brave.browser`,`com.brave.browser_nightly`,`com.brave.browser_beta`)，火炬之光：无限 (`com.xindong.torchlight`)适配。
- 更改`❌`为`❎️`，减少部分人强迫症发作。
> 18.1
- 添加模块`Flags`文件状态显示。
> 18.0
- 添加战争雷霆(War Thunder Mobile) (`com.gaijingames.wtm`)，Chrome浏览器(`com.android.chrome`,`com.chrome.canary`,`com.chrome.beta`,`com.chrome.dev`)适配。
- 调整**AppOpt**内存分配。
- 调整**暗区突围线程**。
> 17.9
- 修复`Aloazny.sh`脚本中对`dex2oat`的线程数设定。
> 17.8
- 添加磁力猫(`com.mankson.reader`)，鬼谷八荒(`com.guigugame.guigubahuang`)，网易云音乐(荣耀定制版)(`com.hihonor.cloudmusic`)适配。
> 17.7
- 修复**AppOpt**部分问题。
- 添加蔚蓝档案其他服务器(`com.nexon.bluearchive`,`com.YostarJP.BlueArchive`)。
> 17.6
- 再次调整**我的世界线程**，压榨下大核簇。
> 17.5
- 调整**Fortnite、系统UI、我的世界线程**。
> 17.4
- 添加轻小说文库(`org.mewx.wenku8`)，CMOE3.0(`com.alibaba.android.rimet.catlcome`)，欢乐斗地主(`com.qqgame.hlddz`)适配。
- 修复`Aloazny.sh`对`dex2oat`的prop值设定部分重复的小bug。
> 17.3
- 修复`Aloazny.sh`对低版本Scene冲突包名检测。
> 17.2
- 修复部分联发科设备，无法使用`sed`命令配置文件不准确的问题。
> 17.1
- 添加`dex2oat`的prop值修改，用于优化安装应用速度和运行速度，默认不修改，要修改，自行创建`/data/adb/modules/AppOpt_Aloazny/dexota_modtify`空文件。
- 调整**永劫无间、系统UI线程**线程。
> 17.0
- 修复一个`Aloazny.sh`MIUI版本识别错误的bug。
> 16.9
- 添加MusicFree (`fun.upup.musicfree`)适配。
- 同步更新`AppOpt`至上游`1.6.3`。
> 16.8
- 添加多看阅读(`com.duokan.reader`,`com.duokan.r`)，MultiTTS(`org.nobody.multitts`)，静读天下(`com.flyersoft.moonreaderp`)适配。
- 调整`miui_booster`加速。
> 16.7
- 修复**AppOpt**更新配置文件后，不会触发全量扫描`/proc`的bug。
> 16.6
- 调整**少女前线、闲鱼**线程。
> 16.5
- 添加Breezy Weather(`org.breezyweather`)适配。
- 添加好好看(`com.hhk.hh*`)通配符进程适配，顺便做示例。
- 例如`com.hhk.hh*{RenderThread}=7`规则相当于匹配到任何以`com.hhk.hh`开头的进程的`RenderThread`线程，也就是`com.hhk.hh21{RenderThread}=7`或者`com.hhk.hh20{RenderThread}=7`都会被匹配到。
> 16.4
- 更正`module.prop`信息。
- 调整**萤火突击(部分)线程**。
> 16.3
- 添加我的世界(腾讯)(`com.tencent.tmgp.wdsj666`)，妄想山海(`com.tencent.tmgp.djsy`)适配。
- 调整**我的世界、萤火突击(部分)线程**。
> 16.2
- 更新`AppOpt`至上游`1.6.1`。
> 16.1
- 添加Turrit(`org.telegram.group`)，下饭影视(`com.suiyiji.syj`)适配。
- 调整**SonicePlus线程**。
> 16.0
- 添加逃跑吧少年(`com.bairimeng.dmmdzz`,`com.bairimeng.dmmdzz`,`com.bairimeng.dmmdzz.mi`,`com.bairimeng.dmmdzz.betazone`,`com.bairimeng.dmmdzz.m4399`,`com.bairimeng.dmmdzz.honor`,`com.bairimeng.dmmdzz.vivo`,`com.tencent.tmgp.bairimeng.dmmdzz`)适配。
- 调整Unity引擎的`FMOD`音频线程和`UnityChoreograp`非重要渲染线程到小核簇。
> 15.9
- `AppOpt`重建缓存的进程变化退回。
> 15.8
- 添加影之诗 (`com.netease.yzs`)适配。
- 调整`Perfect Viewer`线程。
- `AppOpt`重建缓存的进程变化改成`5`。
- 回退`Aloazny.sh`脚本逻辑。
> 15.7
- 添加VueTube(`com.Frontesque.vuetube`)，HLB站缓存合并(`com.molihua.hlbmerge`)适配。
- 修复`Aloazny.sh`的一个bug。
- 调整`cpu_control.sh`。
> 15.6
- 添加Icey Player (`com.icey.player`)，Librera (`com.foobnix.pro.pdf.reader`)，快图浏览 (`com.alensw.PicFolder`)适配。
- 调整`AppOpt`错误规则检测输出。
> 15.5
- 添加B仔浏览器(`com.huicunjun.bbrowser`)，MDM 浏览器(`moe.browser`)适配。
- 调整部分线程。
> 15.4
- 修复`kernel su`上面执行`Action`时`grep_prop`函数的错误。
> 15.3
- 添加音乐剪辑(`www.imxiaoyu.com.musiceditor`)，倒带(`com.kuss.rewind`)适配。
- 调整**明日之后线程**。
- 恢复简单日志，用于**输出错误规则**情况和`inotify`的情况。
> 15.2
- 添加腾讯地图(`com.tencent.map`)，最强NBA (`com.tencent.tmgp.NBA`)适配。
- 调整部分日用应用线程。
> 15.1
- 更新`1.5.6`底包。
- **抛弃日志输出**。
- 更新和[A-soul](https://github.com/nakixii/Magisk_AsoulOpt)冲突包名的列表。
> 15.0
- 添加搜图神器(`com.example.administrator.searchpicturetool`)，猫和老鼠(`com.netease.tom.mi`,`com.netease.tom`)适配。
> 14.9
- 添加抖音Play版(`com.ss.android.ugc.aweme.mobile`)适配。
- 修复一个`Aloazny.sh`脚本的bug。
> 14.8
- 添加Bilimiao(`com.a10miaomiao.bilimiao`)，拷贝漫画(`top.fumiama.copymanga`)适配。
- 调整**DNF(地下城与勇士：起源) (`com.tencent.tmgp.dnf`)线程**。
- 调整**pubg/和平精英**线程。
> 14.7
- 调整部分应用/游戏线程。
> 14.6
- 调整**MIUI桌面**线程，尝试解决`hyperos`上MIUI掉帧的问题。
> 14.5
- 添加达达秒送骑士(`com.dada.mobile.android`)，纯粹直播(purelive)(`com.mystyle.purelive`)，Pure天气 (`hanjie.app.pureweather`)适配。
> 14.4
- 添加异人之下(`com.tencent.YiRen`)适配。
- 调整**pubg/和平精英**线程。
> 14.3
- 添加知乎(`com.zhihu.android`)，Hydrogen(`com.zhihu.hydrogen.x`)适配。
> 14.2
- 补充TIKTOK(`com.ss.android.ugc.trill`)和暗区突围(`com.proximabeta.mf.uamo`)包名适配。
> 14.1
- 添加TIKTOK(`com.zhiliaoapp.musically`)，抖音精选(`com.ss.android.yumme.video`)，今日头条(`com.ss.android.article.news`)适配。
> 14.0
- 搞了半天，还是退回**旧版本AppOpt**吧。
> 13.9
- 完善MIUI桌面(`com.miui.home`)部分线程。
- 添加圣斗士星矢(`com.tencent.tmgp.sskgame`)适配。
- 修复上个版本**AppOpt**的内存泄露bug。
> 13.8
- 添加月圆之夜(`com.ztgame.yyzy`,`com.ztgame.yyzy.aligames`)，我的听书(`com.github.eprendre.tingshu`)，巅峰极速(`com.netease.dfjs`,`com.netease.dfjs.aligames`,`com.netease.dfjs.mi`)适配。
> 13.7
- 添加微软翻译(Translator) (`com.microsoft.translator`)，MIUI主题壁纸 (`com.android.thememanager`)适配。
- 移除`cpu_control.sh`对`Colors OS`修改。
- 退回`2+3+2+1`修改，整体进程还是改为调用大核簇`2-6`。
- 调整Appopt的`getdents`缓冲区大小。
> 13.6
- 尝试修改Appopt，使用`getdents`批量获取`/proc`目录。
> 13.5
- 修复`action.sh`的一个bug。
> 13.4
- 添加ADM下载器(`com.dv.adm`)，战魂铭人(`com.chillyroom.zhmr.yw`,`com.chillyroom.zhmr.gp`,`com.chillyroom.zhmr.mi`,`com.chillyroom.zhmr.aligames`)适配。
> 13.3
- 添加哈啰共享单车(`com.jingyao.easybike`)适配。
- 调整部分**老旧的线程分配**。
> 13.2
- 添加Google相机(MGC)(`com.android.MGC`)，腾讯桌球(`com.tencent.pocket`)适配。
> 13.1
- 添加棕色塵埃2-冒險RPG(BrownDust2)(`com.neowizgames.game.browndust2`)适配。
- **更正元梦之星线程。**
> 13.0
- 添加Google相机(三星底包)(`com.samsung.android.scan3d`)适配。
- **更正蛋仔派对线程。**
> 12.9
- 修复`cpu_control.sh`的一个bug。
> 12.8
- 添加创建快捷方式(`com.x7890.shortcutcreator`)，吾爱PSP模拟器 (`com.aiwu.psp`)，PPSSPP(`org.ppsspp.ppsspp`,`org.ppsspp.ppssppgold`)适配。
> 12.7
- 尝试解决部分设备出现卡死问题，移除`/sys/module/metis/parameters`修改。
> 12.6
- 添加MiXplorer(`com.mixplorer.silver`)，Aplayer(`remix.myplayer`)，极品飞车: 集结(`com.tencent.nfsonline`)适配。
- 调整**鸣潮线程**。
> 12.5
- 添加初音未来: 缤纷舞台 (`com.hermes.mk`)，英魂之刃 (`com.nd.he`,`com.nd.he.mi`,`com.nd.hoa.aligames`,`com.nd.he.gamename.m4399`)，蔚蓝档案 (`com.RoamingStar.BlueArchive`)，杀戮尖塔(`com.humble.SlayTheSpire`)，时光大爆炸 (`com.sqw.cc.sgdbz_ta`)适配。
> 12.4
- 添加抖音极速版(`com.ss.android.ugc.aweme.lite`)适配。
- 调整**ZArchiver Pro**线程，优化解压缩功耗。
> 12.3
- 添加汽水音乐(`com.luna.music`)，喜马拉雅(`com.ximalaya.ting.android`)，Himalaya(`com.ximalaya.ting.himalaya`)，喜马拉雅车机版(`com.ximalaya.ting.android.car`)适配。
> 12.2
- 添加Gboard(`com.google.android.inputmethod.latin`)，语燕输入法 (`com.yuyan.pinyin.offline.release`)，微信输入法(`com.tencent.wetype`)适配。
- 适配Android7以下的**Webview线程**。
> 12.1
- 添加部落冲突(play)(`com.supercell.clashofclans`)和皇室战争(play)(`com.supercell.clashroyale`)适配。
> 12.0
- 调整萤火突击线程。
- 鉴于有的人完全不懂创建空文件，**默认第一次刷入时创建`/data/adb/modules/AppOpt_Aloazny/update_config`和`modtify_config`**。
> 11.9
- 添加萤火突击(`com.netease.yhtj.m4399`,`com.netease.yhtj.gg`,`com.netease.yhtj.aligames`,`com.netease.yhtj.mi`)适配。
> 11.8
- 添加AGE动漫 (`com.agefans.sp3`)，乐秀视频 (`com.xvideostudio.videoeditor`)适配。
> 11.7
- 添加Blocker(`com.merxury.blocker`)，吉里吉里(kirikiri) (`org.tvp.kirikiri2_free_10309`)，ToolWiz Photos(`com.btows.photo`)适配。
> 11.6
- 添加代号血战(Blood Strike)(`com.netease.newspike`)，Pica Comic(`com.github.pacalini.pica_comic`)，开放空间(`com.Nekootan.kfkj.android`,`com.Nekootan.kfkj.yhlm.aligames`,`com.tencent.tmgp.Nekootan.kfkj.yhlm`,`com.Nekootan.kfkj.yhlm.mi`)适配。
> 11.5
- 添加BanGdream(`com.bilibili.star.bili`)，天翼云盘(`com.cn21.ecloud`)适配。
- 修复`cpu_control.sh`的bug。
- 将360极速浏览器和UC浏览器的垃圾线程`Busy-*`和`spdy-*`放到小核去跑。
> 11.4
- 添加SonicePlus (`com.adtie.klan`)，资源猫(`com.walixiwa.flash.player`)，卡拉彼丘(`com.idreamsky.klbqm`)，解压缩全能王(`com.cjtec.uncompress`)适配。
> 11.3
- 添加文件管理器+(`com.alphainventor.filemanager`)，无畏契约(`com.tencent.tmgp.codev`)适配。
> 11.2
- 添加三国杀(`com.bf.sgs.hdexp`,`com.bf.sgs.mi`,`com.bf.sgs.hdexp.m4399`)，元气骑士前传(`com.humo.yqqsqz.yw`,`com.humo.yqqsqz.hykb`,`com.humo.yqqsqz.bilibili`,`com.humo.yqqsqz.mi`)适配。
- 修正**尘白禁区**线程。
> 11.1
- 添加胜利女神Nikke(`com.tencent.nikke`,`com.gamamobi.nikke`,`com.proximabeta.nikke`)，王牌战争(`com.yingxiong.heroo.nearme.gamecenter`)，Simple Live(`com.xycz.simple_live`)，Tachiyomi (`eu.kanade.tachiyomi`,`eu.kanade.tachiyomi.j2k`,`eu.kanade.tachiyomi.yokai`)适配。
> 11.0
- 添加香肠派对(`com.sofunny.Sausage`)，球球大作战 (`com.ztgame.bob`)，跑跑卡丁车(`com.tencent.tmgp.WePop`)，晶核(`com.hermes.p6game`,`com.hermes.p6game.mi`,`com.hermes.p6game.aligames`)适配。
- 将线程`Job.worker`改成`Job.[Ww]orker`适配。
> 10.9
- 将部分游戏的线程`Job.worker`改为`Job.Worker`。
> 10.8
- 添加小米视频 (`com.miui.video`)，媒体查看器 (`com.miui.mediaviewer`)适配。
- 更新`cpu_control.sh`脚本，添加对`kswapd0`和`kcompactd0`内核进程以及`lmkd`核心分配。
> 10.7
- 添加LibChecker(`com.absinthe.libchecker`)，Roblox(`com.roblox.client`)适配。
- 调整**浏览器线程**。
> 10.6
- 优化`AppOpt`的`load_config`性能。
> 10.5
- 添加弹弹play概念版(`com.xyoye.dandanplay`)适配。
- 调整Animeko(`me.him188.ani`)线程。
> 10.4
- 修复`Appopt`一个内存泄露的问题。
> 10.3
- 添加kiwi Browser(`com.kiwibrowser.browser`)，雨见(国际版) (`com.yjllq.internet`)，可拓浏览器 (`com.yjllq.kito`)，红雨见(chromium内核) (`com.yjllq.chrome.beta`,`com.yjllq.chrome`)适配。
> 10.2
- 调整`AppOpt`可能存在的问题。
- 添加巅峰极速(`com.netease.race`,`com.netease.race.ua`)，阴阳师(`com.netease.onmyoji`,`com.netease.onmyoji.vivo`,`com.netease.onmyoji.wyzymnqsd_cps`,`com.netease.onmyoji.bili`,`com.netease.onmyoji.mi`)，对峙2 (standoff2) (`com.axlebolt.standoff2.huawei`,`com.axlebolt.standoff2`)适配。
> 10.1
- 创建`/data/adb/modules/AppOpt_Aloazny/delete_game_config`空文件，**就会删除自带的游戏配置**。
> 10.0
- 添加QQ浏览器国际版(`com.tencent.mtt.intl`)，QQ浏览器(`com.tencent.mtt`)，M浏览器(3.X)(`cn.mujiankeji.mbrowser`)适配。
> 9.9
- 适配迅雷(`com.xunlei.downloadprovider`)。
> 9.8
- 添加心绪日记(`cn.yooss.moodiary`)，小米笔记 (`com.miui.notes`)，Quetta浏览器 (`net.quetta.browser`)适配。
- 调整支付宝(`com.eg.android.AlipayGphone`)，UC浏览器(`com.UCMobile`)，极速浏览器(`com.qihoo.contents`)线程。
> 9.7
- 移除`Fnmatch`的`FNM_NOESCAPE`flag，线程中的特殊符号`*?[]{}`，只需要一个转义使用"\"作为转义即可，不需要"\\"。
> 9.6
- 添加元气骑士(`com.ChillyRoom.DungeonShooter`)，少女前线(`com.Sunborn.SnqxExilium`, `com.sunborn.snqxexilium.glo`)，皇室战争(`com.tencent.tmgp.supercell.clashroyale`)
适配。
- 修复阅读一个线程分配的错误。
> 9.5
- 添加Tyranor(`com.akira.tyranoemu`)，Minecraft(`com.mojang.minecraftpe`)，我的世界(网易)(`com.netease.x19`)，迷你世界(`com.minitech.miniworld`,`com.minitech.miniworld.TMobile.mi`,`com.tencent.tmgp.minitech.miniworld`,`com.minitech.miniworld.uc`,`com.playmini.miniworld`)，尘白禁区(`com.dragonli.projectsnow.lhm`,`com.dragonli.projectsnow.bilibili`)，Snapseed (`com.niksoftware.snapseed`)，UC浏览器 (`com.UCMobile`)，Outlook(`com.microsoft.office.outlook`)，网易邮箱(大师)(`com.netease.mail`)，QQ邮箱(`com.tencent.androidqqmail`)适配。
> 9.4
- 添加狂野飙车9 (`com.aligames.kuang.kybc.aligames`,`com.tencent.tmgp.aligames.kybc`,`com.aligames.kuang.kybc.mi`,`com.aligames.kuang.kybc.tap`,`com.aligames.kuang.kybc`)，荒野乱斗 (`com.tencent.tmgp.supercell.brawlstars`)适配。
- 调整`2+3+2+1`配置，调整`cpu_control.sh`代码。
> 9.3
- 修改`AppOpt`部分代码，减少性能开销。
> 9.2
- 添加隐藏应用列表 (`com.tsng.hidemyapplist`)，百度网盘(小米联合开发版) (`com.baidu.netdisk.xiaomi.appunion`)，百度网盘 (`com.baidu.netdisk`)，123网盘 (`com.mfcloudcalculate.networkdisk`)适配。
> 9.1
- 修复海阔视界(`com.example.hikerview`)进程规则复制成嗅觉(`com.hiker.youtoo`)的bug。
> 9.0
- 添加Rians 浏览器(`com.rainsee.create`)和网梭浏览器(`com.x.webshuttle`)，海阔视界(`com.example.hikerview`)，移动云盘 (`com.chinamobile.mcloud`)，小米搜索(`com.android.quicksearchbox`)适配。
> 8.9
- 减少`kill`调用和`stat`读取。
> 8.8
- 添加Via浏览器(`mark.via`,`mark.via.gp`)，X浏览器(`com.mmbox.xbrowser`,`com.mmbox.browser`,`com.x.browser.x5`,`com.xbrowser.play`)，Soul浏览器(`com.mycompany.app.soulbrowser`)，嗅觉浏览器(`com.hiker.youtoo`)适配。
> 8.7
- 添加Joplin笔记 (`net.cozic.joplin`)，Javaloader (`ru.woesss.j2meloader`,`ru.playsoftware.j2meloader`)适配。
> 8.6
- 同步上游`1.3.2`更新`AppOpt`，`-f`参数改为`-c`参数，避免不一致。
- 添加农业银行 (`com.android.bankabc`)，建设银行 (`com.chinamworld.main`)，云闪付 (`com.unionpay`)，北部湾银行 (`com.yitong.bbw.mbank.android`)适配。
> 8.5
- 添加现代战争3(MC3) (`com.gameloft.android.GAND.GloftM3HP`)，Google文件(`com.android.documentsui`)，快手极速版 (`com.kuaishou.nebula`)适配。
> 8.4
- 添加光遇 (`com.netease.sky`,`com.tgc.sky.android`,`com.netease.sky.nearme.gamecenter`,`com.netease.sky.bilibili`,`com.tencent.tmgp.eyou.eygy`,`com.netease.sky.mi`,`com.netease.sky.m4399`,`com.netease.sky.vivo`)，绝区零 (`com.miHoYo.Nap`,`com.mihoyo.nap.bilibili`)适配，Comic Screen (`com.viewer.comicscreen`)，Perfect Viewer (`com.rookiestudio.perfectviewer`)，MH-ARK (`com.mhdh.mh_ark`)，Sudachi (`org.sudachi.sudachi_emu`)。
> 8.3
- 尝试修复部分设备`AppOpt`无法启动的情况，如果`AppOpt`无法启动，把`/data/adb/modules/AppOpt_Aloazny/affinity_manager.log`文件通过[文叔叔](https://www.wenshushu.cn/) (免登陆)发给我(记得复制链接)。
> 8.2
- `action.sh`添加重启`AppOpt`的选项，不知道执行`action.sh`，有的设备会让AppOpt终止。
> 8.1
- 修复`AppOpt`可能存在的bug。
- 添加MIUI智能助理(负一屏) (`com.miui.personalassistant`)，Apple music (`com.apple.android.music`)，Spotify (`com.spotify.music`)适配。
> 8.0
- 添加Google 文件 (`com.google.android.documentsui`)，狐猴浏览器 (`com.lemurbrowser.exts`,`com.lemurbrowser.exts.beta`)适配。
> 7.9
- 调整了`uthash.h`缓存的部分机制。
- 添加小米浏览器(`com.android.browser`)，豆包 (`com.larus.nova`)，DeepSeek (`com.deepseek.chat`)，问小白 (`com.yuanshi.wenxiaobai`)，通义千问 (`com.aliyun.tongyi`)。
> 7.8
- 添加`cpu_control.sh`脚本。
- 添加对QQ音乐(play)版的适配以及QQ音乐(魅族定制)(`com.meizu.media.music`)适配。
- 添加AcFun(`tv.acfundanmaku.video`)适配。
> 7.7
- 添加小爱扫一扫 (`com.xiaomi.scanner`)，小爱同学(`com.miui.voiceassist`)适配。
> 7.6
- 修复上个版本`AppOpt`的一些bug。
- 添加Stay(`com.dajiu.stay`)浏览器，NGA玩家社区 (`gov.pianzong.androidnga`)，LocalSend(`org.localsend.localsend_app`)，纯纯看番 (`com.heyanle.easybangumi4`)，Mihon (`app.mihon`)，Miru (`miru.miaomint`)适配。
> 7.5
- 改回`Aloazny.sh`对`Scene核心分配的判定`，加入上游的`/proc/loadavg`机制和移除`inotify`。
- 添加阅读(`io.legado.app.release`)，阅读(play) (`io.legado.play.release`)适配。
> 7.4
- 本来想偷懒，懒得下载那些垃圾流氓应用的，结果发现别人作业里面的**虎牙线程**写错好几个线程名，就这样都没有人发现，绝了，现在已经自己下载了重写了。
- 添加斗鱼极速(play) (`com.douyu.rush`)，斗鱼直播(`air.tv.douyu.android`)适配。
> 7.3
- 添加LeapMusic (`com.leapmusic.leapmusic`)，豆瓣 (`com.douban.frodo`)，虎牙直播 (`com.duowan.kiwi`)，虎牙直播(play) (`com.huya.kiwi`)适配。
> 7.2
- 添加夸克 (`com.quark.browser`)，ALook (`alook.browser`)，最右 (`cn.xiaochuankeji.tieba`)，车来了 (`com.ygkj.chelaile.standard`)，滴滴打车 (`com.sdu.didi.psnger`)适配。
> 7.1
- 改为未安装`zygisk-maphide`或者`shamiko`两种中任意一个时才会跳过挂载`/system`。
- 添加Markor(`net.gsantner.markor`)，RAR(`com.rarlab.rar`)，ZArchiver(`ru.zdevs.zarchiver.pro`,`ru.zdevs.zarchiver`)，Edge(`com.microsoft.emmx`)，360极速浏览器(`com.qihoo.contents`)，WeTV/腾讯视频海外版 (`com.tencent.qqlivei18n`)适配。
> 7.0
- 修复上个版本`3+4+1`和`2+3+2+1`核心配置对于`2-4`未适配的bug。
- 添加Poweramp(`com.maxmpz.audioplayer`)，Image Toolbox(`ru.tech.imageresizershrinker`)，Instagram(`com.instagram.android`)适配。
> 6.9
- 微调**MT文件管理器**，稍微加快MT管理器解压速度，但是降低功耗。
- 优化**Joiplay**解密游戏速度。
- 添加酷狗音乐(`com.kugou.android`,`com.kugou.android.lite`)，Obsidian(`md.obsidian`)适配。
- 添加黄油模拟器Aopaop (`com.aopaop.app`)，Maldives (`net.miririt.maldivesplayer`)，MTOOLS (`app.mtool.mtoolmobile`)适配。
> 6.8
- 炉石传说 (`com.blizzard.wtcg.hearthstone`,`com.blizzard.wtcg.hearthstone.cn.dashen`,`com.blizzard.wtcg.hearthstone.cn.huawei`)，IDM+/1DM+ (`idm.internet.download.manager.plus`)，MIUI player/小米音乐 (`com.miui.player`)，QQ音乐 (`com.tencent.qqmusic`)，酷我音乐 (`cn.kuwo.player`)。
> 6.7
- 适配剪映(`com.lemon.lv`)，红果短剧(`com.phoenix.read`)，QQ(`com.tencent.mobileqq`)，TIM(`com.tencent.tim`)。
> 6.6
- 添加一个如果出现笔误，规则写错情况下，执行`action.sh`的修复。
- 更细致的**网易云音乐**匹配。
- 添加无限暖暖 (`com.papegames.infinitynikki`)，决战平安京 (`com.netease.moba`)，腾讯视频 (`com.tencent.qqlive`)，爱奇艺 (`com.qiyi.video`)适配。
> 6.5
- 添加虎扑 (`com.hupu.games`)，Bangumi (`com.czy0729.bangumi`)，钉钉 (`com.alibaba.android.rimet`)，PiliPala (`com.guozhigq.pilipala`)，PiliPalaX (`com.orz12.PiliPalaX`)。
> 6.4
- 添加菜鸟裹裹(`com.cainiao.wireless`)，顺丰速运(`com.sf.activity`)。
- 添加`Aloazny.sh`修改一个配置文件可能乱写导致的错误。
> 6.3
- 修复可能存在内存泄露的几个bug。
- 适配PipePipe (`InfinityLoop1309.NewPipeEnhanced`)，什么值得买 (`com.smzdm.client.android`)。
> 6.1
- 同步上游部分`1.19`代码，因为修修又补补和源码变化有点多，所以只抄了主要部分。
- 添加不能无法设置的`CPU`亲和规则日志记录。
- 适配微博 (`com.sina.weibo`)
> 6.0
- 添加空线程适配，如`example.app.com{ }=6`。
- 修复`AppOpt`可能存在的几个配置文件解析和优先级匹配的bug。
- 添加MIUI系统应用 相册 (`com.miui.gallery`)，相册编辑 (`com.miui.mediaeditor`)适配，游戏碧蓝航线(`com.bilibili.azurlane`)。
> 5.8
- 添加`Failed to set affinity`的原因。
- 配置文件如果不存在，延迟`5秒`后才会创建，避免和MT管理器冲突(MT管理器会删除再创建文件)。
> 5.7
- 细化线程匹配优先级，从低到高`example.app.com=0-3` **→** `example.app.com{Thread-*}=4-6` **→** `example.app.com{Thread-2*}=4-6` **→** `example.app.com{Thread-[0-9]}=4-6` **→** `example.app.com{Thread-2[0-9]}` **→** `example.app.com{Thread-2}=7`。
- 添加"毒奶粉"(`com.tencent.tmgp.dnf`)，快手(`com.smile.gifmaker`)匹配。
- 调整`system_server`线程分配。
> 5.6
- 修复一个`AppOpt`的bug。
- 添加JJ象棋 (`cn.jj.chess`,`cn.jj.chess.mi`)。
> 5.5
- 调整`applist.prop`配置。
- 添加哔哩哔哩play版(`com.bilibili.app.in`)，航海王热血航线(`com.hermes.h1game`,`com.hermes.h1game.m4399`)，明日之后(`com.netease.mrzh`,`com.netease.mrzh.mi`)，超自然行动组(`com.pi.czrxdfirst`)，Grok(`ai.x.grok`)。
> 5.4
- 添加对`/system/vendor/bin/msm_irqbalance`处理，如果未安装隐藏模块，则默认不挂载`/system`。
> 5.3
- 添加SD Maid(`eu.thedarken.sdm`,`eu.darken.sdmse`)，Telegram(`org.telegram.messenger`,`org.telegram.messenger.web`,`org.telegram.messenger.beta`)适配。
> 5.2
- 调整`6+2`配置，避免`sdm710`都分配给大核，部分场景卡顿。
- 修复一个**配置文件因为错误规则**卡死的bug。
- 添加kazumi(`com.predidit.kazumi`)和Animeko(`me.him188.ani`)适配。
> 5.1
- 调整`applist.prop`部分应用线程。
- 加回`Scene`版本判定(这么久了，应该改好了吧)。
> 5.0
- 添加通配符匹配进程，通配符会稍微增加性能消耗，推荐优先使用精确匹配，自行斟酌。
- 适配游戏荒野行动(`com.netease.hyxd.mi`, `com.netease.hyxd.aligames`, `com.netease.hyxd.nearme.gamecenter`, `com.netease.hyxd.wyzymnqsd_cps`)，三角洲行动(`com.tencent.tmgp.dfm`)，部落冲突(`com.tencent.tmgp.supercell.clashofclans`)
> 4.9
- 添加饿了么(`me.ele`)，美团(`com.sankuai.meituan`)。
> 4.8
- 添加日常应用适配，小红书(`com.xingin.xhs`)，高德地图(`com.autonavi.minimap`)，百度地图(`com.baidu.BaiduMap`)，闲鱼(`com.taobao.idlefish`)，淘宝(`com.taobao.taobao`)，拼多多(`com.xunmeng.pinduoduo`)。
> 4.7
- 新增优先级匹配，精确匹配优先级高于通配符匹配，如下。
```C++
exp.com{Thread-3}=7
exp.com{Thread-[1-2]3}=6
exp.com{Thread-*}=4-6
```
- 模块会优先绑定`exp.com{Thread-3}`到CPU7，`exp.com{Thread-[1-2]3}`也就是`Thread-13/Thread-23`会被绑定到CPU6，剩下的`Thread-*`会给到CPU4-6。
> 4.6
- 添加金铲铲(`com.tencent.jkchess`)。
- 添加适配应用列表，可以在`适配应用.md`查看。
> 4.5
- 添加一个MIUI相机(`com.android.camera`)的线程。
- 修改酷安、MIUI桌面线程。
> 4.4
- 添加对`system_server`的修改。
- 修复`applist.prop`中新的线程把`tv.danmaku.bili`打成`tv.danmaku.bilibilihd`。
- 调整`6+2`和`2+3+2+1`的机械性配置。
> 4.3
- 修复`4+4`把`*}`替换成`=`的错误。
- MTK屏蔽`/system/vendor/etc/power_app_cfg.xml`文件。
- 修复`核心配置`更改的一个bug。
> 4.2
- 更改一个`4+4`配置，新增`2+3+2+1`机械性配置修改。
> 4.1
- 调整`surfaceflinger`线程匹配。
> 4.0
- 添加两个`BILIBILI`线程`IJK`和`Thread-*`。
> 3.9
- 改成`aarch64-linux-android23-clang`编译，和Magisk最低支持的版本同步。
> 3.8
- 修复刷入后不修改`/data/adb/modules_update/AppOpt_Aloazny/applist.prop`的问题。
- 修改机械性适配地部分描述。
> 3.7
- 添加机械性适配`6+2` `3+4+1` `4+4` CPU核心配置的适配，创建`/data/adb/modules/AppOpt_Aloazny/modtify_config`空文件，就会修改配置。
- 创建`/data/adb/modules/AppOpt_Aloazny/update_config`空文件，就会使用模块的配置，而不是读取上一次配置。
> 3.6
- 云适配了`CF`。
> 3.5
- 添加一项配置文件`applist.prop`移除多行，**只保留一行空行**。
- 云适配了一些游戏。
> 3.4
- 添加winlator(`com.winlator`)的核心配置(其实这个主要还是看GPU)。
> 3.3
- 机械地检测了了`A-soul`模块可能会冲突的包名，自行点击Action检测。
> 3.2
- 修改`Job.worker`为`Job.worker*`。
> 3.1
- 同步`SutoLiu`的`1.0.2`版本，默认不暂停`oiface`。
- 更改卸载脚本`uninstall.sh`。
- 退回`Aloazny.sh`的核心显示。
> 3.0
- 抄了`Scene`的作业，把`Scene`的游戏核心分配置(仅配置)，改了改，加到了上面，效果可能不好。
- 添加KB视频工厂(`com.idaodan.video.factory`)的核心分配，空闲小核，减少卡顿。
- 修改`com.google.android.webview`的`Compositor`为核心6，避免在玩Joiplay时，7负载太高，有点卡顿。
> 2.9
- 修正`applist.prop`对webview和Joiplay的部分核心配置，偏向更流畅。
> 2.8
- 添加Joiplay(`cyou.joiplay.joiplay`)配置。
- 修复**inotify**的一个息屏bug，息屏后亮起，即刻修改配置文件，不会生效。
> 2.7
- Scene根本就没兼容，加回Scene检测。
- 修复`applist.prop`把`Twitter`复制粘贴成`coolapk`的一个错误线程。
> 2.6
- 修改**错误日志**输出限制为10行，剩下会在错误日志文件。
- 添加`CPU核心范围`检测。
> 2.5
- 修复`Aloazny.sh`对配置检查报错的bug。
> 2.4
- 添加对`CuDaemon`和`uperf`调度的运行检测。
- 修复对**亮屏逻辑的一个检测**。
> 2.3
- 尝试适配天机不存在`/sys/class/backlight`目录，改为30秒用`dumpsys`检测一次屏幕状态。
> 2.2
- 增加`/proc`缓存，避免反复读取`/proc`，减少部分性能开销。
- 添加`uthash.h`，用于读取配置包名，规则数量到`800+`时，差异明显，`100+`两者差不多。
- 添加MT文件管理器(`bin.mt.plus`)一条新的规则。
- 有了上面的优化，我把`PROC_CACHE_TIME`从`30`该到了`15`，兼容玩游戏的同学，原`SutoLiu`大佬的设定是`10`，但是运行起来CPU占用太高(2%左右)，现在优化好了改成`10`或者`15`运行都能在`0.8%`以下。
> 2.1
- 添加调整`applist.prop`部分配置。
- 替换`@coolapk 10007`大佬的`update-binary`(感谢！)，用于兼容**20400**低版本的Magisk。
> 2.0
- arm64的架构改成用`aarch64-linux-android32-clang`(Android12)编译了。
- 修正一个Apatch错误。
> 1.9
- 还是换回`O3`了，`-O2`省的那点内存还不如不省。
- 添加几个**哔哩哔哩**的附加进程配置参考。
- 修正`Aloazny.sh`在刷入的时候把自己给扬了。
> 1.8
- 配置文件监控改成`inotify`监控，理论上能降低一些Cpu占用和资源消耗。
- 编译时从`-O3`改为`-O2`，理论上内存占用没那么高了。
> 1.7
- **Scene** versioncode大于`820250402`不再检测Scene冲突。
> 1.6
- 新增对**配置文件**存在重复配置进程的提醒。
> 1.5
- 添加抖音(`com.ss.android.ugc.aweme`)配置参考。
- 添加椒盐笔记(`com.moriafly.note`)。
- 更新`Aloazny.sh`脚本(`action.sh`)，模块`action.sh`也能在原模块(@SutoLiu)中使用。
- 修正`service.sh`中把`/data/adb/modules/AppOpt`打错成`/data/adb/module/AppOpt`。
- 修改架构安装。
> 1.4
- 修复非`magisk`，**Ksu/Apatch** 刷入action报错问题。
> 1.3
- 在配置文件中未使用`=`的进程或者中文会被加上`#`注释。
- 酷安加上一个`com.coolapk.market{OkHttp Dispatch}=4-6`的配置，你也可以改成`com.coolapk.market{OkHttp*}=4-6`。
> 1.2
- 修复一个action检测`AppOpt`占用为0%和未运行的情况未区分，导致显示不准确的bug。
> 1.1
- 添加一个命令参数`-f 配置文件路径` `--log=日志文件路径` `--debug=填写true或者false`，如果不指定，那么就是模块默认配置。
- 英文说明`Usage: %s [-f config_file] [--log=log_file] [--debug=(true|false)]`
- 例如下面这个命令就是，指定配置文件`/data/adb/modules/AppOpt_Aloazny/applist.prop`，日志文件`/data/adb/modules/AppOpt_Aloazny/affinity_manager.log`，`关闭调试日志`。
```shell
AppOpt -f /data/adb/modules/AppOpt_Aloazny/applist.prop --log=/data/adb/modules/AppOpt_Aloazny/affinity_manager.log --debug=false
```
> 1.0
- 添加一个8+的`surfaceflinger`和`vendor.qti.hardware.display.composer-service`配置服务。
> 0.9
- 核心信息添加`sort -rn`排序后显示。
> 0.8
- 增加支付宝(`com.eg.android.AlipayGphone`) 番茄小说(`com.dragon.read`) iceraven(`io.github.forkmaintainers.iceraven`)配置。
- 修复`action.sh`识别冲突模块的一个逻辑错误。
> 0.7
- 修改`service.sh`语法，避免有的设备无法识别？
- 修复开机不自动执行。
> 0.6
- 把核心信息写入配置文件`applist.prop`。
> 0.5
- copy了@coolapk 10007代码，添加magisk或者非ksu识别。
> 0.4
- 添加`action.sh`按钮，能够输出模块的一些信息。
> 0.3
- 在配置文件`applist.prop`中修改`Debug_AppOpt=false`能提升一些性能。
- 因为我不打游戏，所以如果发现有的线程生效不及时的话，可以选择自己编译一下源码，把`PROC_CACHE_TIME 30`里面的`30`改回`10`或者`15`，提升响应速度。
- 添加更多冲突模块检测。
> 0.2
- 加回`dumpsys`作为备选，获取屏幕状态。
- 添加`scene`核心分配名单检测(目前无法检测开关，因为我太菜)和其他核心模块检测。
> 0.1
- 更改`module_id`为`AppOpt_Aloazny`。
- 更改配置文件`applist.conf`为`applist.prop`，**舒服我自己**用MT管理器查看配置文件有高亮。
- 更改`uninstall`的`setprop`为`resetprop`，避免卸载模块可能发生的开机阻塞。
- 添加一个shell脚本更正部分用户乱写的`applist.conf(applist.prop)`。
- 添加`日志记录`，去掉`dumpsys`获取屏幕状态。