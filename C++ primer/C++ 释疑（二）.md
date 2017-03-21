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
*   [顺序容器](#sequential_containers)
    *   [顺序容器选择](#choose_sequential_container)
    *   [容器基本操作](#basic_container_operation)
    *   [顺序容器操作](#sequential_container_operation)
*   [容器泛型算法](#generic_algorithms)
    *   [lambda 表达式](#lambda_expression)

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
`fstream fstrm` | 创建一个未绑定的文件流<br>**fstream可以是`ifstream`,`ofstream`或`fstream`**
`fstream fstrm(s)` | explicit构造函数<br>创建一个fstream，并打开一个名为 `s` 的文件<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>[查看默认打开模式](#file_mode)
`fstream fstrm(s, mode)` | 与前一个构造函数一样，但按指定mode打开文件
`fstream fstrm(s)` | explicit构造函数<br>创建一个fstream，并打开一个名为 `s` 的文件<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>[查看默认打开模式](#file_mode)
`fstrm.open(s)` | void函数<br>打开名为 `s` 的文件，并将文件与fstrm绑定<br>`s` 是`std::string`类型或一个指向C风格字符串的指针<br>对一个打开的文件流使用open会导致failbit被置位<br>[](#file_mode)
`fstrm.open(s, mode)` | 与`fstrm.open(s)`一样，但按指定mode打开文件
`fstrm.close()` | void函数<br>关闭与fstrm绑定的文件<br>当一个fstream对象离开其作用域时，会自动调用`close()`
`fstrm.is_open()` | 若与fstrm关联的文件成功打开，且尚未关闭，则返回true

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
`sstream strm` | 创建一个未绑定的string流<br>**sstream可以是`istringstream`,`ostringstream`或`stringstream`**
`sstream strm(s)` | explicit构造函数<br>创建一个sstream对象，并保存`std::string s`的一个拷贝。
`strm.str()` | 返回strm所保存的`std::string`的拷贝
`strm.str(s)` | void函数<br>将`std::string s`拷贝到strm中

**当函数得到整行文本，但却需要处理单个单词时，使用`std::istringstream`会很方便**；

**当需要逐步构造输出，最后一起打印时，使用`std::ostringstream`会比较方便**。

<h2 id="sequential_containers">顺序容器</h2>

标准库容器分为顺序容器与关联容器。

顺序容器中元素的顺序由其加入容器时的位置决定；关联容器中元素的顺序由其相关联的关键字值决定。

<h3 id="sequential_containers_overview">顺序容器概述</h3>

顺序容器类型 | 说明
----------- | -----------------------------------------
vector | 可变大小数组<br>元素保存在连续的内存空间中<br>支持快速随机访问<br>在尾部之外的位置插入或删除元素很慢
deque | 双端队列<br>支持快速随机访问<br>在头尾插入、删除元素很快
list | 双向链表<br>只支持双向顺序访问<br>在任何位置插入、删除都很快<br>额外内存较其他顺序容器开销很大
forward_list | 单向链表<br>只支持单向顺序访问，没有size操作<br>在任何位置插入、删除都很快<br>额外内存较其他顺序容器开销很大
array | 固定大小数组<br>支持快速随机访问<br>不能添加或删除元素
string | 与vector相似<br>专门用于保存字符<br>元素保存在连续的内存空间中<br>支持快速随机访问<br>在尾部插入、删除元素很快

**现代 C++ 程序推荐使用标准库容器，而不是更原始的数据结构，如内置数组**。

<h3 id="choose_sequential_container">顺序容器选择</h3>

**通常，使用vector是最好的选择**，除非你有很好的理由选择其他容器。

**选择容器的基本原则**：

*   除非你有很好的理由选择其他容器，否则应该使用vector；
*   如果你的程序有很多小元素，且空间的额外开销很重要，不要使用`list`或`forward_list`；
*   如果程序要求随机访问元素，应使用vector或deque；
*   如果程序要求在容器的中间插入或删除元素，应使用`list`或`forward_list`；
*   如果程序要求在头尾位置插入或删除元素，但不会在中间位置插入或删除，那么使用deque；
*   如果程序只有在读取输入时才需要在容器中间位置插入元素，随后需要随机访问，
    *   确定是否真的需要在容器中间插入元素，排序之类的可以使用sort重排；
    *   如果必须在中间插入元素，考虑在输入阶段使用list，输入完成之后将其拷贝到vector中；
*   如果程序既要求随机访问又要求在容器中间插入，那么根据占主导地位的操作选择容器。

<h3 id="basic_container_operation">容器基本操作</h3>

#### 类型别名

**类型别名在泛型编程中非常有用**。

类型别名 | 说明
--------------- | -----------------------------------------
`iterator` | 此容器类型的迭代器类型
`const_iterator` | 不可修改元素的迭代器
`size_type` | 无符号整数类型，足以保存此种容器类型最大可能容器的大小
`difference_type` | 带符号整数类型，足以保存两个迭代器之间的距离
`value_type` | 元素类型
`reference` | 元素的左值类型；相当于`value_type&`
`const_reference` | 相当于`const value_type&`

#### 构造函数

构造函数 | 说明
--------------- | -------------------------------------------------
`C c` | 默认构造函数<br>若C是一个array，那么c中元素按默认方式初始化<br>否则构造一个空容器
`C c1(c2)`<br>`C c1 = c2` | 非explicit构造函数<br>c1初始化为c2的拷贝<br>**c1和c2容器类型和元素类型必须相同**<br>对于array，还必须拥有相同大小
`C c{a,b,c...}`<br>`C c = {a,b,c...}` | 非explicit构造函数<br>c初始化为初始化列表中元素的拷贝<br>列表中元素的类型必须与C的元素类型相容<br>对于array类型，列表元素数目须不大于array的大小，遗漏的元素将进行值初始化
`C c(b, e)` | c初始化为迭代器b和e指定范围中的元素的拷贝<br>范围中元素的类型必须与c的元素类型相容<br>不适用于array
`C seq(n)` | explicit构造函数<br>适用于`vector`,`deque`,`list`,`forward_list`<br>seq包含n个元素，这些元素进行值初始化<br>自定义类型必须具有默认构造函数
`C seq(n, t)` | 适用于`vector`,`deque`,`list`,`forward_list`,`string`<br>seq包含n个初始化为值t的元素

#### 常用容器操作

常用容器操作 | 说明
------------------ | -------------------------------------------------
`a.swap(b)` | 交换a与b的元素<br>**a与b必须具有相同的类型**<br>**swap比赋值操作快得多**<br>除array外，swap不对任何元素进行拷贝、删除或插入操作
`swap(a, b)` | 等价于`a.swap(b)`<br>非成员版本的swap在泛型编程中非常重要<br>**统一使用非成员版本的swap**
`c.size()` | c中元素的数目<br>不支持`forward_list`
`c.max_size()` | c中可保存的最大元素数目
`c.empty()` | 若c中未存储元素，返回true
`==, !=` | 所有容器都支持相等和不等运算符
`<, <=, >, >=` | 关系运算符，只支持顺序容器<br>左右两边的运算对象的容器类型和容器元素必须相同<br>元素类型必须定义相应的元素比较运算符<br>比较过程类似于string
`c.begin()`<br>`c.end()` | 返回指向c的首元素与尾元素之后位置的迭代器<br>若c为const对象，那么调用返回`const_iterator`的版本
`c.cbegin()`<br>`c.cend()` | 返回`const_iterator`
`c1 = c2` | 将c1中的元素替换为c2中元素的拷贝<br>**c1, c2必须具有相同的类型**
`c = {a,b,c,...}` | 将c1中元素替换为初始化列表中元素的拷贝<br>会改变容器的大小(array除外)
`seq.assign(b, e)` | 将seq中的元素替换为迭代器b和e所表示的范围中的元素<br>b和e不能指向seq中的元素<br>不适用于array和关联容器
`seq.assign(il)` | 将seq中的元素替换为初始化列表il中的元素<br>不适用于array和关联容器
`seq.assign(n, t)` | 将seq中的元素替换为n个值为t的元素<br>不适用于array和关联容器

#### 反向容器

**对一个反向迭代器执行++操作，会得到上一个元素**。

反向容器(不支持`forward_list`) | 说明
---------------------- | -------------------------------------------------
`reverse_iterator` | 按逆序寻址元素的迭代器
`const_reverse_iterator` | 不可修改元素的逆序迭代器
`c.rbegin(), c.rend()` | 返回指向c的尾元素和首元素之前的迭代器<br>若c为const对象，那么调用返回`const_reverse_iterator`的版本
`c.crbegin(), c.crend()` | 返回`const_reverse_iterator`

#### 迭代器

**`forward_list`迭代器不支持递减运算符**。

**当不需要写访问时，使用`c(r)begin`和`c(r)end`**。

#### array

定义一个array时，除了指定元素类型，还要指定容器大小:

```c++
std::array<int, 42> arr;
```

array不初始化时使用默认初始化；列表初始化时，未被初始化得元素将使用值初始化，自定义类必须包含一个默认构造函数：

```c++
std::array<int, 42> arr0;                 // 默认初始化
std::array<int, 42> arr1{1,2,3,4,5};      
std::array<int, 42> arr2 = {2,3,4,5,6};
```

array可以进行拷贝和赋值操作：

```c++
std::array<int, 42> arr0{2,3,4,5,6};
// 拷贝赋值时容器类型、元素类型、容器大小都必须相同
std::array<int, 42> arr1(arr0);
std::array<int, 42> arr2 = arr0;
arr2 = arr1;
```

<h3 id="sequential_container_operation">顺序容器操作</h3>

#### 向顺序容器添加元素

array不支持添加元素。

`forward_list`不支持`push_back`和`emplace_back`；`forward_list`有自己专有版本的insert和emplace。

vector和string不支持`push_front`和`emplace_front`。

使用一个对象来初始化容器或将一个对象插入到容器中时，**放入容器中的元素是对象值得一个拷贝，而不是对象本身**。

`emplace`,`emplace_back`和`emplace_front`是C++11新引入的成员，它们在容器中直接构造元素，传递给emplace函数的参数必须与元素类型的构造函数相匹配。无参数时，使用默认构造函数。

向顺序容器添加元素 | 说明
------------------ | -------------------------------------------------
`c.push_back(t)`<br>`c.emplace_back(args)` | 在c的尾部创建一个值为t或由args创建的元素，返回void
`c.push_front(t)`<br>`c.emplace_front(args)` | 在c的头部创建一个值为t或由args创建的元素，返回void
`c.insert(p, t)`<br>`c.emplace(p, args)` | 在迭代器p指向的元素之前创建一个值为t或由args创建的元素<br>返回新添加的元素的迭代器
`c.insert(p, n, t)` | 在迭代器p指向的元素之前插入n个值为t的元素<br>返回指向新添加的第一个元素的迭代器<br>若n为0，则返回p
`c.insert(p, b, e)` | 将迭代器b和e指定范围内的元素插入到迭代器p指向的元素之前<br>b和e不能指向c中的元素<br>返回指向新添加的第一个元素的迭代器<br>若范围为空，则返回p
`c.insert(p, il)` | 在迭代器p指向的元素之前插入列表il中的元素<br>返回指向新添加的第一个元素的迭代器<br>若列表为空，则返回p

#### 访问元素

访问元素操作 | 说明
------------------ | -------------------------------------------------
`c.back()` | 返回c中尾元素的引用<br>**若c为空，函数行为未定义**
`c.front()` | 返回c中首元素的引用<br>**若c为空，函数行为未定义**<br>不适用于`forward_list`
`c[n]` | 返回c中下标为n的元素的引用<br>n是一个无符号整数<br>若n>=c.size()，函数行为未定义<br>只适用于string,vector,deque和array
`c.at(n)` | 返回下标为n的元素的引用<br>若下标越界，则抛出`out_of_range`异常<br>只适用于string,vector,deque和array

```c++
auto b1 = c.back();  // b1是c中最后一个元素的拷贝
auto &b2 = c.back(); // b2是c中最后一个元素的引用
```

#### 删除元素

array不支持删除元素。

`forward_list`有特殊版本的erase；`forward_list`不支持`pop_back`。

vector和string不支持`pop_front`。

**删除元素前，必须确保它们是存在的**。

删除元素操作 | 说明
------------------ | -------------------------------------------------
`c.pop_back()` | 删除c中尾元素<br>若c为空，则函数行为未定义<br>返回void
`c.pop_front()` | 删除c中首元素<br>若c为空，则函数行为未定义<br>返回void
`c.erase(p)` | 删除迭代器p所指向的元素<br>返回一个指向被删元素之后元素的迭代器<br>若p是尾后迭代器，则函数行为未定义
`c.erase(b,e)` | 删除迭代器b和e所指定范围内(前闭后开)的元素<br>返回e
`c.clear()` | 删除c中的所有元素<br>返回void

#### 特殊的`forward_list`操作

在一个`forward_list`中添加或删除元素的操作是通过改变给定元素之后的元素来完成的。

`forward_list`插入删除操作 | 说明
------------------------- | -------------------------------------------------
`lst.before_begin()`<br>`lst.cbefore_begin()` | 返回指向链表首元素之前不存在的元素的迭代器<br>不可解引用
`lst.insert_after(p, t)`<br>`lst.insert_after(p, n, t)`<br>`lst.insert_after(p, b, e)`<br>`lst.insert_after(p, il)` | 在迭代器p之后的位置插入元素<br>t是对象，n是数量，b和e是表示范围的一对迭代器(b和e不能指向lst内)，il是花括号列表<br>返回指向最后一个插入元素的迭代器<br>若范围为空，则返回p<br>若p为尾后迭代器，则函数行为未定义
`emplace_after(p, args)` | 使用args在p指定的位置之后创建一个元素<br>返回一个指向这个新元素的迭代器<br>若p为尾后迭代器，则函数行为未定义
`lst.erase_after(p)`<br>`lst.erase_after(b, e)` | 删除p指向的位置之后的元素，或删除`(b,e]`之间的元素<br>返回一个指向被删元素之后元素的迭代器<br>若p指向lst的尾元素，或p是一个尾后迭代器，则函数行为未定义

#### 改变容器的大小

顺序容器大小操作(不适用于array) | 说明
----------------------------- | -------------------------------------------------
c.resize(n) | 调整c的大小为n个元素<br>若`n<c.size()`，则多出的元素被丢弃，反之多出的元素进行值初始化
c.resize(n, t) | 调整c的大小为n个元素<br>若`n<c.size()`，则多出的元素被丢弃，反之多出的元素被初始化为t

**resize并不改变容器`capacity()`的大小**。

**原先得到的迭代器在容器操作后可能造成迭代器失效，使用失效的迭代器会造成严重的运行错误，建议在每次容器操作之后都更新一下指向相应容器的迭代器**。

#### 管理容量

由于vector和string的元素是连续存储的，向它们添加元素时：

*   容器必须分配新的内存空间来保存已有元素和新元素；
*   将已有元素从旧的位置移动到新空间；
*   然后添加新元素，释放旧存储空间。

如果每次添加都执行一次分配和释放操作，会严重降低性能。

标准库采用可以减少容器重新分配次数的策略：

*   不得不获取新的内存空间时，vector和string会分配比新的空间需求更大的内存空间；
*   容器预留这些空间作为备用，用以保存更多的新元素;
*   采用这种策略后，vector的扩张操作比list和deque更快。

容器容量管理 | 说明
-------------- | -------------------------------------------------
`c.capacity()` | 不重新分配内存空间的话，c保存元素的数量<br>仅适用于vector和string
`c.shrink_to_fit()` | 将`capacity()`减小为与size()相同大小<br>仅适用于vector,string和deque
`c.reserve(n)` | 分配至少能容纳n个元素的内存空间<br>若需求小于等于当前容量，什么也不做<br>仅适用于vector和string

#### string常用操作

string常用操作([args形式](#string_args_style0)) | 说明
------------------------ | -------------------------------------------------
`string s(cp)` | cp是`const char*`参数，拷贝从cp开始直到空字符间的字符到s
`s.substr(pos, n)` | 返回一个string，包含s中从pos开始的n个字符的拷贝<br>pos的默认值为0，n的默认值为`s.size() - pos`
`s.insert(pos, args)` | 在pos之前插入args指定的字符<br>pos是下标时，返回一个指向s的引用<br>pos是迭代器时，返回指向第一个插入字符的迭代器
`s.erase(pos, len)` | 删除从位置pos开始的len个字符<br>len的默认值为`s.size() - pos`<br>返回一个指向s的引用
`s.assign(args)` | 将s中的字符替换为args指定的字符<br>返回一个指向s的引用
`s.append(args)` | 将args追加到s<br>返回一个指向s的引用
`s.replace(range, args)` | 删除s中范围为range中的字符，替换为args指定的字符<br>range可以是一个下标和一个长度，也可以是一对指向s的迭代器<br>返回一个指向s的引用

<h5 id="string_args_style0">args形式</h5>

args形式(assign和append都支持) | 说明
----------------------------- | -------------------------------------------------
str | 字符串<br>`replace(pos,len,args)`<br>`replace(b,e,args)`<br>`insert(pos,args)`(pos为下标)
str, pos, len | str中从pos开始最多len个字符<br>`replace(pos,len,args)`<br>`insert(pos,args)`(pos为下标)
cp, len | 从cp指向的字符数组的最多前len个字符<br>`replace(pos,len,args)`<br>`replace(b,e,args)`<br>`insert(pos,args)`(pos为下标)
cp | cp指向的以空字符结尾的字符数组<br>`replace(pos,len,args)`<br>`replace(b,e,args)`
n, c | n个字符c<br>`replace(pos,len,args)`<br>`replace(b,e,args)`<br>`insert(pos,args)`<br>`insert(iter,args)`
b, e | 迭代器b和e指定范围内的字符<br>`replace(b,e,args)`<br>`insert(iter,args)`
初始化列表 | 花括号包围的，以逗号分隔的字符列表<br>`replace(b,e,args)`<br>`insert(iter,args)`

#### string搜索操作

string搜索操作([args形式](#string_args_style1)) | 说明
------------------------ | -------------------------------------------------
`s.find(args)` | 查找s中args第一次出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始
`s.rfind(args)` | 查找s中args最后一次出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始指向从左到右第一个args字符
`s.find_first_of(args)` | 查找s中args中任何一个字符第一次出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始
`s.find_last_of(args)` | 查找s中args中任何一个字符最后一次出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始
`s.find_first_not_of(args)` | 查找s中第一个不在args中的字符出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始
`s.find_last_not_of(args)` | 查找s中最后一个不在args中的字符出现的位置<br>失败返回`string::npos`，成功返回一个`string::size_type`值，从零开始

<h5 id="string_args_style1">args形式</h5>

args形式 | 说明
---------- | -------------------------------------------------
c, pos | 从s中位置pos开始查找字符c <br>pos默认为0
s2, pos | 从s中位置pos开始查找字符串s2 <br>pos默认为0
cp, pos | 从s中位置pos开始查找指针cp指向的以空字符结尾的C风格字符串<br>pos默认为0
cp, pos, n | 从s中位置pos开始查找指针cp指向的数组的前n个字符<br>pos和n无默认值

#### string数值转换

如果string不能转换为一个数值，这些函数抛出一个`invalid_argument`异常。

如果转换后的数值无法用任何类型表示，则返回一个`out_of_range`异常。

**能够进行数值转换的string格式**：

*   **string参数中第一个非空白字符必须是符号+或-或数字**；
*   它可以以`0x`或`0X`开头表示十六进制数；
*   对于要转换成浮点数的函数，string参数也可以以小数点(\.)开头，并可以包含e或E来表示指数部分；
*   对于要转换成整型值的函数，根据基数不同，string参数可以包含字母字符，对应大于9的数。

string数值转换 | 说明
-------------- | -------------------------------------------------
`to_string(val)` | 返回数值val的string表示<br>val可以是任何算数类型
`stoi(s, p, b)`<br>`stol(s, p, b)`<br>`stoul(s, p, b)`<br>`stoll(s, p, b)`<br>`stoull(s, p, b)` | 返回字符串s从开始到p的子串表示的数值<br>分别是int,long,unsigned long,long long,unsigned long long <br>p是`size_t`指针，用来保存s中第一个非数值字符的下标，默认为0(代表全部字符串) <br>b表示转换所用的基数，默认为10
`stof(s, p)`<br>`stod(s, p)`<br>`stold(s, p)` | 返回字符串s从开始到p的子串表示的数值<br>分别是float,double和long double <br>p是`size_t`指针，用来保存s中第一个非数值字符的下标，默认为0(代表全部字符串)

<h2 id="generic_algorithms">容器泛型算法</h2>

大多数算法定义于头文件`<algorithm>`中。

容器泛型算法 | 说明
------------------------------- | -------------------------------------------------
`for_each(b, e, func)` | 对迭代器范围`[b1,e1)`范围内的所有值都调用一次func <br> func可以使用[lambda表达式](#lambda_expression)，接受一个参数
`find(b, e, val)` | 在迭代器范围`[b,e)`范围内搜索val <br>成功返回指向第一个匹配值的元素的迭代器<br>失败返回e
`find_if(b, e, func)` | 返回迭代器范围`[b,e)`范围内指向第一个使func返回true的元素的迭代器<br> func可以使用[lambda表达式](#lambda_expression)，接受一个参数<br>失败返回e
`accumulate(b, e, sum)` | 对迭代器范围`[b,e)`范围内的所有值求和，总和加上sum值为返回值<br> sum的类型决定使用哪种加法，以及返回值的类型(例如string)<br>sum是值传递，代表和的初始值<br>`const char*`没有定义+运算符<br>头文件`<numeric>`
`equal(b1, e1, b2)` | 若迭代器范围`[b1,e1)`范围内的所有值都与从迭代器b2开始的的值对应相等，则返回true <br>从b2开始必须要有`e1 - b1`个元素存在，否则未定义<br>b1和b2指向的元素类型必须要能够使用`==`进行比较
`fill(b, e, val)` | 将迭代器范围`[b,e)`范围内的所有值赋值为val
`fill_n(dest, n, val)` | 将val赋值给从迭代器dest开始的n个元素<br>从dest开始的序列至少包含n个元素
`copy(b1, e1, b2)` | 将迭代器范围`[b1,e1)`范围内的所有值对应拷贝到从迭代器b2开始的值<br>从b2开始必须要有`e1 - b1`个元素存在，否则未定义<br>b1和b2指向的元素类型必须相容
`replace(b, e, old, new)` | 将迭代器范围`[b,e)`范围内的所有等于old的值赋值为new
`replace(b1, e1, e2, old, new)` | 将迭代器范围`[b1,e1)`范围内的所有等于old的值赋值为new，然后保存至从迭代器e2开始的位置，`[b1,e1)`范围内的值不变<br>从b2开始必须要有`e1 - b1`个元素存在，否则未定义
`sort(b, e)` | 将迭代器范围`[b,e)`范围内的所有值从小到大排序<br>默认使用元素类型的\<运算符
`sort(b, e, func)` | 将迭代器范围`[b,e)`范围内的所有值按相邻两两使用func返回true的形式排序，相等元素不一定维持原有顺序<br>func可以使用[lambda表达式](#lambda_expression)，接受两个参数
`stable_sort(b, e, func)` | 与`sort(b, e, func)`类似，但相等元素维持原有顺序<br>func可以使用[lambda表达式](#lambda_expression)，接受两个参数
`unique(b, e)` | 将迭代器范围`[b,e)`范围内的所有相邻的重复项“消除”，使得不重复部分出现在序列开始部分<br>多余的重复项将被排到后面，顺序不定<br>返回一个指向最后一个不重复元素之后的迭代器
`transform(b1, e1, b2, func)` | 对迭代器范围`[b1,e1)`范围内的所有值调用func，并将结果写入从b2开始的位置<br>从b2开始必须要有`e1 - b1`个元素存在，否则未定义<br>func可以使用[lambda表达式](#lambda_expression)，接受一个参数

<h3 id="lambda_expression">lambda 表达式</h3>

lambda 表达式表示一个可调用的代码单元，可以简单的理解为一个未命名的内联函数。

一个 lambda 表达式具有一个返回类型、一个参数列表和一个函数体:

```text
[capture list](parameter list) -> return type { function body }
```

*   capture list 表示该lambda表达式能够使用的非静态局部变量的列表，为空时代表该lambda表达式不使用任何非静态局部变量；
*   变量的捕获可以是值或引用。为值时，对相应值的改变不会影响到原值；为引用时，必须确保该lambda在执行时，引用变量是存在的；
*   当捕获列表为`[=]`时，代表lambda函数体中使用的所有非静态局部变量都采用值捕获；
*   当捕获列表为`[&]`时，代表lambda函数体中使用的所有非静态局部变量都采用引用捕获；
*   当捕获列表为`[=, identifier_list]`时，代表lambda函数体中除`identifier_list`列表中的**引用捕获变量**外，其它使用的所有非静态局部变量都采用值捕获；
*   当捕获列表为`[&, identifier_list]`时，代表lambda函数体中除`identifier_list`列表中的**值捕获变量**外，其它使用的所有非静态局部变量都采用引用捕获；
*   **一个lambda只有在其捕获列表中捕获一个它所在函数中的非静态局部变量，才能在函数体中使用该变量**；
*   **一个lambda可以直接使用定义在它所在函数中的静态局部变量和定义在它所在函数之外的可用的变量**；
*   parameter list 不能包含默认参数；
*   lambda 必须使用尾置返回类型；
*   lambda 表达式可以忽略参数列表及括号和返回类型，但必须包含capture list 和 function body。
*   忽略参数列表及括号代表不接受参数；
*   忽略返回类型，**如果lambda表达式包含任何单一return语句之外的内容，则返回void**；否则从返回表达式的类型推断出返回类型；

```c++
void example(std::ostream &os)
{
	std::vector<int> vec{ 1,2,3,3,2,1 };
	int num = 2;
	transform(cbegin(vec), cend(vec), begin(vec), 
		[&os, num](const int &i) -> int 
	{
		os << i << std::ends;
		return i < num ? i : num; 
	}
	);
	os << std::endl;
	for_each(cbegin(vec), cend(vec),
		[](const int &i)
	{
		std::cout << i << std::ends;
	}
	);
}
```
