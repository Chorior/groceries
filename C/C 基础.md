# C 基础

本文为博主学习 C primer plus 第五版的笔记，适用于有一定 C++ 基础的人食用。

## 本文结构

* [概述](#overview)
* [C 和 C++](#c_and_c++)
* [常用头文件](#common_headers)
* [格式化输入输出](#scanf_and_printf)
* [字符串函数](#string_functions)
* [内存管理](#memory_management)
* [文件输入输出](#file_io)

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

头文件 `stdlib.h` 中的 qsort 可以对一个数组进行快速排序。

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
`char* gets_s( char* str, rsize_t n )` | stdio.h | 从 stdin 读取字符直到发现换行符或出现文件尾 <br> 至多写入 n-1 个字符到 str 所指向的数组 <br> 始终写入空字符（除非 str 是空指针）<br> 若发现换行符，读取并将其舍弃 | 成功返回str <br> 出错或遇到eof返回NULL | c11
`int puts( const char* str )` | stdio.h | 写入字符串 str 的字符到stdout <br> **附加换行符** | 正确返回非负值 <br> 错误返回 EOF | \-
`char* fgets ( char* str, int num, FILE* stream )` | stdio.h | 从文件流里读取最多num-1个字符 <br> 若发现换行符，读取并将其存放在字符串里 | 成功返回str <br> 出错或遇到eof返回NULL | \-
`int fputs ( const char* str, FILE* stream )` | stdio.h | 写入字符串 str 的字符到输出流 <br> 不添加换行符 | 正确返回非负值 <br> 错误返回 EOF | \-
`int sprintf ( char* str, const char* format, ... )` | stdio.h | 写入格式化字符串到str | 成功返回写入字符个数(不包括空字符) <br> 失败返回负数 | \-
`int sscanf ( const char * s, const char * format, ...)` | stdio.h | 从s中读取格式化字符 | 成功返回读入字符个数(不包括空字符) <br> 失败返回EOF | \-
`size_t strlen ( const char* str )` | string.h | 返回从str开始直到空字符间(不包括)的长度 | \- | \-
`char* strncat ( char* dst, const char* src, size_t num )` | string.h | 添加src的最多前num个字符到dst的末尾 <br> 遇到src的空字符时结束 | dst | \-
`int strncmp ( const char* str1, const char* str2, size_t num )` | string.h | 字符序比较两个字符串的最多前num个字符 <br> 遇到空字符时结束 | 大于返回正数 <br> 等于返回0 <br> 小于返回负数 | \-
`char* strncpy ( char* dst, const char* src, size_t n )` | string.h | 复制src的最多前n个字符到dst <br> 遇到src的空字符时结束 | dst | \-
`const char* strchr ( const char* str, int ch )` | string.h | 返回指向str中存放字符ch的第一个位置的指针 <br> ch 可以为空字符 | 失败返回NULL | \-
`const char* strrchr ( const char* str, int ch )` | string.h | 返回指向str中存放字符ch的最后一个位置的指针 <br> ch 可以为空字符 | 失败返回NULL | \-
`const char* strstr ( const char* str1, const char* str2 )` | string.h | 返回str1中str2第一次出现的位置的指针 | 失败返回NULL | \-
`void* memchr ( const void* ptr, int c, size_t num )` | string.h | 从ptr指向的前num个字节中查找c <br> c被转换为unsigned char | 成功返回c第一次出现处的指针 <br> 失败返回NULL
`int memcmp ( const void* ptr1, const void* ptr2, size_t n )` | string.h | 比较ptr1与ptr2的前n个字节 <br> 把每个字节都解释为unsigned char <br> 遇到空字符时不结束| 大于返回正数 <br> 等于返回0 <br> 小于返回负数 | \-
`void* memmove ( void* dst, const void* src, size_t n )` | string.h | 复制src的前n个字节到dst中 <br> 为防止溢出，dst和src至少n个字节 <br> 地址重叠时，src会被修改| dst | \-
`void* memset ( void* ptr, int v, size_t n )` | string.h | v被转换为unsigned char <br> 然后被赋值到ptr指向的前n个字节 | ptr | \-
`long int strtol (const char* restrict str, char** restrict endptr, int base)` <br> `long long strtoll(const char* restrict str, char** restrict endptr, int base)` <br> `unsigned long strtoul(const char* restrict str, char** restrict endptr, int base)` <br> `double strtod(const char* restrict str, char** restrict endptr)` | stdlib.h |假定str中的数字以base为基数 <br> 返回str的开始部分转换为对应返回值类型的值 <br> 跳过开始的空格，在碰到第一个不是数字的字符时结束转换 | 成功时，endptr指向数字之后第一个字符 <br> 失败时endptr指向str | C99

<h2 id="memory_management">内存管理</h2>

C 里面的内存管理与 C++ 大体一致，但是用于内存分配的函数与 C++ 有些区别，C 一般使用下面三个函数分配内存：

函数 | 头文件 | 功能 | 返回值
---- | ---- | ---- | -----
`void* malloc(size_t size)` | stdlib.h | 在内存中分配一块size字节大小的**未初始化**的块 | 成功返回块的起始地址 <br> 失败返回 NULL 
`void* calloc(size_t nmem, size_t size)` | stdlib.h | 为具有nmem个成员的数组分配空间，其中每个成员的大小为size个字节 <br> 空间中所有位都初始化为0 | 成功返回数组地址 <br> 失败返回 NULL
`void* realloc(void* ptr, size_t size)` | stdlib.h | 把ptr指向的内存块大小改变为size字节 <br> 从块开始到新旧大小中较小值的部分中内容不被改变 <br> 当ptr为空时相当于malloc，size为空时相当于free | 成功返回块的位置，**它可能被移动** <br> 失败返回 NULL 且不改变原始的块

上面三个函数得到的指针都可以用 `void free(void* ptr)` 函数来释放，但是需要注意的是：

*	**不能对同一个指针多次调用 free**；
*	**可以对一个 NULL 指针多次调用 free**。

所以**在对某个非空指针调用 free 后，最后将其赋值为 NULL**，这样可以避免野指针和误操作问题。

<h2 id="file_io">文件输入输出</h2>

C++ 里面用于文件处理的标准流是 fstream，C 里面处理文件的函数主要有下面几个：

函数 | 头文件 | 功能 | 返回值
---- | ---- | ---- | -----
`FILE* fopen(const char* restrict filepath, const char* restrict mod)` | stdio.h | 打开filepath指定的文件 | 成功返回对应的文件流 <br> 失败返回 NULL
`int fclose( FILE* fp )` | stdio.h | 关闭fp指向的文件流 | 成功返回0 <br> 失败返回 EOF
`int fgetc(FILE* fp)` | stdio.h | 从fp中读取一个字符 | 成功返回所读取的一个字节 <br> 失败返回 EOF
`int fputc(int c, FILE* fp)` | stdio.h | 将字符c输出到fp中 | 成功返回写入文件的字符的ASCII码值 <br> 失败返回 EOF
`int ferror(FILE* fp)` | stdio.h | fp的测试指示器 | fp发生错误时返回非零值
`size_t fread (void* buffer, size_t size, size_t count, FILE* fp)` | stdio.h | 从fp中读二进制数据，最多读取count个项，每个项size个字节到buffer中 | 返回实际读取的项数
`size_t fwrite(const void* buffer, size_t size, size_t count, FILE* fp)` | stdio.h | 从buffer中写入count个size字节的二进制数据到fp中 | 返回实际写入的数据项个数
`void rewind(FILE* fp)` | stdio.h | 把fp设置到文件开始 | \-
`int fgetpos(FILE* restrict fp, fpos_t* restrict pos)` | stdio.h | 在pos所指的位置放置一个`fpos_t`值，这个值描述了文件中的一个位置 | 失败返回非零值
`int fsetpos(FILE* fp, const fpos_t* pos)` | stdio.h | 将文件指针指向pos指定的位置 | 失败返回非零值
`int fseek(FILE* fp, long offset, int fromwhere)` | stdio.h | 将文件指针设置为从fromwhere偏离offset距离的值 <br> fromwhere 有三个常量：`SEEK_SET`(文件开始),`SEEK_CUR`（当前位置）,`SEEK_END`(文件结尾) | 失败返回-1
`long ftell(FILE* stream)` | stdio.h | 返回文件位置指针当前位置相对于文件首的偏移字节数 | \-
`int rename(char* oldname, char* newname)` | stdio.h | 文件重命名 <br> 可以实现文件移动 | 成功返回0，失败返回-1

