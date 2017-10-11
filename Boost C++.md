# Boost C++

经常使用 C++ 的兄弟们应该知道，**C++ 标准库是不完整的**，例如最常用的文件夹读取是没有标准接口的。Boost 弥补了标准库的缺点，这意味着学习 Boost 将使你的 C++ 更加全面，而且 Boost 还有其它的优点。

##   本文结构

*   [概述](#overview)
*   [安装](#install)

<h2 id="overview">概述</h2>

Boost 是 C++ 标准库的一个扩展库，其许可证**允许商业用途与个人私用，并且不需要分享源代码，也不需要提供版权信息**。如果你开源了自己写的、带有 Boost 的代码，那么 Boost 协议只应用于 Boost 部分，你自己的部分可以随意使用你想要的开源协议。详细的许可协议查看 [Boost Software License](http://www.boost.org/users/license.html)。

Boost 支持几乎所有主流的操作系统，因此可以跨环境运行。

<h2 id="install">安装</h2>

Boost 的安装真的是超级简单，只要在[首页](http://www.boost.org/)下载压缩文件，然后解压即可。

解压后的目录结构如下：

```text
boost_1_65_1
├── boost/        # 所有的 Boost 头文件
├── doc/          
├── libs/         # 测试、示例
├── more/
├── status/
├── tools/
└── index.html    # 说明文档
```

**如果你要使用 Boost 的话，就需要添加 `boost_1_65_1` 到工程的 include 路径中去**。

好在大多数 Boost 库都是直接在其头文件中实现的，这意味着你只需要包含其头文件就能使用了；一些比较复杂的库，如 Thread，才需要编译成库文件进行使用。具体查看 [header-only-libraries](http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html#header-only-libraries)。

为了方便使用所有的库，使用如下命令编译所有需要编译的 Boost 库：

```bash
$ bootstrap
$ .\b2
```

