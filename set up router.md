# 如何设置光纤路由器
---
:bowtie:
---
>作者：彭侦  
>链接：<https://github.com/Chorior/groceries/blob/master/set%20up%20router.md>

* 安装人员会帮你配好光纤的盒子,设置好账号,然后从盒子连出一条线接上电脑就能上网了,但因为只有一根,所以如果不另外使用路由器扩展的话,就会出现供不应求的现象;
* 扩展路由器使用主线或上个路由器的LAN口出来的网线作为WAN口,然后就会有很多子LAN口可以连接上网了。
* 下面说一下如何设置扩展路由器
  * 首先,你需要确保主线或者上个路由器的LAN口可以上网,检测方法如下
    * 如果是主线的话,直接连接,等待1~3分钟,测试是否能够上网,如果不能,联系中国电信;
    * 如果是上个路由器的LAN口
      * 如果上个路由器启用了DHCP,那么直接把LAN口出来的网线插在电脑上,等待1~3分钟,然后随便打开一个网址,如果该LAN口可用的话,就能访问该网址;否则该LAN口不能使用,请更换为另一个LAN口或者检查上个路由器的WAN口网线是否可以上网;
      * 如果上个路由器没有使用DHCP,那么手动将地址改为任何可用的ip地址,检测是否能够访问网页,否则更改LAN口;
  * 接着,你需要确保扩展路由器是可用的,并将其reset,即设置为出厂设置状态;
  * __然后,将扩展路由器插上电;__
  * 将上个路由器的LAN口网线插在扩展路由器的WAN口上;
  * 将一根短的网线一端插入扩展路由器的任意一个可用的LAN口,另一端连上一台电脑,等待1~3分钟,查看扩展路由器背面的说明,上面应该有进入设置路由器的地址,用户名和密码;如果没有,一般是192.168.0.1,用户名和密码都是admin，有些路由器密码可能为空;
  * 打开任意浏览器,输入上面找到的设置路由器的地址,按回车后输入用户名和密码,然后回车,进入路由器管理界面;
  * 找到网络设置,查看WAN口设置,设置为动态获取或者任何第一步测试过可用的静态地址;
  * 找到LAN口设置,这个是真正连接到下下一个路由器或用户的ip地址的设置,这个地址不要和WAN口的地址冲突,就是第三个数字不能与WAN口的一样,例如WAN口的地址是192.168.2.109,那么LAN口不能设置为192.168.2.1,你可以设成192.168.0.1或者192.168.3.1,只要不冲突都可以;
  * 修改好LAN口设置之后,路由器可能会要求重启,等待重启之后,输入上面设置的LAN口地址,再次进入路由器管理界面;
  * 找到DHCP设置,设置为启用;
  * 找到工具选项,点击重启路由器,等待2~3分钟;
  * 现在测试电脑是否可以连接上网
    * 如果可以,那么大功告成,你可以再次进入路由器管理界面,设置是否打开无线和无线密码,或者设置进入管理界面的密码;
    * 如果不行,重复上面所有的步骤再做一次;
  * 拔掉设置路由的网线,并将用户的网线插入各个LAN口,收工。
