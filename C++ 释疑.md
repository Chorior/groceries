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
*   [C或C++标准库头文件选择](#choose_header)
*   [递增递减选择前置还是后置](#front_or_post)
*   [sizeof运算符](#sizeof)

<h2 id="annotation">注释</h2>

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
#include <string>              // std::string
#include <cstring>             // wchar_t, char16_t

bool flag;                     // false
wchar_t arr0[10];              // 0 '\0'
char16_t arr1[10];             // 0 u'\0'
int a[10];                     // 0
double d[10];                  // 0.00
std::string str;               // ""
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

<h2 id="identifier">标识符</h2>

C++ 的标识符由字符、数字、下划线组成，其中数字不能作为开头。

C++ 为标准库保留了一些名字

*	用户自定义的标识符不能连续出现两个下划线，也不能下划线紧连大写字母开头；
*	定义在函数体外地标识符不能以下划线开头。

### 变量命名规范

*	标识符要体现其实际含义；
*	变量名一般使用小写字母；
*	自定义类名一般使用大写字母开头；
*	多个单词用下划线连接，或者从第二个单词开始首字母大写。

<h2 id="nullptr">空指针</h2>

空指针不指向任何对象，在试图使用一个指针时，必须检查它是否为空。

建立空指针有下列三种方法：

```C++
int *pi = 0;
int *p2 = nullptr;
int *p3 = NULL;
```

最好使用nullptr，尽量避免使用NULL。所有指针都必须初始化。

<h2 id="choose_header">选择C或C++标准库头文件</h2>

C++ 兼容 C 语言标准库，去掉了 C 头文件的后缀`.h`，并添加前缀`c`，表明这是一个 C 语言标准库的头文件。

对应的头文件在内容上是一样的，但是在名为`cname`的头文件中定义的名字从属于命名空间`std`，而`name.h`头文件中的却不然。

C++ 程序一律使用名为`cname`的头文件。

<h2 id="front_or_post">递增递减选择前置还是后置</h2>

前置版本的递增（递减）运算符将值加一（减一）后直接返回改变了的对象；

后置版本的递增（递减）运算符需要将原始值存储下来以便返回这个未修改的内容，然后再加一（减一）。

若无特殊需求，一律使用前缀版本的递增或递减运算符。

<h2 id="sizeof">sizeof运算符</h2>

sizeof 运算符返回一条表达式或一个类型名所占的字节数。

sizeof 运算符满足右结合律，返回值是一个`size_t`类型的常量表达式。

sizeof运算符的结果部分依赖于其作用的类型：

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


