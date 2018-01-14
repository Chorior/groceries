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
	*   [顺序容器概述](#sequential_containers_overview)
    *   [顺序容器选择](#choose_sequential_container)
    *   [容器基本操作](#basic_container_operation)
		*   [类型别名](#basic_type_alias)
		*   [构造函数](#basic_constructor)
		*   [常用操作](#basic_common_operations)
		*   [反向容器](#reverse_containers)
		*   [迭代器](#container_iterators)
		*   [array 容器](#array_container)
    *   [顺序容器操作](#sequential_container_operation)
		*   [添加元素](#add_elements)
		*   [访问元素](#access_elements)
		*   [删除元素](#delete_elements)
		*   [改变大小](#resize_container)
		*   [容量管理](#capacity_management)
		*   [string 常用操作](#string_common_operations)
		*   [string 搜索操作](#string_search_operations)
		*   [string 数值转换](#string_numeric_conversion)
		*   [特殊的 forward_list](#special_forward_list)
*   [容器泛型算法](#generic_algorithms)
    *   [lambda 表达式](#lambda_expression)
	*   [函数适配器 bind](#bind_function)
	*   [各种迭代器](#other_iterators)
*   [关联容器](#associative_containers)
	*   [关联容器构造](#associative_container_constructor)
	*   [pair 类型](#pair_type)
	*   [关联容器操作](#associative_container_operations)
		*   [类型别名](#associative_container_type_alias)
		*   [map 类操作](#map_class_operations)
		*   [关联容器常用操作](#common_associative_container_operations)
		*   [无序关联容器](#unordered_associative_containers)
*   [动态内存](#dynamic_memory)
	*   [new, delete](#new_and_delete)
	*   [智能指针](#smart_pointer)

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

标准库容器分为顺序容器与[关联容器](#associative_containers)。

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

<h4 id="basic_type_alias">类型别名</h4>

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

<h4 id="basic_constructor">构造函数</h4>

构造函数 | 说明
--------------- | -------------------------------------------------
`C c` | 默认构造函数<br>若C是一个array，那么c中元素按默认方式初始化<br>否则构造一个空容器
`C c1(c2)`<br>`C c1 = c2` | 非explicit构造函数<br>c1初始化为c2的拷贝<br>**c1和c2容器类型和元素类型必须相同**<br>对于array，还必须拥有相同大小
`C c{a,b,c...}`<br>`C c = {a,b,c...}` | 非explicit构造函数<br>c初始化为初始化列表中元素的拷贝<br>列表中元素的类型必须与C的元素类型相容<br>对于array类型，列表元素数目须不大于array的大小，遗漏的元素将进行值初始化
`C c(b, e)` | c初始化为迭代器b和e指定范围中的元素的拷贝<br>范围中元素的类型必须与c的元素类型相容<br>不适用于array
`C seq(n)` | explicit构造函数<br>适用于`vector`,`deque`,`list`,`forward_list`<br>seq包含n个元素，这些元素进行值初始化<br>自定义类型必须具有默认构造函数
`C seq(n, t)` | 适用于`vector`,`deque`,`list`,`forward_list`,`string`<br>seq包含n个初始化为值t的元素

<h4 id="basic_common_operations">常用操作</h4>

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

<h4 id="reverse_containers">反向容器</h4>

**对一个反向迭代器执行++操作，会得到上一个元素**。

反向容器(不支持`forward_list`) | 说明
---------------------- | -------------------------------------------------
`reverse_iterator` | 按逆序寻址元素的迭代器
`const_reverse_iterator` | 不可修改元素的逆序迭代器
`c.rbegin(), c.rend()` | 返回指向c的尾元素和首元素之前的迭代器<br>若c为const对象，那么调用返回`const_reverse_iterator`的版本
`c.crbegin(), c.crend()` | 返回`const_reverse_iterator`

<h4 id="container_iterators">迭代器</h4>

**`forward_list`迭代器不支持递减运算符**。

**当不需要写访问时，使用`c(r)begin`和`c(r)end`**。

除了为每个容器定义的迭代器之外，标准库在头文件iterator中还定义了[额外几种迭代器](#other_iterators)。

<h4 id="array_container">array 容器</h4>

定义一个array时，除了指定元素类型，还要指定容器大小:

```c++
std::array<int, 42> arr;
```

array不初始化时使用默认初始化；列表初始化时，未被初始化得元素将使用值初始化，**自定义类必须包含一个默认构造函数**：

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

<h4 id="add_elements">添加元素</h4>

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

<h4 id="access_elements">访问元素</h4>

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

<h4 id="delete_elements">删除元素</h4>

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

<h4 id="resize_container">改变大小</h4>

顺序容器大小操作(不适用于array) | 说明
----------------------------- | -------------------------------------------------
c.resize(n) | 调整c的大小为n个元素<br>若`n<c.size()`，则多出的元素被丢弃，反之多出的元素进行值初始化
c.resize(n, t) | 调整c的大小为n个元素<br>若`n<c.size()`，则多出的元素被丢弃，反之多出的元素被初始化为t

**resize并不改变容器`capacity()`的大小**。

**原先得到的迭代器在容器操作后可能造成迭代器失效，使用失效的迭代器会造成严重的运行错误，建议在每次容器操作之后都更新一下指向相应容器的迭代器**。

<h4 id="capacity_management">容量管理</h4>

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

<h4 id="string_common_operations">string 常用操作</h4>

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

<h4 id="string_search_operations">string 搜索操作</h4>

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

<h4 id="string_numeric_conversion">string 数值转换</h4>

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

<h4 id="special_forward_list">特殊的`forward_list`</h4>

在一个`forward_list`中添加或删除元素的操作是通过改变给定元素之后的元素来完成的。

`forward_list`插入删除操作 | 说明
------------------------- | -------------------------------------------------
`lst.before_begin()`<br>`lst.cbefore_begin()` | 返回指向链表首元素之前不存在的元素的迭代器<br>不可解引用
`lst.insert_after(p, t)`<br>`lst.insert_after(p, n, t)`<br>`lst.insert_after(p, b, e)`<br>`lst.insert_after(p, il)` | 在迭代器p之后的位置插入元素<br>t是对象，n是数量，b和e是表示范围的一对迭代器(b和e不能指向lst内)，il是花括号列表<br>返回指向最后一个插入元素的迭代器<br>若范围为空，则返回p<br>若p为尾后迭代器，则函数行为未定义
`emplace_after(p, args)` | 使用args在p指定的位置之后创建一个元素<br>返回一个指向这个新元素的迭代器<br>若p为尾后迭代器，则函数行为未定义
`lst.erase_after(p)`<br>`lst.erase_after(b, e)` | 删除p指向的位置之后的元素，或删除`(b,e]`之间的元素<br>返回一个指向被删元素之后元素的迭代器<br>若p指向lst的尾元素，或p是一个尾后迭代器，则函数行为未定义

<h2 id="generic_algorithms">容器泛型算法</h2>

大多数算法定义于头文件`<algorithm>`中。

**`list`和`forward_list`有专门的sort, merge, remove, reverse和unique算法**，并且定义了独有的splice成员，用于拼接。

很多算法都有重载，以及`_if`, `_copy`, `_n`, `_not`等后缀组合的版本，下面列出一些常用算法：

容器泛型常用算法 | 说明(func为可调用对象或[lambda表达式](#lambda_expression))
------------------------------- | -------------------------------------------------
`accumulate(b, e, sum)` <br> `accumulate(b, e, sum, func)` | 对迭代器范围`[b,e)`范围内的所有元素求和，总和加上sum值为返回值<br> sum的类型决定使用哪种加法(若没有func指定)，以及返回值的类型(如string)<br>sum是值传递，代表和的初始值<br>`const char*`没有定义+运算符<br>头文件`<numeric>`
`all_of(b, e, func)` | 若序列为空，或迭代器范围`[b1,e1)`范围内的所有元素都能使func返回true，则返回true
`any_of(b, e, func)` | 若迭代器范围`[b1,e1)`范围内的能找出一个元素使得func返回true，则返回true<br>序列为空时，返回false
`none_of(b, e, func)` | 若序列为空，或迭代器范围`[b1,e1)`范围内的所有元素都不能使func返回true，则返回true
`copy(b, e, dest)` <br> `copy_if(b, e, dest, func)`| 将迭代器范围`[b,e)`范围内的所有元素或满足func的元素拷贝到从迭代器dest开始的值<br>b和dest指向的元素类型必须相容
`copy_n(beg, n, dest)` | 将从beg开始的n的元素赋值给从dest开始的位置<br>beg和dest指向的元素类型必须相容
`count(b, e, val)` <br> `count_if(b, e, func)` | 返回迭代器范围`[b,e)`范围内与val相等或令func返回true的元素的数量
`equal(b1, e1, b2)` <br> `equal(b1, e1, b2, func)` | 若迭代器范围`[b1,e1)`范围内的所有元素都与从迭代器b2开始的的元素对应相等，则返回true <br>默认使用`==`进行比较，func指定比较函数
`for_each(b, e, func)` | 对迭代器范围`[b1,e1)`范围内的所有元素都调用一次func
`find(b, e, val)` <br> `find_if(b, e, func)`| 在迭代器范围`[b,e)`范围内搜索val或调用func <br>成功返回指向第一个匹配值的元素的迭代器<br>失败返回e
`fill(b, e, val)` | 将迭代器范围`[b,e)`范围内的所有元素赋值为val
`fill_n(dest, n, val)` | 将val赋值给从迭代器dest开始的n个元素<br>从dest开始的序列至少包含n个元素
`max(val1, val2)` <br> `max(val1, val2, func)` | 返回val1和val2中较大者的const引用<br>val1和val2的类型必须完全一致
`min(val1, val2)` <br> `min(val1, val2, func)` | 返回val1和val2中较小者的const引用<br>val1和val2的类型必须完全一致
`max_element(b, e)` <br> `max_element(b, e, func)` | 返回指向迭代器范围`[b,e)`范围内最大元素的迭代器
`min_element(b, e)` <br> `min_element(b, e, func)` | 返回指向迭代器范围`[b,e)`范围内最小元素的迭代器
`replace(b, e, old, new)` <br> `replace_if(b, e, func, new)` | 将迭代器范围`[b,e)`范围内的所有等于old或满足func的元素赋值为new
`replace(b, e, dest, old, new)` | 将迭代器范围`[b,e)`范围内的所有等于old的元素赋值为new，然后保存至从迭代器dest开始的位置，`[b,e)`范围内的值不变
`remove(b, e, val)` <br> `remove_if(b, e, func)` | 删除迭代器范围`[b,e)`范围内所有等于val或使func返回true的元素
`reverse(b, e)` | 将迭代器范围`[b,e)`范围内的所有元素顺序反转
`reverse_copy(b, e, dest)` | 将迭代器范围`[b,e)`范围内的所有元素按逆序拷贝到从dest开始的位置<br>b1和b2指向的元素类型必须相容
`search(b1, e1, b2, e2)` <br> `search(b1, e1, b2, e2, func)` | 返回迭代器范围`[b2,e2]`子序列在迭代器范围`[b1,e1]`序列中第一次出现的位置<br>失败返回e1<br>默认使用`==`运算符，func指定比较函数
`set_union(b1, e1, b2, e2, dest)` <br> `set_union(b1, e1, b2, e2, dest, func)` | 创建两个序列的有序序列到dest<br>同一序列多次出现的元素不会被删除，两个序列都包含的元素只出现一次<br> func指定比较函数
`sort(b, e)` <br> `sort(b, e, func)` | 将迭代器范围`[b,e)`范围内的所有元素排序<br>默认使用元素类型的\<运算符，func指定比较函数
`is_sorted(b, e)` <br> `is_sorted(b, e, func)` | 判断迭代器范围`[b,e)`范围内的元素是否有序<br>默认使用元素类型的\<运算符，func指定比较函数
`stable_sort(b, e, func)` | 与`sort(b, e, func)`类似，但相等元素维持原有顺序<br>func接受两个参数
`transform(b, e, dest, func)` | 对迭代器范围`[b,e)`范围内的所有值调用func，并将结果写入从dest开始的位置
`transform(b1, e1, b2, dest, func)` | func接受两个参数<br>对迭代器范围`[b1,e1)`和从b2开始范围内对应的所有元素调用func，并将结果写入从dest开始的位置
`unique(b, e)` <br> `unique(b, e, func)`| 将迭代器范围`[b,e)`范围内的所有相邻的重复项“消除”，使得不重复部分出现在序列开始部分<br>多余的重复项将被排到后面，顺序不定<br>返回一个指向最后一个不重复元素之后的迭代器<br>默认使用`==`进行比较，func指定比较函数

<h3 id="lambda_expression">lambda 表达式</h3>

lambda 表达式表示一个可调用的代码单元，可以简单的理解为一个未命名的内联函数。对于那种只在一两个地方使用的简单操作，lambda 表达式是最有用的。

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
*   **一个lambda只有在其捕获列表中捕获一个它所在函数中的非静态局部变量后，才能在函数体中使用该变量**；
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

<h3 id="bind_function">函数适配器 bind</h3>

C++11 在`<functional>`头文件中定义了一个名为bind的标准库函数，它接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。

```c++
auto newCallable = bind(callable, arg_list);
```

*   newCallable 本身是一个可调用对象；
*   `arg_list` 是一个逗号分隔的参数列表，对应于 callable 的参数；
*   当我们调用 newCallable 时，newCallable 会调用 callable，并**按顺序**传递给它`arg_list` 中的参数；
*   **`arg_list` 中如果需要引用传递，须使用`std::ref`或`std::cref`函数**，c代表const；
*   `arg_list` 中的参数可能包含形如 `_n` 的名字，其中n是一个整数，代表在生成的可调用对象中参数的位置，例如`_2`代表第二个参数；
*   `arg_list` 中 `_n` 的最大值为生成的可调用对象参数的总个数；

```c++
# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <functional>

bool isShorter(const std::string &s1, const std::string &s2);
void print(std::ostream &os, const std::string &s);
void print(const int *begin, const int *end);

int main()
{
	using namespace std;

	vector<string> svec{ "jskda","asda","dkasdka","iqheqen","qyeiqk" };
	sort(begin(svec), end(svec), isShorter);
    // 与上面效果一样
	sort(begin(svec), end(svec),
		[](const std::string &s1, const std::string &s2)
	{
		return s1.size() < s2.size();
	}
	);

    // 使用 _1, _2
	using namespace std::placeholders;

    // 利用bind重排参数顺序，反向排序
	sort(begin(svec), end(svec), bind(isShorter,_2,_1));

	for_each(cbegin(svec), cend(svec), 
		[](const std::string &s)
	{
		cout << s << std::endl;
	}
	);
    // 使用bind将cout引用绑定到print第一个参数，生成一个只接受一个参数的可调用对象
    // 如果bind的函数有重载，须使用 static_cast 指定类型，print可以添加取地址符号&
	for_each(cbegin(svec), cend(svec), 
		bind(static_cast<void(*)(std::ostream &, const std::string &)>(print), ref(cout), _1));

	getchar();
	return 0;
}

bool isShorter(const std::string &s1, const std::string &s2)
{
	return s1.size() < s2.size();
}

void print(std::ostream &os, const std::string &s)
{
	os << s << std::endl;
}

void print(const int *begin, const int *end)
{
	while (begin != end) std::cout << *begin++ << std::endl;
}
```

<h3 id="other_iterators">各种迭代器</h3>

迭代器类型 | 说明
--------- | -------------------------------------------------
插入迭代器 | 被绑定到一个容器，用以向容器插入元素<br>当通过一个插入迭代器进行赋值时，该迭代器调用容器操作在指定位置插入一个元素<br> `it = back_inserter(c); *it = val;`对应`c.push_back(val);` <br> `it = front_inserter(c); *it = val;`对应`c.push_front(val);` <br> `it = inserter(c, iter); *it = val;`对应`it = c.insert(iter, val); ++ it;` <br>插入迭代器对于那些第二个序列只有一个迭代器的算法特别有用，如`copy(std::cbegin(c1),std::cend(c1),back_inserter(c2));`
流迭代器 | 被绑定到输入或输出流，用以遍历关联的 IO 流<br> `istream_iterator`读取输入流，`ostream_iterator`向输出流写数据<br>流迭代器对于那些第二个序列只有一个迭代器的算法特别有用，如`copy(std::cbegin(vec),std::cend(vec),out);`
反向迭代器 | 从右向左移动的迭代器，只有`forward_list`不支持<br>使用`riter.base()`得到指向riter指向的元素右边的元素的正向迭代器
移动迭代器 | 不拷贝其中的元素，而是移动它们

```c++
std::istream_iterator<int> in_iter(std::cin), eof;
// 读取输入，并保存到一个vector
std::vector<int> vec(in_iter, eof);
// 读取输入，并求和
// auto sum = std::accumulate(in_iter, eof, 0);
// std::ostream_iterator<T> out(os, d)
// 将类型为T的值写到os，每个值后面输出一个d，d指向一个空字符结尾的字符数组
std::ostream_iterator<int> out(std::cout, " ");
copy(std::cbegin(vec), std::cend(vec), out);
std::cout << std::endl;
```

<h2 id="associative_containers">关联容器</h2>

关联容器中的元素是按关键字来保存和访问的。map类的元素类型为[pair](#pair_type);

对于有序容器`map`,`set`,`multimap`和`multiset`，关键字类型必须定义元素比较的方法。

关联容器类型 | 说明
-------------------- | -------------------------------------------------
`map` | 关联数组<br>保存键值对，元素默认按关键字从小到大排列<br>头文件`<map>`
`set` | 关键字即值<br>只保存关键字，元素默认按关键字从小到大排列<br>头文件`<set>`
`multimap` | 关键字可重复出现的map<br>头文件`<map>`
`multiset` | 关键字可重复出现的set<br>头文件`<set>`
`unordered_map` | 用哈希函数组织的无序map<br>头文件`<unordered_map>`
`unordered_set` | 用哈希函数组织的无序set<br>头文件`<unordered_set>`
`unordered_multimap` | 用哈希函数组织的无序的，关键字可重复出现的map <br>头文件`<unordered_map>`
`unordered_multiset` | 用哈希函数组织的无序的，关键字可重复出现的set <br>头文件`<unordered_set>`

<h3 id="associative_container_constructor">关联容器构造</h3>

```c++
// 空容器
map<type1, type2> map_name1;
// 列表初始化
map<type1, type2> map_name2 = { {key1, value1}, {key2, value2}, ... };
// 指定func_comp为map_name3关键字的排序算法
map<type1, type2, decltype(func_comp) *> map_name3(func_comp);
// 空容器
set<type> set_name1;
// 列表初始化
set<type> set_name2 = { key1, key2, ... };
// set_name2包含来自c的不重复元素，且从小到大进行排列
set<type> set_name3(c.cbegin(), c.cend()); 
// 指定func_comp为set_name4关键字的排序算法
set<type, decltype(func_comp)*> set_name4(func_comp);
```

<h3 id="pair_type">pair 类型</h3>

pair 定义在头文件`<utility>`中，保存两个数据成员。

pair 操作 | 说明
--------------------------- | -------------------------------------------------
`p.first` | 返回 p 的名为 first 的公有数据成员<br> 类型别名`first_type`
`p.second` | 返回 p 的名为 second 的公有数据成员<br> 类型别名`second_type`
`pair<T1, T2> p` | 构建一个pair对象<br>对first和second进行值初始化
`pair<T1, T2> p(v1, v2)` | 构建一个pair对象<br>first和second分别使用v1, v2进行初始化
`pair<T1, T2> p = {v1, v2}` | 与`pair<T1, T2> p(v1, v2)`等价
`make_pair(v1,v2)` | 返回一个first和second分别使用v1, v2初始化的pair <br> pair 的类型从v1, v2推断出来
`<,<=,>,>=` | 使用元素运算符进行比较<br>若first不等，则结果为first的比较结果<br>若first相等，则结果为second的比较结果
`==,!=` | 使用元素运算符进行比较<br>当first和second都相等时，两个pair相等

<h3 id="associative_container_operations">关联容器操作</h3>

<h4 id="associative_container_type_alias">类型别名</h4>

关联容器类型别名 | 说明
------------------ | -------------------------------------------------
`key_type` | 关键字类型
`mapped_type` | map 关键字关联的类型
`value_type` | 对于set，与`key_type`相同<br> 对于map，为`pair<const key_type, mapped_type>`

**关键字不能被更改**。

<h4 id="map_class_operations">map 类操作</h4>

向map添加元素：

```c++
word_count.insert({word, 1});
word_count.insert(make_pair(word, 1));
word_count.insert(pair<string, size_t>(word, 1));
word_count.insert(map<string, size_t>::value_type(word, 1));
```

map类的下标操作：

*	**不能对multimap进行下标操作**，因为一个关键字可能对应多个值；
*	对map进行下标操作时，如果关键字没有在map中，那么会创建一个元素插入到map中，并对关联值进行值初始化；
*	**不能对const的map进行下标操作**，因为下标操作可能插入一个新元素。

map下标操作 | 说明
------------------ | -------------------------------------------------
`c[k]` | 返回关键字为k的元素的值<br>若k不在c中，则添加一个关键字为k的元素，并对关联值进行值初始化
`c.at(k)` | 访问关键字为k的元素的值<br>若k不在c中，抛出`out_of_range`异常

<h4 id="common_associative_container_operations">关联容器常用操作</h4>

关联容器常用操作 | 说明
------------------ | -------------------------------------------------
`c.insert(v)` <br> `c.emplace(args)` | v是一个`value_type`类型的对象，args用来构造一个元素<br>返回一个pair，first是一个指向具有给定关键字的元素的迭代器，second指示插入是否成功<br>对于map和set，只有当关键字不在c中时才插入或构造元素<br>**对于multimap和multiset，总会插入或构造元素，并返回一个指向新元素的迭代器**
`c.insert(b, e)` <br> `c.insert(il)` | void函数，插入迭代器范围`[b,e)`内或花括号列表il中的值<br>map和set只插入关键字不存在的值
`c.insert(p, v)` <br> `c.emplace(p, args)` | 迭代器p指出从哪里开始搜索新元素应该存储的位置(因为是有序的)，然而是循环搜索的<br>返回一个迭代器，指向具有给定关键字的元素
`c.erase(k)` | 从c中删除每个关键字为k的元素<br>返回删除元素的数量，类型为`size_type`
`c.erase(p)` | 从c中删除迭代器p指定的元素<br>p必须指向c中一个真实的元素<br>返回一个指向p之后元素的迭代器
`c.erase(b, e)` | 删除迭代器范围`[b,e)`内的元素<br>返回e
`c.find(k)` | 返回一个迭代器，指向第一个关键字为k的元素<br>失败返回`c.end()`<br>**比通用泛型算法find快得多**
`c.count(k)` | 返回关键字为k的元素的数量
`c.lower_bound(k)` | 返回一个迭代器，指向第一个关键字等于k的元素<br>若关键字k不在c中，则与`c.upper_bound(k)`返回相同的迭代器
`c.upper_bound(k)` | 返回一个迭代器，指向最后一个关键字等于k的元素之后的位置<br>若关键字k不在c中，则与`c.lower_bound(k)`返回相同的迭代器
`c.equal_range(k)` | 返回一个迭代器组成的pair，表示关键字等于k的元素的范围，前闭后开<br>失败两个成员都是`c.end()`

<h4 id="unordered_associative_containers">无序关联容器</h4>

C++11 定义了4个无序关联容器: `unordered_map`,`unordered_set`,`unordered_multimap`和`unordered_multiset`。这些容器不使用比较运算符组织元素，而是使用哈希函数(将给定类型的值映射到整形`size_t`值的函数)和关键字的`==`运算符。

可以直接定义关键字为内置类型、string或智能指针类型的无序容器；对于自定义类型，需要提供自定义的hash模板和`==`运算符。

无序关联容器可以使用find,insert等操作，理论上哈希技术会获得更好的平均性能。

**无论是有序关联容器，还是无序关联容器，具有相同关键字的元素都是相邻存储的**。

<h2 id="dynamic_memory">动态内存</h2>

**动态分配的对象的生存期与它们在哪里创建无关，只有当其被显式释放时，这些对象才会被销毁**。

静态内存用来保存static对象和外部变量，栈内存用来保存函数内的非static对象。**动态分配使用栈内存**。

<h3 id="new_and_delete">new, delete</h3>

new: `bad_alloc`和`nothrow`定义于头文件`<new>`中。

```c++
type *name = new type;                  // 默认初始化，失败抛出std::bad_alloc
type *name = new type();                // 值初始化
const type *name = new const type(val); // const对象必须初始化，除非有默认构造函数
type *name = new (nothrow) type;        // 失败返回nullptr
// 动态数组，非数组类型
// 不能对动态数组使用begin或end
// size为0时，返回一个合法的非空指针，但不能解引用
type *name = new type[size];
type *name = new type[size]();
type *name = new type[size]{ val0, val1, ... }; // 剩余值进行值初始化
const type *name = new const type[size](val); 
type *name = new (nothrow) type[size];        
```

delete: 

*	**传递给delete的指针必须指向动态分配的内存，或是一个空指针**；
*	**对同一块内存多次释放，可能会破坏内存空间**；
*	**delete之后，指针仍然指向已经释放的动态内存的地址**；
*	**建议delete之后，赋值为nullptr**；

```c++
int i, *pi1 = &i, *pi2 = nullptr;
double *pd1 = new double(33), *pd2 = pd1;
const int *pci = new const int(123);
delete i;      // 错误
delete pi1;    // 未定义
delete pi2;    // 可以
delete pd1;    // 正确
delete pd2;    // 未定义，已被释放
delete pci;    // 正确
// 释放动态数组
auto *ip = new int[10]();
delete []ip;
```

<h3 id="smart_pointer">智能指针</h3>

智能指针会自动释放(**默认使用delete**)所指向的对象：

*	`shared_ptr`允许多个指针指向同一个对象；
*	`unique_ptr`独占所指向的对象；
*	`weak_ptr`是一种弱引用，指向`shared_ptr`所管理的对象；
*	它们都在头文件`<memory>`中。

`shared_ptr`自动释放的原理：**一旦使用`shared_ptr`管理指针，就不应该用内置指针来访问`shared_ptr`所指向的内存**。

*	当一个`shared_ptr`被用于初始化另一个`shared_ptr`，或将它作为参数传递给一个函数以及作为函数的返回值时，其关联的计数器递增；
*	当一个`shared_ptr`被赋新值或是其被销毁，其关联的计数器会递减；
*	当一个`shared_ptr`所关联的计数器变为0时，就会自动释放其保存的指针。

关于`unique_ptr`:

*	**某个时刻只能有一个`unique_ptr`指向给定的对象**；
*	当`unique_ptr`被销毁时，其所指向的对象也被销毁；
*	**定义一个`unique_ptr`需要将其绑定到一个new返回的指针上，且必须直接初始化**；
*	**`unique_ptr`不支持拷贝和赋值**；
*	**可以拷贝或赋值一个即将被销毁的`unique_ptr`**。

关于`weak_ptr`:

*	不控制所指向对象生存期的智能指针；
*	指向由一个`shared_ptr`管理的对象；
*	**将一个`weak_ptr`绑定到一个`shared_ptr`不会改变`shared_ptr`的引用计数**；
*	**不能使用`weak_ptr`直接访问对象，必须调用lock**。

`shared_ptr` 和 `unique_ptr` 都支持的操作 | 说明
---------------------------------------- | -------------------------------------------------
`shared_ptr<T> sp` <br> `unique_ptr<T> up` | 空智能指针，可以指向类型为T的对象
`p` | 将p用作一个条件判断，保存的指针不为空时为true
`*p` | 解引用，获得p指向的对象
`p->mem` | `(*p).mem`
`p.get()` | 返回p中保存的指针<br>危险，**只有在确定代码不会delete指针的情况下才能使用**
`swap(p, q)` <br> `p.swap(q)` | 交换p和q中的指针

`shared_ptr` 独有的操作 | 说明
------------------------ | -------------------------------------------------
`make_shared<T>(args)` | 返回一个`shared_ptr` <br>指向一个动态分配的类型为T的对象<br>使用args初始化此对象<br>**最安全的分配和使用动态内存的方法**
`shared_ptr<T> p(q)` | p是`shared_ptr` q 的拷贝，递增q中的计数器<br> q中的指针必须能转换为 `T*`
`shared_ptr<T> p(q)` | explicit构造函数<br>p管理内置指针q指向的对象<br> q必须指向new分配的内存，且必须直接初始化<br>q必须能够转换成`T*`类型
`shared_ptr<T> p(u)` | p从`unique_ptr` u 那里接管了对象的所有权，并将u置为空
`shared_ptr<T> p(q, d)` | q可以是内置指针，也可以是`shared_ptr`，但使用可调用对象d代替delete <br>适用于指针指向的资源不是new分配的内存
`p = q` | p, q皆为`shared_ptr`，所保存的指针必须能够相互转换<br>递增q的引用计数，递减p的引用计数
`p.use_count()` | 返回与p共享对象的智能指针数量<br>**很慢，主要用于调试**
`p.unique()` | 若`p.use_count()`为1，返回true

`unique_ptr` 独有的操作 | 说明
----------------------- | -------------------------------------------------
`unique_ptr<T> u1` <br> `unique_ptr<T, D> u2` <br> `unique_ptr<T, D> u3(d)`| 空`unique_ptr`<br> u1使用delete <br> u2使用一个类型为D的可调用对象来释放指针<br> u3使用d释放指针
`unique_ptr<T> u1(new T(val))` <br> `unique_ptr<T, D> u2(new T(val), d)` | 初始化
`unique_ptr<T[]> u` | u指向一个动态分配的数组
`unique_ptr<T[]> u(p)` | u指向内置指针p所指向的一个动态分配的数组<br> p必须能转换为类型`T*`
`u[i]` | 若u指向一个动态数组，返回数组下标为i的对象
`u = nullptr` | 释放u指向的对象，并将u置为空
`u.release()` | u放弃对指针的控制权，并将u置为空<br>返回管理的指针，**需要负责释放**<br>可以将指针所有权转换给另一个`unique_ptr`
`u.reset()` | 释放u指向的对象，并将u置为空
`u.reset(q)` | 释放u指向的对象，并将u指向内置指针q

```c++
unique_ptr<int> p1(new int(10));     // 初始化
unique_ptr<int> p2(p1.release());    // p1的所有权转移给p2，p1置为空
unique_ptr<int> p3(new int(20)); 
p2.reset(p3.release());              // 释放p2，将p3的所有权转移给p2
unique_ptr<int[]> p4(new int[10]()); // 动态数组
p4[1] = 123;
shared_ptr<int> sp(new int[10](), [](int *p){ delete []p; });
*(sp.get() + 1) = 123;               // shared_ptr未定义下标运算符
```

`weak_ptr` 独有的操作 | 说明
------------------------------- | -------------------------------------------------
`weak_ptr<T> w` | 空`weak_ptr`
`weak_ptr<T> w(sp)` | 与`shared_ptr` sp 指向相同的对象<br>T必须能转换为sp指向的类型
`w = p` | p可以是一个`shared_ptr`或`weak_ptr` <br> w和p共享对象
`w.reset()` | 将w置为空
`w.use_count()` | 与w共享对象的`shared_ptr`的数量
`w.expired()` | 若`w.use_count()`为0，返回true
`w.lock()` | 返回一个指向w的对象的`shared_ptr` <br> 若`w.expired()`为true，返回一个空`shared_ptr`

