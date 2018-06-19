# Linux 系统编程

本文为博主学习 Linux 系统编程的笔记，适合有一定的 shell 基础、且能够编译简单的 C 程序的人员食用。

## 本文结构

* [概述](#overview)
* [文件 I/O](#file_io)

<h2 id="overview">概述</h2>

### Linux 系统编程三大基石

* **系统调用**：为了从操作系统请求一些服务或资源，从用户空间如文本编辑器、游戏等向内核发起的函数调用。
* **C 库**：C 库由 GNU lib 提供，简称 libc，其除了标准 C 库之外，还提供了系统调用封装、线程支持和基本应用工具。
* **C 编译器**：标准 C 编译器由 GNU 编译器工具集 gcc 提供。

### API 与 ABI

* **API**：Application Programming Interface，应用程序编程接口，其**定义了软件模块之间在源代码层交互的接口**。
* **ABI**：Application Binary Interface，应用程序二进制接口，其**定义了两个软件模块在特定体系结构上的二进制接口**，它由内核和工具链来定义和实现。

### UNIX 两大标准

* **POSIX**：Portable Operating System Interface，可移植操作系统接口。
* **SUS**：Single UNIX Specification，单一 UNIX 规范，它是 POSIX 的扩展。

**LSB(Linux Standard Base)扩展了 POSIX 和 SUS，大多数 Linux 厂商在一定程度上都遵循了 LSB 标准**。

<h2 id="file_io">文件 I/O</h2>