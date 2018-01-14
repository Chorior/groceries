---
title:      "C++ 释疑（一）"
subtitle:   " 一些常见错误规避方法 "
date:       2017-03-01 20:00:00 +0800
header-img: "img/C++_logo.jpg"
tags:
    - C++
    - C++ primer
---

本文是我重温《C++ Primer 5》第一部分 C++ 基础时，摘录的一些要点，适合初级程序员食用。

#   本文结构

*   [注释选择](#annotation)
*   [无符号类型不能混用](#unsigned_type)
*   [标识符定义](#identifier)
*   [空指针选择](#nullptr)
*   [C 或 C++ 标准库头文件选择](#choose_header)
*   [递增递减选择前置还是后置](#front_or_post)
*   [sizeof 运算符](#sizeof)
*   [C 或 C++ 强制类型转换选择](#forced_conversion)
*	[尽量使用常量引用传递参数](#constant_reference)
*	[数组形参传递](#array_params)
    *   [三种确保数组不越界的函数定义方法](#protect_array_params)
*	[可变形参函数](#flexible_arguments)
*	[关于返回值](#about_return_value)
*	[关于重载函数、内联函数](#overloaded_and_inline_function)
*	[函数指针](#function_ptr)
*   [类](#class)
    *   [this 指针](#this_ptr)
    *   [构造函数](#constructor)
    *   [拷贝、赋值和析构](#copy_assign_and_destructor)
    *   [访问控制符](#access_control)
    *   [类的静态成员](#static_class_members)

<h2 id="annotation">注释选择</h2>

**一律使用C++单行注释`//`，因为界定符`/* */`注释不能嵌套**。

如果你一定要使用界定符`/* */`的话，建议每行都以一个星号\*开头，用以指出整个范围都是多行注释的一部分。

```c++
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

特别是当有符号数是一个负数时，**转换后的无符号数结果等于这个负数加上对应无符号数的模**。

```c++
unsigned u = 10;
int i = -42;
std::cout << i + i << std::endl;   // 84
std::cout << i + u << std::endl;   // maybe 10 + (2^32 - 42) = 4294967264
```

这里还有一个隐式无限循环的例子:

```c++
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

**定义于任何函数体之外的内置类型被初始化为0，定义于函数体内部的内置类型将不被初始化**。

非内置类型的类对象如果没有显式初始化得话，其值决定于该类是否允许隐式初始化（如`std::string`类就允许隐式初始化）。

内置类型包含`bool`,`char`,`wchar_t`,`char16_t`,`char32_t`,`short`,`int`,`long`,`long long`,`float`,`double`,`long double`.

```c++
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

**只要是内置类型，一律初始化！**

<h2 id="identifier">标识符定义</h2>

**C++ 的标识符由字符、数字、下划线组成，其中数字不能作为开头**。

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

```c++
int *pi = 0;
int *p2 = nullptr;
int *p3 = NULL;
```

**最好使用nullptr，尽量避免使用NULL。所有指针都必须初始化**。

<h2 id="choose_header">C 或 C++ 标准库头文件选择</h2>

C++ 兼容 C 语言标准库，去掉了 C 头文件的后缀`.h`，并添加前缀`c`，表明这是一个 C 语言标准库的头文件。

对应的头文件在内容上是一样的，但是在名为`cname`的头文件中定义的名字从属于命名空间`std`，而`name.h`头文件中的却不然。

**C++ 程序一律使用名为`cname`的头文件**。

<h2 id="front_or_post">递增递减选择前置还是后置</h2>

前置版本的递增（递减）运算符将值加一（减一）后直接返回改变了的对象；

后置版本的递增（递减）运算符需要将原始值存储下来以便返回这个未修改的内容，然后再加一（减一）。

**若无特殊需求，一律使用前缀版本的递增或递减运算符**。

<h2 id="sizeof">sizeof 运算符</h2>

sizeof 运算符返回一条表达式或一个类型名所占的字节数。

sizeof 运算符满足右结合律，返回值是一个`size_t`类型的常量表达式。

sizeof 运算符的结果部分依赖于其作用的类型：

*	对char或者类型为char的表达式，结果为1；
*	对引用类型变量，结果为被引用对象所占空间的字节数；
*	对指针，结果为指针本身所占空间的大小；
*	对解引用指针，结果为指针指向的对象所占空间的字节数，**指针不需要有效**；
*	对数组，结果为整个数组所占空间的字节数（**sizeof不会将数组作为指针处理，指针也不会被作为数组处理**）；
*	对`std::string`对象或`vector`对象，结果只返回该类型固定部分的大小，不会计算对象中的元素占用了多少字节。

```c++
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

```c++
double d = 3.14;
void *p = &d;
double *dp = static_cast<double*>(p);
```

`const_cast`: 只改变运算对象的底层const，不能用于改变类型；

```c++
const char *pc;
char *p = const_cast<char*>(pc);  // 正确，但通过p写值是未定义的行为
static_cast<string>(pc);          // 正确，字符串字面值转换成string类型
const_cast<string>(pc);           // 错误，const_cast 只改变常量属性，不能用于改变类型
```

`reinterpret_cast`: 为运算对象的位模式提供较低层次上的重新解释，这往往会伴随很多未定义的事情发生。

```c++
int *ip;
char *pc = reinterpret_cast<char*>(ip);
```

关于底层const与顶层const: 顶层const表示本身是个常量，而底层const表示指向的对象是个常量。

C 风格的强制类型转换根据涉及的类型，分别具有与`const_cast`, `static_cast`或`reinterpret_cast`相似的行为；但表现形式却不如C++强制类型转换来的清晰明了，一旦转换出现问题，很难追踪。

```text
type(expr);
(type)expr;
```

**如非必要情况下，避免使用强制类型转换，如果一定要的话，建议使用 C++ 强制类型转换**。

<h2 id="constant_reference">尽量使用常量引用传递参数</h2>

拷贝大的类类型对象或者容器对象比较低效，甚至有的类类型根本就不支持拷贝操作，这时候应该使用引用传递方式传递参数。

如果函数无须改变引用形参的值，那么最好将其声明为常量引用。因为我们可以将一个非常量对象传递给常量形参，但是常量对象却不能传递给非常量形参。

如果你强行将一个常量对象转成非常量对象，然后再传递给非常量形参的话，万一程序运行过程中出现什么异常改变了常量对象，这就会造成非常严重的问题。

```c++
void func0(char*);
void func1(const char*);

std::string str("test");
func0(&str[0]);                        // OK
func0(const_cast<char*>(str.c_str())); // OK
func1(str.c_str());                    // 很安全

const char* cstr = "hahah";
func0(const_cast<char*>(cstr));        // 很危险
func1(cstr);                           // 很安全
```

**尽量使用常量引用传递参数**。

<h2 id="array_params">数组形参传递</h2>

数组不允许拷贝，所以无法使用值传递方式传递数组参数。

以下三个函数声明都是等价的：

```c++
void print(const int*);    // 一般拉沙
void print(const int[]);   // 使函数意图更加明显
void print(const int[10]); // 这里的维度表示期望传递的数组包含10个元素，实际不一定
```

<h3 id="protect_array_params">三种确保数组不越界的函数定义方法</h3>

#### 使用标记指定数组长度

数组自身包含一个结束符：

```c++
void print(const char *cp)
{
    if(cp) while(*cp) std::cout << *cp++; // 输出直到遇到空字符
}
```

#### 使用标准库规范

传递首元素和尾后元素指针：

```c++
void print(const int *begin, const int *end)
{
    while(begin != end) std::cout << *begin++ << std::endl;
}

int a[3]{ 0, 2, 3};
print(std::begin(a), std::end(a));
```

#### 显式传递一个表示数组大小的形参

```c++
void print(const int ia[], std::size_t size)
{
    for(std::size_t i = 0; i != size; ++ i)
    {
        std::cout << ia[i] << std::endl;
    }
}
```

### 数组引用形参

```c++
void print(int (&arr)[10])       // arr 是具有10个整数的整形数组的引用
{
    for(auto elem : arr) std::cout << elem << std::endl;
}

int i = 0, j[2]{0, 1};
int k[10]{0,1,2,3,4,5,6,7,8,9};
print(&i);                       // 错误： 实参必须是含有10个整数的数组
print(j);                        // 错误
print(k);                        // 正确
```

### 多维数组形参

**多维数组第一维大小会被忽略，第二维以及后面所有维度的大小都是数组类型的一部分，不能省略**：

```c++
void print(int matrix[][10], int row_size){ /* ... */ }
```

### 复杂数组的识别方法

由内而外，由右向左。

如`int *matrix[10]`，先看最右边`[10]`，表示含有10个元素的数组，再看`*`，表示元素是指针，所以`int *matrix[10]`表示一个包含10个int型指针的数组；

再看`int (*matrix)[10]`，先看最里面`*`，表示matrix是个指针，再看最右面`[10]`，表示包含10个元素的数组，再看最左边`int`，表示元素是整型，所以`int (*matrix)[10]`表示一个指向包含10个int对象的数组的指针。

<h2 id="flexible_arguments">可变形参函数</h2>

C++11 新标准提出两种处理可变形参的方法：

*	若实参类型相同，则可以传递一个名为`initializer_list`的标准库类型；
*	若实参类型不同，则需要编写可变函数模板。

这里只介绍`initializer_list`的使用：

`initializer_list`定于于头文件`<initializer_list>`中，包含`size()`,`begin()`,`end()`成员函数。

其所包含元素永远是常量值，不能对其进行更改。

如果要向`initializer_list`形参中传递一个值的序列，则必须把序列放在一对花括号内。

```c++
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
    for (const int &i : il)
    {
        std::cout << i << std::endl;
    }
}
```

省略符形参是为了便于C++程序访问某些特殊的C代码而设置的，它仅仅用于 C 和 C++ 通用的类型。

**大多数类类型的对象在传递给省略符形参时都无法正确拷贝。**

<h2 id="about_return_value">关于返回值</h2>

强行令void函数返回其他类型的表达式将产生编译错误。

**不要返回局部变量的引用或者指针**。

除返回引用的函数能得到左值外，其它返回类型的函数都只能得到右值。

**C++11 新标准规定，函数可以返回花括号包围的值的列表，这个列表用来对函数返回的临时量进行初始化。若列表为空，则对临时量执行值初始化。**

值初始化：若元素类型为内置类型，则初始化为0；若为其它类类型，则元素由类进行默认初始化。

```c++
std::vector<std::string> process(int i)
{
    if (0 == i)     return{};
    else if (0 < i) return{ "i", "is", "bigger" ,"than", "0" };
    else if (0 > i) return{ "i", "is", "smaller","than", "0" };
}
```

C++11 支持尾置返回类型：

```c++
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

C++ 中，**名字查找发生在类型检查之前**:

```c++
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

**当把函数名作为一个值使用时，该函数自动地转换成指针**：

```c++
bool func(int, int);
bool (*pf)(int, int) = func;
bool (*pf)(int, int) = &func; // 取地址符是可选的
auto pf = func;               // pf 是一个指针
decltype(print) pf;           // pf 是一个函数类型
decltype(print) *pf = func;   // pf 是一个指针
```

可以直接使用指向函数的指针调用该函数：

```c++
bool b1 = pf(1,2);
bool b2 = (*pf)(1,2); // 与上面等价
```

当函数指针用作形参时，函数类型会被当做指针使用：

```c++
void example(int i, bool pf(int, int));
void example(int i, bool (*pf)(int, int)); // 与上面等价
```

返回指向函数的指针：

```c++
int (*f1(int))(int*, int);          // 由内向外，返回一个指向函数的指针
auto f1(int) -> int (*)(int*, int);
```

**decltype 作用于某个函数时，它返回函数类型而非指针**。

<h2 id="class">类</h2>

定义在类内部的函数都是隐式内联的。

编译器分两步处理类：**首先编译成员声明，然后再到成员函数体**。所以成员函数相互间使用时，不用在意成员出现的顺序。

一个用关键字`mutable`声明的数据成员永远也不会是const，即使它是const对象的成员。

<h3 id="this_ptr">this 指针</h3>

每个类的成员函数都有一个名为this的隐式参数，它是一个常量指针，且指向当前对象。

默认情况下，this的类型是`class_name *const`，即this指向非常量版本的类对象；若类对象是常量，那么该对象不能调用普通的成员函数。

```c++
# include <iostream>
class TestClass
{
    int a = 0;            // C++ 11
    int b = 1;            // C++ 11
public:
    void print()
    {
        std::cout << this->a << std::endl;
        std::cout << this->b << std::endl;
    }
};

int main()
{
    const TestClass test; // 使用合成的默认构造函数
    test.print();         // 错误！const对象不能调用普通成员函数print！
    getchar();
    return 0;
}
```

**常量类对象只能调用常量成员函数**。

常量成员函数即在函数声明和定义的参数列表后加上const的普通成员函数，跟在参数列表后面的const表示this是一个指向常量的指针，所以常量成员函数不能改变调用它的对象的内容。

```c++
# include <iostream>
class TestClass
{
    int a = 0;            // C++ 11
    int b = 1;            // C++ 11
public:
    void print() const;
};

int main()
{
    const TestClass test; // 使用合成的默认构造函数
    test.print();         // OK
    TestClass test2;
    test2.print();        // OK
    getchar();
    return 0;
}

void TestClass::print() const
{
    std::cout << this->a << std::endl;
    std::cout << this->b << std::endl;
}
```

<h3 id="constructor">构造函数</h3>

构造函数初始化类对象的数据成员，只要类的对象被创建，就会执行构造函数。

**建议每个类都显式的定义一个默认构造函数**。

#### 合成的默认构造函数

**如果类没有显式的定义构造函数，那么编译器就会为我们隐式的定义一个默认构造函数**，即合成的默认构造函数。

合成的默认构造函数按如下规则初始化类的数据成员：

*   如果存在类内的初始值，那么用它来初始化成员；
*   否则，[默认初始化](#default_initialization)该成员。

C++11 可以通过在参数列表后面写上`= default`来要求编译器生成构造函数。`= default`既可以和声明一起出现在类的内部，也可以作为定义出现在类的外部。

*   如果`= default`出现在类的内部，那么默认构造函数是内联的；
*   如果`= default`出现在类的外部，那么默认情况下不是内联的。

#### 构造函数初始值列表

下面构造函数参数列表后的冒号以及冒号和花括号间的代码即为构造函数初始值列表:

```c++
class TestClass
{
    int a = 0;  // C++ 11
    int b = 2;  // C++ 11
public:
    TestClass(int ia, int ib): a(ia),b(ib) {}
};
```

没有出现在构造函数初始值列表中的成员将通过相应的类内初始值（如果存在的话）初始化，或者[默认初始化](#default_initialization)。

```c++
class TestClass
{
    int a = 0;  // C++ 11
    int b = 2;  // C++ 11
public:
    TestClass(int ia, int ib) 
    {
        std::cout << a << "," << b << std::endl;  // 0, 2
        a = ia; b = ib;                           // 赋值
    }
};
```

**成员初始化顺序与它们在类定义中出现的顺序一致，所以不用太在意构造函数初始值列表初始值出现的顺序。**

```c++
class TestClass
{
    int a = 0;  // C++ 11
    int b = 2;  // C++ 11
public:
    TestClass(int ia, int ib): b(ib),a(b) {}      // 错误！a先被初始化！
};
```

#### 委托构造函数

委托构造函数使用它所属类的其他构造函数执行它自己的初始化。

```c++
# include <iostream>
class TestClass
{
    int a;
    int b;
public:
    TestClass(int ia, int ib) : a(ia), b(ib) { std::cout << 2 << std::endl; }
    TestClass(int ia) :TestClass(ia, 0) { std::cout << 1 << std::endl; }
    TestClass() :TestClass(0) { std::cout << 0 << std::endl; }
};

int main()
{
    TestClass test;                               // print 2,1,0
    getchar();
    return 0;
}
```

#### 转换构造函数

当构造函数参数只接受一个实参时（有默认值，或真的只有一个参数），它实际上定义了转换为此类类型的隐式转换机制：

```c++
# include <iostream>
class TestClass
{
    int a;
    int b;
public:
    TestClass(int ia, int ib = 0) : a(ia), b(ib) {}
};

int main()
{
    TestClass test = 2;                           // 隐式转换
    getchar();
    return 0;
}
```

**使用关键字`explicit`可以阻止隐式类型转换。`explicit`只对一个实参的构造函数有效，且`explicit`只能出现在类内的声明处**。

<h3 id="copy_assign_and_destructor">拷贝、赋值和析构</h3>

**如果不主动定义拷贝、赋值和析构操作的话，编译器将生成相应的版本对对象的每个成员执行拷贝、赋值和销毁操作**。

国内著名 C++ 博主陈硕曾经说过：
> 知道禁用 copy-ctor/assign operator 是 C++ 程序员的试金石

因为一般来说，对类对象做副本没有什么意义，所以如果**确实不需要拷贝和赋值的话**，需要限制编译器自动生成的拷贝构造函数和赋值构造函数。

可以参照<http://www.boost.org/doc/libs/1_63_0/boost/core/noncopyable.hpp>，对其进行派生，从而禁用拷贝和赋值操作。

```c++
//  Boost noncopyable.hpp header file  --------------------------------------//

//  (C) Copyright Beman Dawes 1999-2003. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/utility for documentation.

#ifndef BOOST_CORE_NONCOPYABLE_HPP
#define BOOST_CORE_NONCOPYABLE_HPP

#include <boost/config.hpp>

namespace boost {

//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

//  Contributed by Dave Abrahams

namespace noncopyable_  // protection from unintended ADL
{
  class noncopyable
  {
  protected:
#if !defined(BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      BOOST_CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
#else
  private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };
}

typedef noncopyable_::noncopyable noncopyable;

} // namespace boost

#endif  // BOOST_CORE_NONCOPYABLE_HPP
```

<h3 id="access_control">访问控制符</h3>

`public`, `protected`和`private`被称为访问说明符，用于说明接下来的成员访问级别，其有效范围直到出现下一个访问说明符或者到达类的结尾为止。

一个类可以包含0个或多个访问说明符。

`class`和`struct`都可以定义一个类，唯一的区别是`struct`的默认访问权限是`public`，而`class`的是`private`。

<h3 id="static_class_members">类的静态成员</h3>

**类的静态成员存在于任何对象之外**，对象中不包含任何与静态数据成员有关的数据。

**静态成员函数也不与任何对象绑定在一起，它们不包含this指针，所以静态成员函数也不能设成const的**。

#### 定义静态成员

`static`与`explicit`一样，只能出现在类内部的声明语句处。

必须在类的外部定义和初始化每个静态成员。

一个静态数据成员只能定义一次。

**可以使用静态成员作为默认实参**。

#### 使用静态成员

```c++
# include <iostream>
# include <string>
class TestClass
{
    std::string str;
    int c;
public:
    explicit TestClass(std::string s) : str(s),c(a) {}
    static void print();
    static int a;
    static int b;
};

int main()
{
    TestClass test("hahah");
    test.print();
    TestClass::print();
    TestClass::a = 41;
    getchar();
    return 0;
}

int TestClass::a = 3;
int TestClass::b = 4;

void TestClass::print()
{
    std::cout << a << "," << b << std::endl;
}
```
