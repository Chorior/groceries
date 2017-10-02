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
*   [构建一个 C++ 工程](#build_a_c++_project)

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

**所有的 Bazel 构建都发生在工作区--一个文件夹，该文件夹包含了你想编译的源文件、并且其顶层目录拥有一个名为 WORKSPACE 的文件**。这个 WORKSPACE 文件可以为空，也可以用来引用编译所需要的外部依赖库。

**工作区只是一个声明般的存在，真正创建编译规则的是 BUILD 文件**。

构建完成后的输出文件通常不在工作区，但**会在 BUILD 文件所在目录生成一个符号链接目录，该目录指向输出文件所在的目录**。

<h2 id="build_a_c++_project">构建一个 C++ 工程</h2>



