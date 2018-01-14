# Boost C++

经常使用 C++ 的兄弟们应该知道，**C++ 标准库是不完整的**，例如最常用的文件夹读取是没有标准接口的。

Boost 弥补了标准库的缺点，这意味着学习 Boost 将使你的 C++ 更加全面，而且 Boost 是高效率的。

##   本文结构

*   [概述](#overview)
*   [安装](#install)
*   [Hello World](#hello_world)
*   [更多](#further)

<h2 id="overview">概述</h2>

Boost 是 C++ 标准库的一个扩展库，其许可证**允许商业用途与个人私用，并且不需要分享源代码，也不需要提供版权信息**。如果你分发(distribute，即移交版权)了自己写的、带有 Boost 的代码，那么 Boost 协议只应用于 Boost 部分，你自己的部分可以随意使用你想要的协议。详细的许可协议查看 [Boost Software License](http://www.boost.org/users/license.html)，如果需要开源的话，查看 [choosealicense](http://choosealicense.online/)。

Boost 支持几乎所有主流的操作系统，因此可以跨环境运行。

<h2 id="install">安装</h2>

Boost 的安装真的是超级简单，只要在[首页](http://www.boost.org/)下载对应压缩文件，然后解压即可。

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

好在大多数 Boost 库都是直接在其头文件中实现的，这意味着你只需要包含其头文件就能使用了；一些比较复杂的库，如 Thread、FileSystem，才需要编译成库文件进行使用。具体查看 [header-only-libraries](http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html#header-only-libraries)。

为了方便使用所有的库，使用如下命令编译所有需要编译的 Boost 库：

```bash
$ cd boost_1_65_1
$ bootstrap
$ .\b2
```

上面的命令使用默认编译器进行编译，例如如果你电脑上安装了 VS2015 和 VS2017，那么很可能使用 VS2017 的编译器进行编译。

如果你想自定义选项的话，参考 [Invocation](http://www.boost.org/build/doc/html/bbv2/overview/invocation.html)。例如，如果你想使用 VS2015 编译 filesystem 的debug 版本的话：

```bash
$ b2 toolset=msvc-14.0 variant=debug filesystem
```

全编译后的库文件在 `boost_1_65_1\stage\lib` 目录下，摘录 filesystem 的库文件名如下：

```text
libboost_filesystem-vc141-mt-1_65_1.lib
libboost_filesystem-vc141-mt-gd-1_65_1.lib
```

部分编译的库文件在 `boost_1_65_1\bin.v2\libs` 相应目录下，如上面用 VS2015 编译 debug 版本的 filesystem 如下：

```bash
$ pwd
/e/boost_1_65_1/bin.v2/libs/filesystem/build/msvc-14.0/debug/threading-multi

$ ls
boost_filesystem-vc140-mt-gd-1_65_1.dll*
boost_filesystem-vc140-mt-gd-1_65_1.dll.manifest
boost_filesystem-vc140-mt-gd-1_65_1.dll.rsp
boost_filesystem-vc140-mt-gd-1_65_1.exp
boost_filesystem-vc140-mt-gd-1_65_1.lib
boost_filesystem-vc140-mt-gd-1_65_1.pdb
...
```

可以看到，编译后的库文件名遵循一定的规则，查看 [Library Naming](http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html#library-naming)，我们知道库文件名的一般形式如下：

```text
libboost_regex-vc71-mt-d-1_34.lib
```

*	`lib`: **在 Windows 下只有静态库才会以 lib 为前缀**，其它系统下所有库都会以 lib 为前缀；
*	`boost_regex`: 模块名，如 `boost_system`, `boost_thread`；
*	`-vc71`: 编译器及其版本号；
*	`-mt`: 支持多线程；
*	`-d`: 根据编译属性的不同，如 `-gd` 表示构建代码的 debug 版本，并且使用标准库和运行库的 debug 版本；
*	`-1_34`：boost 版本号；
*	`.lib`：后缀名。

<h2 id="hello_world">Hello World</h2>

这里的 Hello World 并不是真正的 Hello World，它只是我们入门某个领域的代名词。

我们首先使用 boost 中最靠前的一个搜索算法 [boyer_moore_search](http://www.boost.org/doc/libs/1_65_1/libs/algorithm/doc/html/algorithm/Searching.html#the_boost_algorithm_library.Searching.BoyerMoore) 来开始我们进入 boost 世界的第一步：

*	在 VS2015 中新建空项目 `boost_test`；
*	更改模式为 debug/x86；
*	添加包含目录为 `E:\boost_1_65_1`，由于该算法不需要编译成库文件，所以不需要添加库目录和链接器输入；
*	添加源文件 main.cpp:

```c++
#include <string>
#include <iostream>
#include <boost\algorithm\searching\boyer_moore.hpp>

int main()
{
	std::string corpora("hello world");
	std::string pattern("wo");

	boost::algorithm::boyer_moore<std::string::const_iterator> bm(pattern.begin(), pattern.end());
	auto ret = bm(corpora.begin(), corpora.end());
	//auto ret = boost::algorithm::boyer_moore_search(corpora.begin(), corpora.end(), pattern.begin(), pattern.end());
	if (ret.first != corpora.end()) {
		std::cout << std::string(ret.first, ret.second);
	}
}
```

运行：

```bash
$ boost_test.exe
wo
```

由于是直接使用头文件，上面的 Hello World 一定能够运行。为了检验编译的库文件是否可以使用，我们使用 filesystem 来进行测试：

*	继续添加库目录 `E:\boost_1_65_1\stage\lib`；
*	添加链接器输入 `libboost_filesystem-vc140-mt-gd-1_65_1.lib`；
*	修改 main.cpp:

```c++
#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>
using std::cout;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Usage: boost_test path\n";
		return 1;
	}

	path p(argv[1]);

	try
	{
		if (exists(p))
		{
			if (is_regular_file(p))
				cout << p << " size is " << file_size(p) << '\n';

			else if (is_directory(p))
			{
				cout << p << " is a directory containing:\n";

				std::vector<std::string> v;

				for (auto&& x : directory_iterator(p))
					v.push_back(x.path().filename().string());

				std::sort(v.begin(), v.end());

				for (auto&& x : v)
					cout << "    " << x << '\n';
			}
			else
				cout << p << " exists, but is not a regular file or directory\n";
		}
		else
			cout << p << " does not exist\n";
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << '\n';
	}

	return 0;
}
```

运行：

```bash
$ boost_test.exe boost_test.pdb
"boost_test.pdb" size is 1486848

$ boost_test.exe .
"." is a directory containing:
    boost_test.exe
    boost_test.ilk
    boost_test.pdb

$ boost_test.exe hello
"hello" does not exist
```

更多相关知识，查看 [Filesystem](http://www.boost.org/doc/libs/1_65_1/libs/filesystem/doc/tutorial.html)。

<h2 id="further">更多</h2>

boost 是一个非常全面的库，你可以在 [boost index](http://www.boost.org/doc/libs/1_65_1/libs/libraries.htm) 中找到相应库的文档、结合 libs 目录下的 example 进行学习。
