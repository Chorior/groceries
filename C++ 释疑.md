---
title:      "C++ 释疑"
subtitle:   " 一些代码规范及常见错误 "
date:       2017-03-01 20:00:00 +0800
header-img: "img/C++_logo.jpg"
tags:
    - C++
---

本文是我重温《C++ Primer 5》时，摘录的一些要点，适合初级程序员食用。

#   本文结构

*   [注释选择](#annotation)
*   [无符号类型不能混用](#unsigned_type)
*   [标识符定义](#identifier)
*   [空指针选择](#nullptr)
*   [C 或 C++ 标准库头文件选择](#choose_header)
*   [递增递减选择前置还是后置](#front_or_post)
*   [sizeof 运算符](#sizeof)
*   [C 或 C++ 强制类型转换选择](#forced_conversion)
*	[可变形参函数](#flexible_arguments)
*	[关于返回值](#about_return_value)
*	[关于重载函数、内联函数](#overloaded_and_inline_function)
*	[函数指针](#function_ptr)

<h2 id="annotation">注释选择</h2>

一律使用C++单行注释`//`，因为界定符`/* */`注释不能嵌套。

如果你一定要使用界定符`/* */`的话，建议每行都以一个星号\*开头，用以指出整个范围都是多行注释的一部分。

```C++
#include <iostream>
/*
 * main function
 * print "Hello World!"
 */
int main()
{
	std::cout << "Hello World!\n";
	return 0;
}
```

<h2 id="unsigned_type">无符号类型不能混用</h2>

当一个算数表达式中既有无符号数又有有符号数，且其中的无符号类型不小于带符号类型时，有符号数会被转为无符号数。

特别是当有符号数是一个负数时，转换后的无符号数结果等于这个负数加上对应无符号数的模。

```C++
unsigned u = 10;
int i = -42;
std::cout << i + i << std::endl;   // 84
std::cout << i + u << std::endl;   // maybe 10 + (2^32 - 42) = 4294967264
```

这里还有一个隐式无限循环的例子:

```C++
for(unsigned i = 10; i >= 0; -- i)
{
	std::cout << i << std::endl;
}
```

当有符号数类型大于无符号类型时：

*	如果无符号类型的所有值都能存在该带符号类型中，则无符号数转换成有符号数；
*	如果无符号类型的所有值不能都存在该带符号类型中，则有符号数转换成无符号数。

<h2 id="default_initialization">默认初始化</h2>

如果内置类型的变量未被显式初始化，那么它的值由定义的位置决定。

定义于任何函数体之外的变量被初始化为0，定义于函数体内部的内置类型将不被初始化。

非内置类型的类对象如果没有显式初始化得话，其值决定于该类是否允许隐式初始化（如`std::string`类就允许隐式初始化）。

内置类型包含`bool`,`char`,`wchar_t`,`char16_t`,`char32_t`,`short`,`int`,`long`,`long long`,`float`,`double`,`long double`.

```C++
#include <iostream>
#include <string>          // std::string
#include <cstring>         // wchar_t, char16_t

bool flag;                 // false
wchar_t arr0[10];          // 0 '\0'
char16_t arr1[10];         // 0 u'\0'
int a[10];                 // 0
double d[10];              // 0.00
std::string str;           // ""
int main()
{
	bool flag1;            // true
	wchar_t arr01[10];     // 52428 '쳌'
	char16_t arr11[10];
	int a1[10];
	double d1[10];
	std::string str;       // ""
	getchar();
	return 0;
}
```

只要是内置类型，一律初始化！

<h2 id="identifier">标识符定义</h2>

C++ 的标识符由字符、数字、下划线组成，其中数字不能作为开头。

C++ 为标准库保留了一些名字

*	用户自定义的标识符不能连续出现两个下划线，也不能下划线紧连大写字母开头；
*	定义在函数体外地标识符不能以下划线开头。

### 变量命名规范

*	标识符要体现其实际含义；
*	变量名一般使用小写字母；
*	自定义类名一般使用大写字母开头；
*	多个单词用下划线连接，或者从第二个单词开始首字母大写。

<h2 id="nullptr">空指针选择</h2>

空指针不指向任何对象，在试图使用一个指针时，必须检查它是否为空。

建立空指针有下列三种方法：

```C++
int *pi = 0;
int *p2 = nullptr;
int *p3 = NULL;
```

最好使用nullptr，尽量避免使用NULL。所有指针都必须初始化。

<h2 id="choose_header">C 或 C++ 标准库头文件选择</h2>

C++ 兼容 C 语言标准库，去掉了 C 头文件的后缀`.h`，并添加前缀`c`，表明这是一个 C 语言标准库的头文件。

对应的头文件在内容上是一样的，但是在名为`cname`的头文件中定义的名字从属于命名空间`std`，而`name.h`头文件中的却不然。

C++ 程序一律使用名为`cname`的头文件。

<h2 id="front_or_post">递增递减选择前置还是后置</h2>

前置版本的递增（递减）运算符将值加一（减一）后直接返回改变了的对象；

后置版本的递增（递减）运算符需要将原始值存储下来以便返回这个未修改的内容，然后再加一（减一）。

若无特殊需求，一律使用前缀版本的递增或递减运算符。

<h2 id="sizeof">sizeof 运算符</h2>

sizeof 运算符返回一条表达式或一个类型名所占的字节数。

sizeof 运算符满足右结合律，返回值是一个`size_t`类型的常量表达式。

sizeof 运算符的结果部分依赖于其作用的类型：

*	对char或者类型为char的表达式，结果为1；
*	对引用类型变量，结果为被引用对象所占空间的字节数；
*	对指针，结果为指针本身所占空间的大小；
*	对解引用指针，结果为指针指向的对象所占空间的字节数，**指针不需要有效**；
*	对数组，结果为整个数组所占空间的字节数（**sizeof不会讲数组作为指针处理，指针也不会被作为数组处理**）；
*	对`std::string`对象或`vector`对象，结果只返回该类型固定部分的大小，不会计算对象中的元素占用了多少字节。

```C++
#include <iostream>
#include <string>
#include <vector>

int main()
{
	using namespace std;

	string str("hahah");
	vector<int> vec0( 20, 2);
	vector<string> vec1( 10, "haha" );

	// 40 32 32
	cout << sizeof(string) << " " << sizeof(vector<int>) << " " << sizeof(vector<string>) << endl;
	// 40 32 32
	cout << sizeof(str)    << " " << sizeof(vec0)        << " " << sizeof(vec1) << endl;

	getchar();
	return 0;
}
```

<h2 id="forced_conversion">C 或 C++ 强制类型转换选择</h2>

C++ 强制类型转换包含 `static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`。

`static_cast`: 任何具有明确定义的类型转换，只要不包含底层const，都可以使用`static_cast`）；

```C++
double d = 3.14;
void *p = &d;
double *dp = static_cast<double*>(p);
```

`const_cast`: 只改变运算对象的底层const，不能用于改变类型；

```C++
const char *pc;
char *p = const_cast<char*>(pc);  // 正确，但通过p写值是未定义的行为
static_cast<string>(cp);          // 正确，字符串字面值转换成string类型
const_cast<string>(cp);           // 错误，const_cast 只改变常量属性，不能用于改变类型
```

`reinterpret_cast`: 为运算对象的位模式提供较低层次上的重新解释，这往往会伴随很多未定义的事情发生。

```C++
int *ip;
char *pc = reinterpret_cast<char*>(ip);
```

关于底层const与顶层const: 顶层const表示本身是个常量，而底层const表示指向的对象是个常量。

C 风格的强制类型转换根据涉及的类型，分别具有与`const_cast`, `static_cast`或`reinterpret_cast`相似的行为；但表现形式却不如C++强制类型转换来的清晰明了，一旦转换出现问题，很难追踪。

```text
type(expr);
(type)expr;
```

如非必要情况下，避免使用强制类型转换，如果一定要的话，建议使用C++强制类型转换。

<h2 id="flexible_arguments">可变形参函数</h2>

C++11 新标准提出两种处理可变形参的方法：

*	若实参类型相同，则可以传递一个名为`initializer_list`的标准库类型；
*	若实参类型不同，则需要编写可变函数模板。

这里只介绍`initializer_list`的使用：

`initializer_list`定于于头文件`<initializer_list>`中，包含`size()`,`begin()`,`end()`成员函数。

其所包含元素永远是常量值，不能对其进行更改。

如果要向`initializer_list`形参中传递一个值的序列，则必须把序列放在一对花括号内。

```C++
# include <iostream>
# include <initializer_list>

void print(std::initializer_list<int> il);
int main()
{
    print({ 1,3,4,5,6,7 });
    int a[5]{ 24,234,455,134,441 };
    print({a[0], a[1], a[2], a[3], a[4]});
    getchar();
    return 0;
}

void print(std::initializer_list<int> il)
{
    for (int i : il)
    {
        std::cout << i << std::endl;
    }
}
```

省略符形参是为了便于C++程序访问某些特殊的C代码而设置的，它仅仅用于 C 和 C++ 通用的类型。

**大多数类类型的对象在传递给省略符形参时都无法正确拷贝。**

<h2 id="about_return_value">关于返回值</h2>

强行令void函数返回其他类型的表达式将产生编译错误。

不要返回局部变量的引用或者指针。

除返回引用的函数能得到左值外，其它返回类型的函数都只能得到右值。

**C++11 新标准规定，函数可以返回花括号包围的值的列表，这个列表用来对函数返回的临时量进行初始化。若列表为空，则对临时量执行值初始化。**

值初始化：若元素类型为内置类型，则初始化为0；若为其它类类型，则元素由类进行默认初始化。

```C++
std::vector<std::string> process(int i)
{
    if (0 == i)     return{};
    else if (0 < i) return{ "i", "is", "bigger" ,"than", "0" };
    else if (0 > i) return{ "i", "is", "smaller","than", "0" };
}
```

C++11 支持尾置返回类型：

```C++
auto func(int i) -> int(*)[10];   // 返回值是一个指向包含10个int元素的数组的指针
```

### main 函数的返回值

如果 main 函数运行到了结尾却没有return语句，那么编译器将隐式的插入一条`return 0;`。

main 函数的返回值可以看做是状态指示器，返回0表示执行成功，非零值的具体含义视机器而定。

为了使返回值与机器无关，`<cstdlib>`头文件定义了两个预处理变量`EXIT_SUCCESS`和`EXIT_FAILURE`，用于表示成功和失败。

<h2 id="overloaded_and_inline_function">关于重载函数、内联函数</h2>

### 重载函数

函数重载： 如果同一作用域的几个函数**名字相同但形参列表不同**，则称他们为重载函数。

main 函数不能重载。

如果在内层作用域中声明了与外层作用域相同的函数名字，那么外层作用域中同名的全部函数将被隐藏。

C++ 中，名字查找发生在类型检查之前:

```C++
int read();

int main()
{
    bool read = false;
    int i = read();     // 错误！read是一个布尔值，不是一个函数。
    return 0;
}
```

### 内联函数

一次函数调用包含一系列工作：

*	调用前要先保存寄存器，并在返回时恢复；
*	可能需要拷贝实参；
*	程序转向一个新的位置继续执行。

内联函数可以避免函数调用的开销：调用内联函数的地方在编译过程中会被展开。

**内联函数只用于规模较小、流程直接、调用频繁的函数。**

<h2 id="help_debug">调试帮助</h2>

`assert(expr)`：如果`expr`为真，什么也不做；否则输出相关信息并终止程序的执行。

头文件`<cassert>`，如果**在包含头文件之前**定义了`NDEBUG`宏，那么assert什么也不做。

5个预处理器定义的、对于程序调试很有用的`const char`静态数组名：

*	`__func__`: 当前函数名；
*	`__FILE__`: 当前文件名；
*	`__LINE__`: 当前行号；
*	`__TIME__`: 文件编译时间；
*	`__DATE__`: 文件编译日期。

<h2 id="function_ptr">函数指针</h2>

当把函数名作为一个值使用时，该函数自动地转换成指针：

```C++
bool func(int, int);
bool (*pf)(int, int) = func;
bool (*pf)(int, int) = &func; // 取地址符是可选的
```

可以直接使用指向函数的指针调用该函数：

```C++
bool b1 = pf(1,2);
bool b2 = (*pf)(1,2); // 与上面等价
```

当函数指针用作形参时，函数类型会被当做指针使用：

```C++
void example(int i, bool pf(int, int));
void example(int i, bool (*pf)(int, int)); // 与上面等价
```

返回指向函数的指针：

```C++
int (*f1(int))(int*, int);          // 由内向外，返回一个指向函数的指针
auto f1(int) -> int (*)(int*, int);
```

decltype 作用于某个函数时，它返回函数类型而非指针。
