# C 基础

本文为博主学习 C primer plus 第五版的笔记，适用于有一定 C++ 基础的人食用。

## 本文结构

* [概述](#overview)
* [C 和 C++](#c_and_c++)
* [常用头文件](#common_headers)
* [格式化输入输出](#scanf_and_printf)
* [字符串函数](#string_functions)

<h2 id="overview">概述</h2>

C 编程的基础策略是使用程序将源代码文件转换为可执行文件，此文件包含可以运行的机器语言代码。C 分为两步来完成这一工作：**编译和链接**。

编译器将源代码转换为中间代码，链接器将此中间代码与其它代码相结合来生成可执行文件。你可以分别编译各个模块，然后使用链接器将编译过的模块结合起来，这样，如果需要改变一个模块，则不必重新编译所有其它模块。

<h2 id="c_and_c++">C 和 C++</h2>

C 的基本语法与 C++ 无异，但是**在传统 C 语言中，所有的变量都必须在一个代码块的开始处进行声明，在这之前不允许任何语句**，C99 遵循 C++ 的惯例，只需要在使用变量之前进行声明即可。

**char 变量在 C 中被存储在一块大小与 int 类型相同的内存中，但在 C++ 中只用一个字节进行存储**：

```c
int x = 'ABCD'
char c = 'ABCD'
printf("%d %d %c %c\n",x,'ABCD',c,'ABCD');
// 1094861636 1094861636 D D
```

上面这个例子说明，如果把 'ABCD' 看做一个 int 值，它就是一个4字节的整数值，但是如果把它看做 char 类型，程序就只使用最后一个字节。

<h2 id="common_headers">常用头文件</h2>

头文件 `inttypes.h` 中包含了常用的整数类型，使用这些整数类型可以便于移植程序。

头文件 `limits.h` 和 `float.h` 提供了有关整数类型和浮点类型的大小限制的详细信息。

头文件 `ctype.h` 提供了有关字符的一些常用函数，如 `isdigit` 判断字母是否是一个数字，`tolower` 返回一个字符的小写字符。

<h2 id="scanf_and_printf">格式化输入输出</h2>

C++ 中用 `std::cout` 和 `std::cin` 来输入输出，但是 C 用 `printf` 和 `scanf` 来输入输出，这两个函数都有相应的格式控制符来控制输入输出的格式。

### printf

转换说明符 | 意义
----------- | ------
`%c` | 一个字符
`%d` <br> `%i` | 有符号十进制整数
`%e` <br> `%E` | 浮点数，e或E记数法
`%f` | 浮点数，十进制记数法
`%g` <br> `%G` | 根据数值不同自动选择`%f`或`%e` <br> `%e`或`%E`格式在指数小于-4或者大于等于精度时使用
`%o` | 无符号八进制数
`%p` | 指针
`%s` | 字符串
`%u` | 无符号十进制数
`%x` <br> `%X` | 使用十六进制数字0f或0F的无符号十六进制整数
`%%` | 百分号

修饰符 | 意义
--------- | ----
`digit(s)` | 字段宽度的最小值，如 "%4d" <br> 如果该字段不能容纳要打印的数或字符串，系统就会使用更宽的字段
`.digit(s)` | 精度，如 "%5.2f" <br> 对于`%e`,`%E`和`%f`转换，是将要在小数点的右边打印的数字的位数 <br> 对于`%g`和`%G`转换，是有效数字的最大位数 <br> 对于`%s`转换，是将要打印的字符的最大数目 <br> 对于整数转换，是将要打印的数字的最小位数 <br> 如果必要，要使用前导零来达到这个位数
`h` | 和整数转换说明符一起使用，表示一个short int或unsigned short int类型数值，如 "%hu"
`hh` | 和整数转换说明符一起使用，表示一个signed char或unsigned char类型数值，如 "%hhu"
`j` | 和整数转换说明符一起使用，表示一个intmax_t或uintmax_t值，如 "%jd"
`l` | 和整数转换说明符一起使用，表示一个long int或unsigned long int值，如 "%ld"
`ll` | 和整数转换说明符一起使用，表示一个long long int或unsigned long long int值，如 "%lld"
`z` | 和整数转换说明符一起使用，表示一个size_t值，如 "%zd"
`L` | 和浮点转换说明符一起使用，表示一个long double值，如 "%Lf"

标志 | 意义
------- | ----
`-` | 项目是左对齐的，如 "%-20s"
`+` | 有符号的值若为正，则显示带加号的符号 <br> 若为负，则带减号的符号，如 "%+6.2f"
`#` | 若为`%o`格式，则以0开始 <br> 若为`%x`和`%X`格式，则以`0x`和`0X`开始 <br> 对于浮点数，#保证即使不跟任何数字，也打印一个小数点字符 <br> 对于`%g`和`%G`格式，它防止尾随零被删除，如 "%#o"

### scanf

转换说明符 | 意义
----------- | ---------
`%c` | 一个字符
`%d` <br> `%i` | 一个有符号十进制整数
`%e,%f,%g` <br> `%E,%F,%G` | 一个浮点数
`%o` | 一个有符号八进制整数
`%p` | 一个指针
`%s` | 一个字符串 <br> 以第一个非空白字符开始，直到下一个空白字符的全部字符
`%u` | 一个无符号十进制整数
`%x` <br> `%X` | 一个有符号十六进制整数

修饰符 | 意义
--------- | ----
`digit(s)` | 最大字段宽度 <br> 在达到最大字段宽度或遇到第一个空白字符时停止读取
`hh` | 把整数读作signed char或unsigned char
`ll` | 把整数读作long long或unsigned long long
`h` | "%hd"和"%hi"指示该值将会被存储在一个short中 <br> "%ho","%hx","%hu"指示该值将会被存储在一个unsigned short中
`l` | "%ld"和"%li"指示该值将会被存储在一个long中 <br> "%lo","%lx","%lu"指示该值将会被存储在一个unsigned long中 <br> "%le","%lf","%lg"指示该值将会被存储在一个double中
`L` | "%Le","%Lf","%Lg"指示该值将会被存储在一个long double中

### printf 与 scanf 的 \* 修饰符

**如果你在使用printf时不想事先指定字段宽度，而是希望程序指定该值，那么可以使用 \* 代替数字来达到这个目的，但是你也必须使用一个参数来告诉函数字段宽度应该是什么**：

```c
// 使用可变宽度的输出
#include <stdio.h>

int main(void)
{
	// 在传统 C 语言中，所有的变量都必须在一个代码块的开始处进行声明
	unsigned width = 0, precision = 0;
	int number = 256;
	double weight = 242.5;

	printf("What field width?\n");
	scanf("%d", &width);
	printf("The number is: %*d", width, number);
	printf("\nNow enter a width and a precision: \n");
	scanf("%d %d", &width, &precision);
	printf("Weight = %*.*f\n", width, precision, weight);

	return 0;
}
```

```text
What field width?
8
The number is:      256
Now enter a width and a precision:
10 6
Weight = 242.500000
```

**scanf格式字符串中的空格意味着跳过下一个输入项之前的任何空格，甚至没有空格的情况；但是其它字符一定要与输入字符串准确匹配**，下面的语句：

```c
scanf("%d, %d", &n, &m);
```

将会接受下列任何一个输入行：

```text
123,321
123, 321
123,   321
123 , 321
```

**当把 \* 放在 % 和说明符之间时，它使函数跳过相应的输入项目**：

```c
// 跳过输入
#include <stdio.h>

int main(void)
{
	int n;

	printf("Enter 3 intergers: \n");
	scanf("%*d %*d %d", &n);
	printf("The last interger is: %d", n);

	return 0;
}
```

```text
Enter 3 intergers:
1 2 3
The last interger is: 3
```

<h2 id="string_functions">字符串函数</h2>

函数 | 头文件 | 功能 | 返回值 | 可用性
---- | ---- | ---- | ----- | -------
`char *gets_s( char *str, rsize_t n )` | stdio.h | 从 stdin 读取字符直到发现换行符或出现文件尾 <br> 至多写入 n-1 个字符到 str 所指向的数组 <br> 始终写入空终止字符（除非 str 是空指针）<br> 若发现换行符，读取并将其舍弃 | 成功返回字符串地址 <br> 出错或遇到文件尾返回空 | c11
`int puts( const char *str )` | stdio.h | 写入每个来自空终止字符串 str 的字符及**附加换行符** '\n' 到输出流 stdout | 正确返回非负值 <br> 错误返回 EOF | \-

