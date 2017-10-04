---
title:      "通用编译工具 Bazel"
subtitle:   "Hello Bazel"
date:       2017-10-01 14:00:00 +0800
header-img: "img/stock-photo-8.jpg"
tags:
    - C++
    - Tool
---

最近 Google 开源了它的 C++ 常用库 [Abseil](https://abseil.io/docs/cpp/quickstart)，鉴于它的文档内容都是基于 [Bazel](https://docs.bazel.build/versions/master/bazel-overview.html) 编译的，所以需要掌握一些常用的 Bazel 知识。

#   本文结构

*   [Bazel 概述](#overview)
*   [Bazel 安装](#bazel_install)
*   [Bazel 配置](#bazel_config)
*   [构建 Hello Wrold](#build_hello_world)
*   [Bazel 概念及术语](#bazel_concepts_and_terminology)

<h2 id="overview">Bazel 概述</h2>

Bazel 是一个编译工具，用于构建和运行测试。通过其扩展语言，你可以编译**任何语言**写的源文件，并**提供对 java,c,c++,python 的本地支持**。

**Bazel 支持大多数主流的运行环境(Linux,Windows,MacOs)和编译器**，这意味着**只要你学会了 Bazel ，就能在大多数系统下编译和测试任意语言了**，是不是很酷？:smirk:

Bazel 是没有 GUI 的，这意味着你需要知道如何为自己的工程添加编译规则(rules)。如果你用过 makefile、CMakeLists.txt 或者 QT 的 .pro 文件的话，那么 Bazel 添加编译规则的方式跟它们差不多，**Bazel 使用一个名为 BUILD 的文件来描述如何编译你的工程**。

Bazel 还有一个方便的查询语言([Query Lauguage](https://docs.bazel.build/versions/master/query.html))，通过与 [Graphviz](http://www.webgraphviz.com/) 配合使用，**可以立即构建出准确的关系依赖图**，这在管理工程时会节约不少时间。

与 cmake 一样，**Bazel 只会编译修改后的文件**。

<h2 id="bazel_install">Bazel 安装</h2>

bazel 在 Windows 下的安装非常简单，只要下载一个 Windows 安装包管理器 [Chocolatey](https://chocolatey.org/)，安装完成后以管理员身份在 cmd.exe 下执行下面这句话，然后一直按y回车就行了：

```cmd
$ choco install bazel
```

其它系统下的安装，由于没有实际操作过，可以移步 [Installing Bazel](https://docs.bazel.build/versions/master/install.html)。

<h2 id="bazel_config">Bazel 配置</h2>

**所有的 Bazel 构建都发生在工作区--一个文件夹，该文件夹包含了你想编译的源文件、并且其顶层目录拥有一个名为 WORKSPACE 的文件**，这个 WORKSPACE 文件可以为空，也可以用来引用编译所需要的外部依赖库。

**工作区只是一个声明般的存在，真正创建编译规则的是 BUILD 文件，工作区内包含一个 BUILD 文件的文件夹被称为一个包(package)**。

**不同工作区的文件默认是相互独立的**。

构建完成后的输出文件通常不在工作区，但**会在 WORKSPACE 文件所在目录生成多个符号链接目录，这些目录指向对应输出文件所在的目录**，具体可查看下面的 [Hello World](#build_hello_world)。

<h2 id="build_hello_world">构建 Hello Wrold</h2>

为了尽快入个门，我们首先构建一个 Hello World 练练手。

首先，我们构造一个这样的目录结构：

```text
bazel
├── hello_world
│   ├── BUILD
│   ├── main.cpp
│   ├── hello_world.h
│   └── hello_world.cpp
└── WORKSPACE
```

然后开始我们最喜欢的 Hello World：

```c++
// hello_world.h
#pragma once

void print_hello_world();

void print_time();
```

```c++
// hello_world.cpp
#include <ctime>
#include <string>
#include <iostream>
#include "hello_world.h"

void print_hello_world() {
	std::cout << "Hello World" << std::endl;
}

void print_time() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);

	std::cout << now->tm_year + 1900 << "-"
		<< now->tm_mon + 1 << "-"
		<< now->tm_mday << " "
		<< now->tm_hour << ":"
		<< now->tm_min << ":"
		<< now->tm_sec
		<< std::endl;
}
```

```c++
// main.cpp
#include "hello_world.h"

int main()
{
    print_hello_world();
    print_time();
}
```

接着是 BUILD 文件：

```bazel
cc_library(                     # 构建库
    name = "hello_world",       # 库名
    srcs = ["hello_world.cpp"], # 源文件
    hdrs = ["hello_world.h"],   # 头文件
)

cc_binary(                      # 构建二进制文件
    name = "hello",             # 目标名
    srcs = ["main.cpp"],        # 源文件
    deps = [                    # 依赖项
        ":hello_world",         # 当前目录下的 hello_world 库
    ],
)
```

最后一步，编译(git command)：

```bash
pengzhen@pengzhen-PC MINGW64 /r/bazel
$ bazel build ///hello_world:hello
.............
____Loading package: hello_world
____Loading package: @bazel_tools//tools/cpp
____Loading package: @local_config_xcode//
____Loading package: @local_jdk//
____Loading package: @local_config_cc//
____Loading complete.  Analyzing...
____Found 1 target...
____Building...
____[0 / 6] BazelWorkspaceStatusAction stable-status.txt
Target //hello_world:hello up-to-date:
  C:/users/pengzhen/appdata/local/temp/_bazel_pengzhen/uhncvrpq/execroot/__main__/bazel-out/msvc_x64-fastbuild/bin/hello_world/hello.exe
____Elapsed time: 5.281s, Critical Path: 0.78s
```

根据打印的 log 可以看到，生成的 EXE 文件在 temp 目录下，这意味着当你重启电脑后，这些文件将不复存在。

现在我们来看看工作区发生了哪些变化：

```bash
pengzhen@pengzhen-PC MINGW64 /r/bazel
$ ll -R
.:
total 4
lrwxrwxrwx 1 pengzhen 197121 47 十月  3 11:14 bazel-bazel -> /tmp/_bazel_pengzhen/uhncvrpq/execroot/__main__/
lrwxrwxrwx 1 pengzhen 197121 80 十月  3 11:14 bazel-bin -> /tmp/_bazel_pengzhen/uhncvrpq/execroot/__main__/bazel-out/msvc_x64-fastbuild/bin/
lrwxrwxrwx 1 pengzhen 197121 85 十月  3 11:14 bazel-genfiles -> /tmp/_bazel_pengzhen/uhncvrpq/execroot/__main__/bazel-out/msvc_x64-fastbuild/genfiles/
lrwxrwxrwx 1 pengzhen 197121 57 十月  3 11:14 bazel-out -> /tmp/_bazel_pengzhen/uhncvrpq/execroot/__main__/bazel-out/
lrwxrwxrwx 1 pengzhen 197121 85 十月  3 11:14 bazel-testlogs -> /tmp/_bazel_pengzhen/uhncvrpq/execroot/__main__/bazel-out/msvc_x64-fastbuild/testlogs/
drwxr-xr-x 1 pengzhen 197121  0 十月  3 10:41 hello_world/
-rw-r--r-- 1 pengzhen 197121  0 十月  3 10:40 WORKSPACE

./hello_world:
total 4
-rw-r--r-- 1 pengzhen 197121 453 十月  3 11:06 BUILD
-rw-r--r-- 1 pengzhen 197121 444 十月  3 10:57 hello_world.cpp
-rw-r--r-- 1 pengzhen 197121  63 十月  3 10:57 hello_world.h
-rw-r--r-- 1 pengzhen 197121  89 十月  3 10:58 main.cpp
```

可以看到，hello_world 目录并没有发生任何变化，但是工作区根目录却多了5个链接目录。如果你仔细看的话，bazel-bazel,bazel-out,bazel-bin 指向同一个目录的不同层级，然后根据 bazel-genfiles,bazel-testlogs 的目录名称猜想它们应该分别是生成文件和测试log文件夹。

我们先执行一下生成的可执行文件：

```bash
pengzhen@pengzhen-PC MINGW64 /r/bazel
$ ll bazel-bin/hello_world/
total 6155
drwxr-xr-x 1 pengzhen 197121       0 十月  3 11:06 _objs/
-r-xr-xr-x 1 pengzhen 197121  787968 十月  3 11:06 hello.exe*
drwxr-xr-x 1 pengzhen 197121       0 十月  3 11:06 hello.exe.runfiles/
-r--r--r-- 1 pengzhen 197121     166 十月  3 11:06 hello.exe.runfiles_manifest
-r--r--r-- 1 pengzhen 197121     138 十月  3 11:06 hello.exe-2.params
-r--r--r-- 1 pengzhen 197121 4722688 十月  3 11:06 hello.pdb
-r--r--r-- 1 pengzhen 197121  783128 十月  3 11:06 libhello_world.a
-r--r--r-- 1 pengzhen 197121     164 十月  3 11:06 libhello_world.a-2.params

pengzhen@pengzhen-PC MINGW64 /r/bazel
$ bazel-bin/hello_world/hello.exe
Hello World
2017-10-3 11:28:15
```

结果如我们所愿，然后我们使用查询语言查看当前工程的依赖关系：

```bash
pengzhen@pengzhen-PC MINGW64 /r/bazel
$ bazel query --nohost_deps --noimplicit_deps 'deps(//hello_world:hello)' --output graph
digraph mygraph {
  node [shape=box];
"//hello_world:hello"
"//hello_world:hello" -> "//hello_world:hello_world"
"//hello_world:hello" -> "//hello_world:main.cpp"
"//hello_world:main.cpp"
"//hello_world:hello_world"
"//hello_world:hello_world" -> "//hello_world:hello_world.cpp\n//hello_world:hello_world.h"
"//hello_world:hello_world.cpp\n//hello_world:hello_world.h"
}
```

把上面生成的打印文本复制到 [Graphviz](http://www.webgraphviz.com/)，即可看到生成的依赖关系图了：

![Graphviz](hello_world.png)

<h2 id="bazel_concepts_and_terminology">Bazel 概念及术语</h2>

### 工作区(WORKSPACE)

前面我们说过，工作区是一个文件夹，这个文件夹包含了你想编译的源文件，并且构建后的输出文件夹也会以符号链接(symbolic link)的形式生成在工作区的根目录下。

作为工作区的标志，就是**工作区根目录下必须包含一个名为 WORKSPACE 的文件**，这个 WORKSPACE 文件可以为空，也可以被用来引用外部依赖库。

一个简单的工作区目录结构如下所示：

```text
workspace
└── WORKSPACE
```

### 包(package)

**工作区的基本单元是一个包**。一个包也是一个文件夹，这个文件夹在工作区一定是工作区目录的子目录，该文件夹包含了一系列关联的源文件，这些源文件可以以子目录的方式存在。

作为包的标志以及如何编译这些源文件，**包目录必须包含一个名为 BUILD 的文件**，为了包间的独立性，如果一个包包含了另一个包，那么**子包的文件是不属于顶层包的**。

一个简单的实例如下：

```text
bazel
├── hello_world
│   ├── BUILD
│   ├── main.cpp
│   ├── lib
│   │   ├── hello_world.h
│   │   └── hello_world.cpp
│   └── test
│       ├── BUILD
│       ├── test.h
│       └── test.cpp
└── WORKSPACE
```

这里 bazel 目录是工作区，`hello_world` 目录是一个包，`hello_world/test` 也是一个包，所以 test 目录不属于 `hello_world` 包，但是 `hello_world/lib` 不是一个包，所以它属于 `hello_world` 包。

**虽然你可以将工作区根目录作为一个包，但是建议用一些描述性的目录名来创建一个包**。

### 目标(target)

**包里面的元素被称为 target**。

target 主要分为两类--文件(file)和规则(rule)，另一类不太常用的类型是 package group。

文件(file)包含两种类型--源文件与生成文件，其中生成文件是某些规则(rule)生成的文件。

规则(rule)指定了一系列输入文件与一系列输出文件间的关系、以及一步步从这些输入文件生成这些输出文件的方式。其中输入文件可以是源文件也可以是生成文件，但输出文件一定是生成文件，这意味着**你可以将一个规则的输出文件作为另一个规则的输入文件**。

一个规则可能包含另一个规则，**假设规则A包含规则B，那么在编译时B的头文件对于A是可用的、在链接时B的标志(symbol)对于A是可用的、在运行时B的运行时数据(runtime data)对于A是可用的**。

package group 将一系列包合并为一个组，这样就能引用这个组内所有包的可见标志了。

### 标签(label)

**目标(target)的名字就是一个标签**。

每个标签包含两个部分--包名和目标名，例如我们在 [Hello World](#build_hello_world) 里面一个标签就是：

```text
//hello_world:hello
```

**其中 `hello_world` 是包名(不包含双斜杠)**，双斜杠代表从工作区根目录开始，`hello` 是目标名。

**每个标签唯一标识一个目标**。**当你省略目标名时，目标名被认为与包名的最后一部分相同**，例如下面两个标签被认为是一样的：

```text
//hello/world
//hello/world:world
```

**在 BUILD 文件中，包名是可以省略的**，例如下面四个标签都是一样的：

```text
//hello/world
//hello/world:world
:world
world
```

但是引用其他包的文件的话就不能省略包名，例如对于下面的目录结构中的 `//hello_world:BUILD` 文件，如果要引用 `hello_world/test` 的 `test.h` 文件，就必须使用 `//hello_world/test:test.h` 而不能使用 `test:test.h`，不过所幸如果你使用了错误的方式，编译时会报出错误信息：

```text
bazel
├── hello_world
│   ├── BUILD
│   ├── main.cpp
│   ├── lib
│   │   ├── hello_world.h
│   │   └── hello_world.cpp
│   └── test
│       ├── BUILD
│       ├── test.h
│       └── test.cpp
└── WORKSPACE
```

#### 标签命名规范

一个完整的标签命名公式如下：

```text
//package_name:target_name
```

其中 `package_name` 就是相对于根目录的包含 BUILD 文件的路径名，如 `hello_world/test`；当根目录是一个包时，`package_name` 为空，如 `//:hello`。

**包名必须由字母、数字、`/-._` 组成，但是不能以斜杠 `/` 开头和结尾**，所以不要在工作区创建中文包。

`target_name` 分为文件(file)名和规则(rule)名，其中文件名是相对于包目录的路径名，如 `lib/hello_world.h`；规则名就是 BUILD 文件中某个规则的 name 参数的值，如我们 [Hello World](#build_hello_world) 中就有两个规则名--`hello_world` 和 `hello`。**规则名中最好不要使用斜杠 `/`，因为可能会造成误导**。

**目标名必须有字母、数字、`_/.+-=,@~` 组成，但是不能以斜杠 `/` 开头和结尾，也不能以多个连续的斜杠 `/` 作为路径分隔符，如 `hello_world//lib`**。另外，**两个特殊的目录 `..` 和 `.` 也不能使用**。


### 规则(rule)

规则(rule)指定了一系列输入文件与一系列输出文件间的关系、以及一步步从这些输入文件生成这些输出文件的方式。

例如 [Hello World](#build_hello_world) 中就有两个规则--`hello_world` 和 `hello`。

每个规则都有一系列属性，这些属性要么是整数、要么是字符串或字符串集、要么是输出标签或输出标签集。