### 蓝奏云下载地址(密码:111)
- [点击转跳](https://aloazny.lanzouo.com/b00je9nu1i)
- [1.3.5版本](https://aloazny.lanzouo.com/b00jeipeeb)
- [实时模式](https://aloazny.lanzouo.com/b00jeku6cd)
- [Ebpf版本(测试)](https://aloazny.lanzouv.com/b00jf0lz0h)
- [小飞机盘](https://share.feijipan.com/s/AN2lFUeN)

### 注意
- [Github地址](https://github.com/Aloazny/AppOpt_Aloazny)
- [查看适配应用列表](https://aloazny.github.io/AppOpt_Aloazny/#%E9%80%82%E9%85%8D%E5%88%97%E8%A1%A8)
- [查看Flags文件说明](https://aloazny.github.io/AppOpt_Aloazny/#%E6%8F%90%E7%A4%BA)

### 更新日志
#### **31.3**
- 更新**燕云十六声**线程。
- 首选机型改为`getprop ro.product.marketname`，备选获取机型改为`getprop | sed -n '/\.marketname\]/{s/.*: \[\(.*\)\]/\1/;/./{p;q}}'`适配更多变体marketname。
#### **31.1**
- 修正`cpu_control.sh`一个`disable_some_service`函数的bug。