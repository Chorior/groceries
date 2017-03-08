---
title:      "C++ 释疑（二）"
subtitle:   " IO库、容器类及一些泛型算法 "
date:       2017-03-08 20:00:00 +0800
header-img: "img/sunset_beach.jpg"
tags:
    - C++
    - C++ primer
---

本文是我重温《C++ Primer 5》第二部分 C++ 标准库时，摘录的一些要点，适合初级程序员食用。

#   本文结构

*   [IO 库](#io_library)

<h2 id="io_library">IO 库</h2>

<h3 id="io_type_and_header">IO 库类型和头文件</h3>

头文件 | 类型
------ | ------
iostream | istream, wistream 从流读取数据<br> ostream, wostream 向流写入数据<br> iostream, wiostream 读写流
fstream | ifstream, wifstream 从文件读取数据<br> ofstream, wofstream 向文件写入数据<br> fstream, wfstream 读写文件
sstream | istringstream, wistringstream 从 string 读取数据<br> ostringstream, wostringstream 向 string 写入数据<br> stringstream, wstringstream 读写 string

类型`ifstream`和`istringstream`都继承自`istream`。

类型`ofstream`和`ostringstream`都继承自`ostream`。

所以我们可以像使用`cin`和`cout`一样使用文件流和string流对象。

### IO 对象无拷贝或赋值

不能将形参或返回类型设置为流类型。

进行 IO 操作的函数通常以引用方式进行传递和返回流。

读写一个 IO 对象会改变其状态，因此传递和返回的引用都不能时const的。

### IO 库条件状态

条件状态 | 说明
------ | ------
strm::iostate | strm 是一种 [IO 类型](#io_type_and_header) <br> iostate是一种机器相关的类型，提供了表达条件状态的完整功能
strm::badbit | 不可恢复的读写错误，流已崩溃
strm::failbit | 可恢复错误，一个 IO 操作失败了
strm::eofbit | 流到达了文件结尾
strm::goodbit | 流未处于错误状态，值为0
s.eof() | 若流s的eofbit置位，则返回true
s.fail() | 若流s的failbit或badbit置位，则返回true
s.bad() | 若流s的badbit置位，则返回true
s.good() | 若流s处于有效状态（所有错误位均未置位），则返回true
s.clear() | void函数<br> 将流s的所有条件状态位复位<br> 将流的状态设置为有效
s.clear(flags) | void函数<br>flags的类型为`strm::iostate`<br> 根据给定的flags标志位，将流s中对应条件状态位复位
s.setstate(flags) | void函数<br>flags的类型为`strm::iostate`<br> 根据给定的flags标志位，将流s中对应条件状态位置位
s.rdstate() | 返回流s的当前条件状态<br> 返回值类型为`strm::iostate`

如果达到文件结束位置，`eofbit`和`failbit`都会被置位。

一个流一旦发生错误，那么后续的所有 IO 操作都会失败。因此在使用一个流之前必须检查它是否处于良好状态。

确定一个流对象状态的最简单方法是将它当做一个条件来使用：

```c++
while(cin >> word)
{
    // do something
    // badbit,failbit,eofbit任一个被置位，则检测流状态的条件会失败
}
```

**将流当做条件使用不等于使用`!fail()`**，因为`fail()`只检查`failbit`位和`badbit`位，而流状态会检查三个错误位。因此，将流当做条件使用与使用`good()`是等效的。

```c++
...
static constexpr _Iostate goodbit = (_Iostate)0x0;
static constexpr _Iostate eofbit = (_Iostate)0x1;
static constexpr _Iostate failbit = (_Iostate)0x2;
static constexpr _Iostate badbit = (_Iostate)0x4;
...
bool __CLR_OR_THIS_CALL good() const
    {	// test if no state bits are set
    return (rdstate() == goodbit);
    }

bool __CLR_OR_THIS_CALL eof() const
    {	// test if eofbit is set in stream state
    return ((int)rdstate() & (int)eofbit);
    }

bool __CLR_OR_THIS_CALL fail() const
    {	// test if badbit or failbit is set in stream state
    return (((int)rdstate()
        & ((int)badbit | (int)failbit)) != 0);
    }

bool __CLR_OR_THIS_CALL bad() const
    {	// test if badbit is set in stream state
    return (((int)rdstate() & (int)badbit) != 0);
    }
```
