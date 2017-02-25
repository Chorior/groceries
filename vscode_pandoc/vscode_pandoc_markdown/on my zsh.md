#oh my zsh
---
:ant:
---
* [oh my zsh](https://github.com/robbyrussell/oh-my-zsh)是github上一个著名的用于linux或OS x的开源框架,它可以使你的terminal显得非常酷炫而又实用,原因在于它有很多themes和plugins;
* 废话不说,开始在linux上安装
    * 首先安装zsh: `sudo apt-get install zsh`
    * 然后安装curl: `sudo apt-get install curl`
    * 执行命令`sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"`(可能需要翻墙)
    * 对,这就安装好了,你不要打我
* 最重要的部分来了
    * 你可以在[plugins](https://github.com/robbyrussell/oh-my-zsh/wiki/Plugins)里选择你想要的插件;
    * 还可以在[themes](https://github.com/robbyrussell/oh-my-zsh/wiki/Themes)里选择你想要的主题(有预览哦);
        * 如果你想每次打开terminal时都自动运行zsh,在~/.bashrc最后添加一行: `env zsh`,你也可以手动执行;
        * 如果你想每次运行zsh时都出现不一样的主题,在~/.zshrc中添加`ZSH_THEME="random"`;
    * 如果你想自动更新,在~/.zshrc中添加`DISABLE_UPDATE_PROMPT=true"`;
    * 更多信息see<https://github.com/robbyrussell/oh-my-zsh>
* 卸载oh my zsh: 命令行执行`uninstall_oh_my_zsh`,这会卸载on my zsh并恢复原来的配置;        