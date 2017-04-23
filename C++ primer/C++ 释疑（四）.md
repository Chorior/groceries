---
title:      "C++ 释疑（四）"
subtitle:   "一些可能用不到的高级功能"
date:       2017-04-21 14:28:00 +0800
header-img: "img/sunset_beach.jpg"
tags:
    - C++
    - C++ primer
---

本文是我重温《C++ Primer 5》第四部分高级主题时，摘录的一些要点，适合初级程序员食用。

#   本文结构

*   [tuple](#tuple)
*   [正则表达式](#regular_expression)
*   [bitset与随机数](#bitset_and_random_number)
*   [异常处理](#exception)
*   [枚举类型](#enum_type)
*   [其它](#others)

<h2 id="tuple">tuple</h2>

tuple是类似pair的模板，但一个tuple可以有任意数量的成员，每个成员都是public的。

当希望将一些数据组合成单一对象，但又不想新定义一个数据结构时，可以使用tuple。

tuple定义于`<tuple>`，由于tuple定义了<和==运算符，所以可以在无序容器总将tuple作为关键字类型。

tuple支持的操作 | 说明
-------------------------------------- | -------------------------------------------------
`tuple<T1,T2,...,Tn> t` | t是一个tuple，成员数为n，第i个成员的类型时Ti <br> 所有成员进行值初始化
`tuple<T1,T2,...,Tn> t(v1,v2,...,vn)` | explicit构造函数<br>每个成员使用对应的vi进行初始化
`make_tuple(v1,v2,...,vn)` | 类似`make_pair`
`t1 == t2`<br>`t1 != t2` | 当两个tuple成员数量相等且成员对应相等时，两个tuple相等<br>一旦找到不等的成员，后续成员不再比较
`t1 relop t2` | 两个tuple的关系运算符采用字典序，即类似于string的比较<br>但他们的成员数量必须相等<br>使用<运算符进行比较
`get<i>(t)` | 返回t的第i个数据成员的引用
`tuple_size<tupleType>::value` | 一个类模板，value类型为`size_t` <br>表示给定tuple类型中成员的数量
`tuple_element<i,tupleType>::type` | 一个类模板，type表示给定tuple类型中第i个成员(从0开始)的类型

```c++
tuple<int, vector<int>, bool> test{ 1,{1,2,3},true };
typedef decltype(test) trans;
size_t size = tuple_size<trans>::value;         // size = 3
tuple_element<1, trans>::type iv = get<1>(test);// v1 = {1,2,3}
auto flag = get<2>(test);                       // flag = true

auto test2 = make_tuple("hahah", 212, 20.00);   // const char*, int, double
tuple_element<0, decltype(test2)>::type x;
tuple_element<1, decltype(test2)>::type y;
tuple_element<2, decltype(test2)>::type z;
tie(x,y,z) = test2;                             // 适用于pair或tuple，创建一个参数的左值引用的tuple
std::cout << x << " " << y << " " << z << std::endl;
```

<h2 id="regular_expression">正则表达式</h2>

正则表达式是一种描述字符序列的方法，是一种强大的计算工具。但**正则表达式的编译时非常慢的操作，避免创建不必要的regex**。

C++正则表达式库简称RE库(regular)，定义于头文件`<regular>`中。

**一个正则表达式的语法是否正确是在运行时解析的**。

RE库组件 | 说明
------------------ | -------------------------------------------------
`regex` | 表示一个正则表达式的类
`regex_match` | 将一个字符序列与一个正则表达式匹配
`regex_search` | 寻找第一个与正则表达式匹配的子序列
`regex_replace` | 使用给定格式替换一个正则表达式
`sregex_iterator` | 迭代器适配器<br>调用`regex_search`来遍历一个string中所有匹配的子串
`smatch` | 容器类<br>保存在string中搜索的结果
`ssub_match` | string中匹配的子表达式的结果

regex(和wregex)选项 | 说明（构造函数或赋值可能会抛出`regex_error`异常）
------------------ | -------------------------------------------------
`regex r(re)` <br> `regex r(re,f)` | re表示一个正则表达式<br>re可以是string，表示字符范围的迭代器对，指向空字符结尾的字符数组的指针，一个字符指针和一个计数器，或者一个花括号包围的字符列表<br> f指定[正则表达式标志](#regular_flag)，默认ECMAScript
`r1 = re` | 将ri中的正则表达式替换为re <br> re可以是另一个regex对象、一个string、一个指向空字符结尾的字符数组的指针、或是一个花括号包围的字符列表
`r1.assign(re,f)` | 与使用赋值运算符相同
`r.mark_count()` | r中子表达式的数目
`r.flags()` | 返回r的标志集

`regex_search` 和 `regex_match` | 说明
------------------ | -------------------------------------------------
`bool(seq,m,r,mft)` <br> `bool(seq,r,mft)` | 在字符序列seq中查找regex对象r中的正则表达式<br> seq可以是string，一对迭代器，或者一个指向空字符结尾的字符数组的指针<br>m是一个smatch对象，用来保存匹配结果<br>m与seq必须具有兼容的类型<br>mft是一个可选的`regex_constants::match_flag_type`值，影响匹配过程

输入类型 | 正则表达式类
------------------ | -------------------------------------------------
string | regex,smatch,ssub_match,sregex_iterator
const char* | regex,cmatch,csub_match,cregex_iterator
wstring | wregex,wsmatch,wssub_match,wsregex_iterator
const wchar_t* | wregex,wcmatch,wcsub_match,wcregex_iterator

<h4 id="regular_flag">正则表达式标志</h4>

正则表达式标志 | 说明
------------------ | -------------------------------------------------
`icase` | 匹配过程中忽略大小写
`nosubs` | 不保存匹配的子表达式
`optimize` | 执行速度优于构造速度
`ECMAScript` | 使用ECMA-262指定的语法
`basic` | 使用posix基本的正则表达式语法
`extended` | 使用posix扩展的正则表达式语法
`awk` | 使用posix版本的awk语法
`grep` | 使用posix版本的grep语法
`egrep` | 使用posix版本的egrep语法

<h2 id="bitset_and_random_number">bitset与随机数</h2>

bitset类定义于`<bitset>`中，能够处理超过最长整形类型大小的位集合。

**C++程序不应该使用库函数rand，而应使用`default_random_engine`类和恰当的分布类对象(`<random>`)**。

随机数发生器是指分布对象与引擎对象的组合。

```c++
uniform_int_distribution<unsigned> u(0, 9);
default_random_engine e;
for (int i = 0; i < 10; ++i) {
	cout << u(e) << " ";
}
```

<h2 id="exception">异常处理</h2>

异常使得我们能够将问题的检测与解决过程分离开来。程序的一部分负责检测问题的出现，另一部分则负责解决该问题。检测环节无须知道问题处理环节的所有细节，反之亦然。

C++ 通过抛出一个表达式来引发一个异常。被选中的处理代码是调用链中与抛出对象类型匹配的最近的处理代码。

**当执行一个throw时，其后的语句将不再执行，该作用域创建的局部对象将被销毁**。

当throw出现在一个try语句块内时，

*   若通过**栈展开**找到了一个最近的匹配的catch子句，则程序进入该子句并执行其中的代码，然后找到与try块关联的最后一个catch子句之后的点，并从这里继续执行；
*   若没有找到匹配的catch子句，程序将调用标准库函数terminate终止程序。

**在搜寻catch语句的过程中，最终找到的catch未必是最佳匹配，而是第一个与异常匹配的catch语句**。

为了确保资源的正确释放，**若析构函数需要执行某个可能抛出异常的操作，则该操作应该被放置在一个try语句块中，并且在析构函数内部得到处理**，即**析构函数不应该抛出异常**。

编译器使用异常抛出表达式来**对异常对象进行拷贝初始化**，异常对象位于编译器管理的空间中，任何catch子句都能访问该空间。

**抛出表达式的静态类型决定了异常对象的类型**，即若一条throw表达式解引用一个基类指针，而该指针实际指向的是继承类对象，则抛出的对象将被切掉一部分。

一条catch语句可以通过重新抛出(一条空throw语句)的操作将异常向上传递给另外一个catch语句。**空throw出现在处理代码之外的区域将会执行terminate**。

`catch(...)`将捕获所有异常。

**构造函数体内的catch语句无法处理构造函数初始值列表抛出的异常**，但可以通过函数try语句块来处理。

```c++
Solution(int a, bool b) try: i(a),f(b)
{
    // do some initialize
}
catch (...) {
    // handle exception
}
```

C++11 允许提供noexcept说明(早期使用throw())指定某个函数不会抛出异常。**知道不会抛出异常将有助于编译器进行优化**。

noexcept要么出现在函数的所有声明和定义中，要么一次也不出现。noexcept出现在函数参数列表之后，const及引用限定符之后，尾置返回类型、final、override或虚函数的=0之前。

**一旦一个noexcept函数跑出了异常，程序将执行terminate**。

noexcept可以用在两种情况下：

*   确认函数不会抛出异常；
*   不知道如何处理异常。

exception类定义于`<exception>`中，`bad_alloc`,`bad_cast`,`runtime_error`,`logic_error`继承自exception，`overflow_error`,`underflow_error`,`range_error`继承自`runtime_error`，`domain_error`,`invalid_argument`,`out_of_range`,`length_error`继承自`logic_error`。

exception类定义了一个名为what的虚成员，返回一个以空字符结尾的`const char*`字符串数组。

我们也可以通过继承，来实现自己的异常类型。

<h2 id="enum_type">枚举类型</h2>

枚举类型将一组整形常量组织在一起。枚举属于字面常量类型。

*   枚举值默认从0开始，然后依次加一；
*   **若显式指定，则枚举值可以不唯一**；
*   **枚举成员是const，初始化枚举成员时必须使用常量表达式**；
*   **要向初始化或赋值一个enum对象，必须使用该类型的一个枚举成员或该类型的另一个对象**；
*   **不限定作用域的枚举类型的对象或枚举成员自动的转换成整形，但不能使用整形为枚举对象初始化或赋值**；
*   可以在enum的名字后加上冒号以及我们想在该enum中使用的类型，默认情况下限定作用域的enum成员类型为int，不限定作用域的enum不存在默认类型，但保证类型足够大；
*   一旦某个enum成员超出该类型所能容纳的范围将引发程序错误，但**控制enum类型可以使得不同环境生成的代码一致**；

```c++
// 限定作用域的枚举类型
enum class open_modes { input, ouput, append };
// 不限定作用域的枚举类型
enum color { red, yellow, green };
enum custom : unsigned { white = 0, black = 2, blue = 2 };
enum {hsda,jdad,kwq}unnamed;

color r = color::red;
color g = green;
open_modes in = open_modes::input;
int i = r;
```

<h2 id="others">其它</h2>

定位new允许在一个特定的、预先分配的内存地址上构造对象。

**联合union可以有多个数据成员，但是任意时刻只有一个数据成员可以有值，其他成员为未定义状态。一个union对象的存储空间至少能容纳它的最大数据成员**。

**当对象的值可能在程序的控制或检测之外被改变时(比如硬件)，应该讲对象声明为volatile，告诉编译器不应对这样的对象进行优化**。

运行时类型识别(RTTI)：

*   typeid运算符，返回表达式的类型；
*   `dynamic_cast`运算符，用于将基类指针或引用安全的转换成派生类的指针或引用。

当需要调用其他语言编写的函数，或者令一个C++函数在其他语言编写的程序中可用时，需要使用链接指示指出任意非C++函数所用的语言。

*   链接指示包含一个关键字`extern`，后跟一个指示编写函数所用的语言(必须与C++兼容)的字符串常量，以及函数声明；
*   链接指示不仅对函数有效，而且对返回类型或形参类型的函数指针也有效；

```c++
extern "C" size_t strlen(const char*);
extern "C" void f1(void(*p)(int)); // p: C pointer
extern "C"{
    int strcmp(const char*, const char*);
    char *strcat(char*, const char*);
}

void (*pf1)(int);            // C++ pointer
extern "C" void (*pf2)(int); // C pointer
pf1 = pf2;                   // error
pf2 = pf1;                   // error
```

有时需要在C和C++中编译同一个源文件，可以使用如下方法：

```c++
#ifdef __cplusplus
// compile C++ program
extern "C"
#endif
// compile C program
```