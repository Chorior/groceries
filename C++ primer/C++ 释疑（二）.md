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
    *   [IO 库类型和头文件](#io_type_and_header)
    *   [IO 库条件状态](#io_condition_state)
    *   [IO 输出缓冲](#output_buffer)
    *   [文件输入输出](#file_io)
    *   [string 流](#string_io)

<h2 id="io_library">IO 库</h2>

<h3 id="io_type_and_header">IO 库类型和头文件</h3>

头文件 | 类型
---------| ---------------------------------
iostream | istream, wistream 从流读取数据<br>ostream, wostream 向流写入数据<br>iostream, wiostream 读写流
fstream | ifstream, wifstream 从文件读取数据<br>ofstream, wofstream 向文件写入数据<br>fstream, wfstream 读写文件
sstream | istringstream, wistringstream 从 string 读取数据<br>ostringstream, wostringstream 向 string 写入数据<br>stringstream, wstringstream 读写 string

类型`ifstream`和`istringstream`都继承自`istream`。

类型`ofstream`和`ostringstream`都继承自`ostream`。

所以我们可以像使用`cin`和`cout`一样使用文件流和string流对象。

### IO 对象无拷贝或赋值

不能将形参或返回类型设置为流类型。

进行 IO 操作的函数通常以引用方式进行传递和返回流。

读写一个 IO 对象会改变其状态，因此传递和返回的引用都不能时const的。

<h3 id="io_condition_state">IO 库条件状态</h3>

条件状态 | 说明
------------- | -----------------------------------------
strm::iostate | strm 是一种 [IO 类型](#io_type_and_header) <br>iostate是一种机器相关的类型，提供了表达条件状态的完整功能
strm::badbit | 不可恢复的读写错误，流已崩溃
strm::failbit | 可恢复错误，一个 IO 操作失败了
strm::eofbit | 流到达了文件结尾
strm::goodbit | 流未处于错误状态，值为0
s.eof() | 若流s的eofbit置位，则返回true
s.fail() | 若流s的failbit或badbit置位，则返回true
s.bad() | 若流s的badbit置位，则返回true
s.good() | 若流s处于有效状态（所有错误位均未置位），则返回true
s.clear() | void函数<br>将流s的所有条件状态位复位<br>将流的状态设置为有效
s.clear(flags) | void函数<br>flags的类型为`strm::iostate`<br>根据给定的flags标志位，将流s中对应条件状态位复位
s.setstate(flags) | void函数<br>flags的类型为`strm::iostate`<br>根据给定的flags标志位，将流s中对应条件状态位置位
s.rdstate() | 返回流s的当前条件状态<br>返回值类型为`strm::iostate`

如果达到文件结束位置，`eofbit`和`failbit`都会被置位。

**一个流一旦发生错误，那么后续的所有 IO 操作都会失败。因此在使用一个流之前必须检查它是否处于良好状态**。

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

<h3 id="output_buffer">IO 输出缓冲</h3>

每个输出流都管理一个缓冲区，用来保存程序读写的数据。例如，当你使用如下代码时：

```c++
os << "hahah";
```

文本串可能立即打印出来，也有可能被操作系统保存在缓冲区随后打印。

若程序异常终止，输出缓冲区是不会被刷新的。**在log调试的时候，一定要确保所有缓冲区的数据确实已经输出了**，不然你会浪费时间在一些你认为没执行但其实已经执行了的代码上面。

缓冲机制可以将程序的多个输出操作组合成单一的系统级写操作，这样可以提升性能。

#### 刷新输出缓冲区

```c++
std::cout << "hahah" << std::endl;  // 输出hahah加一个换行，然后刷新缓冲区
std::cout << "hahah" << std::ends;  // 输出hahah加一个空字符，然后刷新缓冲区
std::cout << "hahah" << std::flush; // 输出hahaa不附加任何字符，然后刷新缓冲区
```

#### unibuf 操纵符

```c++
std::cout << std::unitbuf;   // 所有输出立即刷新，无缓冲
std::cout << std::nounitbuf; // 回归正常输出状态，有缓冲
```

**`std::cerr`默认是设置`unitbuf`的，所以log调试时，你可以使用`std::cerr`进行输出**。当然你也可以手动设置一个流为`unitbuf`的。

#### 输出流关联到另一个流

若一个输出流被关联到另一个流，那么当读写另一个流时，该输出流会被刷新。

`std::cin`和`std::cerr`默认都关联到`std::cout`。

**每个流最多同时关联一个流，但输出流可以同时被关联多个流**。

关联函数`tie`有两个重载的版本：

* 不带参数的版本，返回关联的输出流的指针；若没有关联输出流，那么返回空指针。

```c++
std::ostream *p = std::cin.tie();
if(p) *p << "hahah\n";
```

* 带参数的版本，接受一个指向ostream的指针，将自己关联到该ostream，并返回旧的关联输出流。

```c++
std::ostream *old_tie = std::cin.tie(nullptr); // 解除关联
if (!std::cin.tie()) std::cout << "std::cin.tie() == nullptr\n";
std::cin.tie(old_tie);
if (std::cin.tie()) std::cout << "std::cin.tie() != nullptr\n";
```

<h3 id="file_io">文件输入输出</h3>

头文件`<fstream>`定义了三个类型

* `ifstream`: 读取给定文件；
* `ofstream`: 写入给定文件；
* `fstream` : 读写给定文件。

除了继承自`iostream`类型的行为外，`fstream`还定义了一些自己独有的成员：

fstream独有的操作 | 说明
----------------------- | -----------------------------------------
fstream fstrm; | 创建一个未绑定的文件流<br>**fstream可以是`ifstream`,`ofstream`或`fstream`**
fstream fstrm(s); | explicit构造函数<br>创建一个fstream，并打开一个名为 `s` 的文件<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>[查看默认打开模式](#file_mode)
fstream fstrm(s, mode); | 与前一个构造函数一样，但按指定mode打开文件
fstream fstrm(s); | explicit构造函数<br>创建一个fstream，并打开一个名为 `s` 的文件<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>[查看默认打开模式](#file_mode)
fstrm.open(s) | void函数<br>打开名为 `s` 的文件，并将文件与fstrm绑定<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>对一个打开的文件流使用open会导致failbit被置位<br>[](#file_mode)
fstrm.open(s, mode) | 与`fstrm.open(s)`一样，但按指定mode打开文件
fstrm.close() | void函数<br>关闭与fstrm绑定的文件<br>当一个fstream对象离开其作用域时，会自动调用`close()`
fstrm.is_open() | 若与fstrm关联的文件成功打开，且尚未关闭，则返回true

可以类比`iostream`来运用`fstream`。

<h4 id="file_mode">文件模式</h4>

文件模式用来指出如何使用文件。

**`ifstream`默认使用in模式；`ofstream`默认使用out模式，而out模式默认指定trunc模式；`fstream`默认使用in和out模式**。

文件模式 | 说明
------- | -------------------------
in | 以读方式打开<br>**只支持`ifstream`或`fstream`对象**
out | 以写方式打开<br>默认指定trunc模式<br>指定in或app模式会保留原有数据<br>**只支持`ofstream`或`fstream`对象**
app | 每次写操作前均定位到文件末尾<br>默认指定out模式<br>**只有当trunc模式没被设定时才能设定app模式**
ate | 打开文件后立即定位到文件末尾
trunc | 截断文件<br>**只有当out被设定时才可设定trunc模式**
binary | 以二进制方式进行 IO

```c++
std::ofstream out("file");
std::ofstream out1("file", std::ofstream::out);
std::ofstream out2("file", std::ofstream::out | std::ofstream::app);
```

<h3 id="string_io">string 流</h3>

头文件`<sstream>`也定义了三个类型

* `istringstream`: 读取给定string；
* `ostringstream`: 写入给定string；
* `stringstream` : 读写给定string。

除了继承自`iostream`类型的行为外，`sstream`还定义了一些自己独有的成员：

sstream独有的操作 | 说明
---------------- | -----------------------------------------
sstream strm; | 创建一个未绑定的string流<br>**sstream可以是`istringstream`,`ostringstream`或`stringstream`**
sstream strm(s); | explicit构造函数<br>创建一个sstream对象，并保存`std::string s`的一个拷贝。
strm.str() | 返回strm所保存的`std::string`的拷贝
strm.str(s) | void函数<br>将`std::string s`拷贝到strm中

**当函数得到整行文本，但却需要处理单个单词时，使用`std::istringstream`会很方便**；

**当需要逐步构造输出，最后一起打印时，使用`std::ostringstream`会比较方便**。
