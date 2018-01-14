---
title:      "C++ 释疑（三）"
subtitle:   "拷贝控制、运算符重载与模板编程"
date:       2017-04-04 14:28:00 +0800
header-img: "img/sunset_beach.jpg"
tags:
    - C++
    - C++ primer
---

本文是我重温《C++ Primer 5》第三部分类设计者的工具时，摘录的一些要点，适合初级程序员食用。

#   本文结构

*   [拷贝控制](#copy_control)
	*	[拷贝构造函数](#copy_constructor)
	*	[拷贝赋值运算符](#copy_assignment_operator)
	*	[析构函数](#destructor)
	*	[右值引用](#rvalue_reference)
	*	[移动操作](#move_operation)
	*	[swap 操作](#swap_operation)
	*	[移动和拷贝重载、引用限定符](#move_copy_override_and_ref_qualifier)
*	[重载运算与类型转换](#operator_overload)
	*	[运算符重载抉择](#operator_overload_choice)
	*	[箭头运算符的执行过程](#arrow_operator_execution)
	*	[标准库函数对象](#standard_function_object)
	*	[可调用对象](#callable_object)
	*	[类型转换运算符](#type_conversion)
*	[面向对象程序设计](#object_oriented_programming)
	*	[基类与派生类](#base_and_derived_class)
	*	[虚函数](#virtual_function)
*	[模板与泛型编程](#template_and_generic_program)
	*	[模板概述](#template_overview)
	*	[模板特例化](#template_specialization)

<h2 id="copy_control">拷贝控制</h2>

拷贝控制操作包括：

*	拷贝构造函数
*	拷贝赋值运算符
*	移动构造函数
*	移动赋值运算符
*	析构函数

**如果一个类没有定义所有这些拷贝控制成员，编译器将自动为它定义缺失的操作，可以通过使用`=default`显式的使用合成的版本，也可以使用`=delete`来阻止生成相应函数**。

**只能对默认构造函数和拷贝控制操作使用`=default`，但`=delete`可以对除析构函数外的任意函数使用（`=delete`必须出现在函数第一次声明的时候）**。

**如果一个类有数据成员不能默认构造、拷贝、复制或销毁，则对应的合成函数被定义为删除的**。

老式C++通过将拷贝构造函数和拷贝赋值运算符定义为private，且不定义它们来阻止拷贝。**声明但不定义一个成员函数是合法的**。

*	**使用等号定义变量**时，如果右侧对象是右值引用，那么会使用移动构造函数，否则使用拷贝构造函数；
*	**将一个对象作为实参传递给一个非引用类型的形参**时，如果对象是右值引用，那么会使用移动构造函数，否则使用拷贝构造函数；
*	**从一个返回类型为非引用类型的函数返回一个对象**时，如果对象是右值引用，那么会使用移动构造函数，否则使用拷贝构造函数；
*	**用花括号列表初始化一个数组中的元素或一个聚合类中的成员**时，如果列表值是右值引用，那么会使用移动构造函数，否则使用拷贝构造函数；

**通常一个类时不需要移动操作和拷贝操作的**，可以通过继承如下类或[noncopyable.hpp](http://www.boost.org/doc/libs/1_63_0/boost/core/noncopyable.hpp)来达到这个需求

```c++
class noncopyable
{
protected:
	noncopyable() = default;           // 默认构造函数不可访问
	virtual ~noncopyable() = default;  // 移动操作不会合成，派生类也不会合成

	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};
```

<h3 id="copy_constructor">拷贝构造函数</h3>

**如果一个构造函数的第一个参数是自身类类型的左值引用，且任何额外的参数都有默认值，则此构造函数是拷贝构造函数**。

**拷贝构造函数通常不应该是explicit的**；编译器生成的合成拷贝构造函数会使用成员的拷贝构造函数从给定对象中依次将每个非static成员拷贝到正在创建的对象中（数组成员会逐元素拷贝）。

**如果类的某个成员的拷贝构造函数是删除的或不可访问的（如private），则合成的拷贝构造函数被定义为删除的**。

```c++
class FOO
{
public:
	FOO();            // 默认构造函数
	FOO(const FOO&);  // 拷贝构造函数
}
```

<h3 id="copy_assignment_operator">拷贝赋值运算符</h3>

标准库通常要求保存在容器中的类型要具有拷贝赋值运算符，且其返回值是左侧运算对象的引用。

编译器生成的合成拷贝赋值运算符会通过成员类型的拷贝赋值运算符将右侧运算对象的每个非static成员赋予左侧运算对象的对应成员，并且返回一个指向其左侧运算对象的引用。

**如果类的某个成员的拷贝赋值运算符是删除的或不可访问的（如private），或是类有一个const的或引用成员，则合成的拷贝赋值运算符被定义为删除的**。

**需要拷贝赋值运算符的类也需要拷贝构造函数，反之亦然**。

**通过先拷贝右侧运算对象，可以处理自赋值情况，且能在异常发生时将左侧运算对象置于一个有意义的状态**。

```c++
class FOO
{
public:
	FOO& operator=(const FOO&); // 拷贝赋值运算符
	FOO& operator=(const FOO);  // 异常安全，能正确处理自赋值，且能用作移动赋值
}
```

<h3 id="destructor">析构函数</h3>

析构函数**首先执行函数体**，然后通过成员自己的析构函数（内置类型什么也不做）销毁成员。成员按初始化顺序的逆序销毁。

析构函数名字由波浪号接类名构成，没有返回值，也不接受参数。一个类只会有一个析构函数。

当一个对象被销毁时，就会自动调用其析构函数。

**如果类的某个成员的析构函数是删除的或不可访问的（如private)，则合成的析构函数和拷贝构造函数被定义为删除的**。

```c++
class FOO
{
public:
	~FOO(); // 析构函数
}
```

**如果一个类需要定义析构函数，那么你需要删除或重定义拷贝构造函数和拷贝赋值运算符**，因为如果该类有一个指针成员，默认的拷贝构造函数和拷贝赋值运算符会直接拷贝该指针成员，当拷贝的对象被销毁时，原对象的指针成员也被销毁了

```c++
class FOO
{
private:
	int *pi;
public:
	FOO() : pi(nullptr) {}
	FOO(int *p) : pi(P) {}
	~FOO() { delete pi; } 
}

FOO func(FOO foo)
{
	FOO ret = foo;
	return ret;    // ret和foo中的pi都被delete了，返回的pi指向无效内存！
}
```

<h3 id="rvalue_reference">右值引用</h3>

`std::move`定义于头文件`<utility>`中。

必须绑定到右值的引用，称为右值引用。

*	使用`&&`来获得右值引用；
*	**右值引用只能绑定到一个即将销毁的对象**；
*	不能将右值引用绑定到一个左值上，**右值引用类型变量也是一个左值**；
*	字面常量是右值，可以绑定到右值引用；
*	可以使用`std::move`来显式的将一个左值转换为相应的右值引用，但**移后源对象只能进行赋值和销毁操作**。

<h3 id="move_operation">移动操作</h3>

**当对象在拷贝后就立即被销毁时，使用移动而非拷贝会大幅度提升性能**。

**标准库容器、string、`shared_ptr`既支持移动也支持拷贝；IO 类和`unique_ptr`只支持移动**。

**如果一个构造函数的第一个参数是自身类类型的右值引用，且任何额外的参数都有默认值，则此构造函数是移动构造函数**。

**一旦资源完成移动，源对象必须不再指向被移动的资源，且销毁源对象时无害的（通常将指针赋值为nullptr来实现）**。

移动操作通常不会抛出任何异常，因为它不分配任何资源，所以移动构造函数和移动赋值运算符通常标记为`noexcept`。

*	`noexcept`出现在参数列表和初始化列表开始的冒号之间；
*	声明和定义都需要指定`noexcept`。

```c++
class FOO
{
	FOO(FOO &&) noexcept;            // 移动构造函数
	FOO& operator=(FOO &&) noexcept; // 移动赋值运算符，需要考虑自赋值
}
```

*	**如果一个类定义了自己的拷贝构造函数、拷贝赋值运算符或析构函数，编译器就不会为它合成移动构造函数和移动赋值运算符了，此时对应的移动操作会被转换为相应的拷贝操作**；
*	**只有当一个类没有定义任何自己版本的拷贝控制成员，且它的所有数据成员都能移动构造或移动赋值（内置类型可以移动）时，编译器才会合成移动构造函数或移动赋值运算符**；
*	**定义了移动构造函数或移动赋值运算符的类也必须定义自己的拷贝操作，否则拷贝操作默认被定义为删除的**。

<h3 id="swap_operation">swap 操作</h3>

如果一个类需要与重排元素顺序的算法一起使用，因为这类算法会在交换两个元素时调用`swap`，若该类自定义了自己的swap，那么算法将使用类自定义版本，否则使用`std::swap`。

```c++
template<class _Ty,
	class> inline
	void swap(_Ty& _Left, _Ty& _Right)
		_NOEXCEPT_OP(is_nothrow_move_constructible<_Ty>::value
			&& is_nothrow_move_assignable<_Ty>::value)
	{	// exchange values stored at _Left and _Right
	_Ty _Tmp = _STD move(_Left);
	_Left = _STD move(_Right);
	_Right = _STD move(_Tmp);
	}
```

如果成员m有自定义的swap函数，那么使用方式是

```c++
using std::swap;
swap(m, other.m);
```

因为对于一个调用，**如果一个非函数模板与一个函数模板提供同样好的匹配，则选择非模板版本**。

<h3 id="move_copy_override_and_ref_qualifier">移动和拷贝重载、引用限定符</h3>

区分移动和拷贝的重载函数通常有一个版本接受一个`const T&`，而另一个版本接受一个`T&&`。

```c++
void example(const int&); // 拷贝元素
void example(int&&);      // 移动元素
```

引用限定符类似const限定符，分为`&`和`&&`，分别指出this可以指向一个左值或右值

*	引用限定符只能用于非static成员函数，因为static成员函数没有this指针；
*	引用限定符必须同时出现在函数的声明与定义之中；
*	`&`限定的函数只能用于左值；
*	`&&`限定的函数只能用于右值；
*	引用限定符和const限定符一起使用时，引用限定符必须跟随在const限定符之后；
*	**如果一个成员函数有引用限定符，则具有相同参数列表的所有版本都必须有引用限定符**；
*	**没什么特别要求的话，不要使用引用限定符**。

```c++
FOO &retFOO(); // 返回一个引用，retFOO调用是一个左值
FOO retVal();  // 返回一个值，retval调用是一个右值
FOO i, j;      // i, j是左值
i = j;         // 正确
retFOO() = j;  // 正确
retVal() = j;  // 错误，retval()返回一个右值
i = retVal();  // 正确
```

<h2 id="operator_overload">重载运算与类型转换</h2>

当运算符作用于类类型的运算对象时，可以通过运算符重载定义该运算符的含义。

*	**重载运算符的优先级和结合律与内置运算符保持一致**；
*	重载运算符函数的名字由关键字operator和其后要定义的运算符号共同组成；
*	重载运算符函数也包含返回类型、参数列表以及函数体；
*	**对于一个重载的运算符函数，它要么是类的成员，要么至少含有一个类类型的参数**；
*	**重载运算符函数的参数数量与该运算符作用的运算对象数量一样多，但对于成员重载运算符函数，左侧运算对象绑定到隐式的this指针，所以成员运算符函数的显式参数数量比运算符的运算对象总数少一个**；
*	**对于二元运算符，左侧运算对象传递给第一个参数，右侧运算对象传递给第二个参数**；
*	**对于成员重载运算符，左侧运算对象必须是所属类的一个对象**；
*	**除重载的函数调用运算符`operator()`外，其它重载运算符均不能含有默认实参**。

可以被重载的运算符

```text
+ 		-		*		/		%		^
&		|		~		!		,		=
<		>		<=		>=		++		--
<<		>>		==		!=		&&		||
+=		-=		/=		%=		^=		&=
|=		*=		<<=		>>=		[]		()
->		->*		new		new[]		delete		delete[]
```

不能被重载的运算符

```c++
::		.*		.		?:
```

可以像调用普通函数一样调用运算符函数

```c++
data1 + data2;           // 普通表达式
operator+(data1, data2); // 等价的函数调用
data1.operator++();      // 前置版本
data1.operator++(2);     // 后置版本
```

**重载的逻辑与运算符`&&`、逻辑或运算符`||`和逗号运算符`,`无法保留运算对象的求值顺序，重载的`&&`和`||`还无法保留内置运算符的短路求值属性，因此不建议重载它们，另外取地址运算符`&`也不建议重载**。

<h3 id="operator_overload_choice">运算符重载抉择</h3>

*	**如果需要定义`operator==`，那么通常也应该定义`operator!=`**；
*	**如果需要定义`operator<`，那么通常也应该包含`>`、`>=`、`<=`操作**；
*	**如果需要定义算数运算符或位运算符，那么最好也定义复合赋值运算符**；
*	**下标运算符最好同时定义常量版本和非常量版本，一个返回常量引用，一个返回普通引用**；
*	**递增递减运算符最好同时定义前置和后置版本；后置版本接受一个额外的不被使用的int类型的参数，且返回值是一个值**；
*	**重载的箭头运算符必须返回类的指针或自定义了箭头运算符的某个类的对象，关于[箭头运算符的执行过程](#arrow_operator_execution)**；
*	**逻辑运算符和关系运算符应该返回bool**；
*	**算数运算符（如+）应该返回一个类类型的值**；
*	**赋值运算符和复合赋值运算符（如+=）应该返回左侧运算对象的一个引用**；
*	**赋值(=)、下标([])、调用(())、成员访问箭头(->)必须定义为成员函数**；
*	**一个类可以定义多个函数调用运算符**；
*	**输入输出运算符必须是非成员函数，因为左侧运算符对象是一个iostream对象而非类对象**。

```c++
bool operator==(const CLASS_NAME &a, const CLASS_NAME &b);
bool operator!=(const CLASS_NAME &a, const CLASS_NAME &b) { return !(a == b); }
CLASS_NAME operator+(const CLASS_NAME &a, const CLASS_NAME &b);
CLASS_NAME& operator+=(CLASS_NAME &a, const CLASS_NAME &b);
std::ostream& operator<<(std::ostream& out, const CLASS_NAME &item); // 尽可能减少格式化操作
std::istream& operator>>(std::istream& in, CLASS_NAME &item); // 必须处理输入可能失败的情况
class FOO
{
public:
	TYPE& operator[](const std::size_t n);
	const TYPE& operator[](const std::size_t n) const;
	FOO& operator++();     // 前置版本
	FOO operator++(int);   // 后置版本
	TYPE& operator*() const;
	TYPE* operator->() const { return & this->operator*(); }
	TYPE operator()(args) const; // 函数调用运算符
}
```

<h3 id="arrow_operator_execution">箭头运算符的执行过程</h3>

```c++
point->mem;
```

1. 如果point是指针，则表达式等价于`(*point).mem`;
2. 如果point是定义了`operator->`的类的一个对象，那么获取`point.operator->()`的结果，若结果是一个指针，则执行第一步，否则重复调用当前步骤。

<h3 id="standard_function_object">标准库函数对象</h3>

标准库函数对象定义于`<functional>`头文件中。

标准库函数对象 | 说明
-------------------- | -------------------------------------------------
`plus<Type>` <br> `minus<Type>` <br> `multiplies<Type>` <br> `divides<Type>` <br> `negate<Type>`| 加<br>减<br>乘<br>除<br>取反
`equal_to<Type>` <br> `not_equal_to<Type>` <br> `greater<Type>` <br> `greater_equal<Type>` <br> `less<Type>` <br> `less_equal<Type>` | `=` <br> `!=` <br> `>` <br> `>=` <br> `<` <br> `<=`
`logical_and<Type>` <br> `logical_or<Type>` <br> `logical_not<Type>` | 与<br>或<br>非

```c++
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <functional>

int main()
{
	std::istream_iterator<std::string> in_iter(std::cin), eof;
	std::vector<std::string> svec(in_iter, eof);
	std::sort(svec.begin(), svec.end(), std::greater<std::string>());
	for (auto &s : svec)
	{
		std::cout << s << std::endl;
	}
	getchar();
	return EXIT_SUCCESS;
}
```

<h3 id="callable_object">可调用对象</h3>

可调用对象包括

*	函数
*	函数指针
*	lambda表达式
*	bind创建的对象
*	重载了函数调用符的类

函数指针不能指向lambda表达式、bind创建的对象和重载了函数调用符的类，要想保存所有类型的可调用对象，需要使用C++11的新标准库类型function。

function是一个模板，定义于`<functional>`头文件中，需要的额外信息是function类能够表示的对象的调用形式，如`function<int(int, int)>`。

function操作 | 说明
-------------------- | -------------------------------------------------
`function<T> f` | f是一个用来存储可调用对象的空function,T是`retType(args)`
`function<T> f(nullptr)` | 显式的构造一个空function
`function<T> f(obj)` | 在f中存储可调用对象obj的副本
`f` | 将f作为条件：当f含有可调用对象时为真
`f(args)` | 调用f中的对象，参数是args
`result_type` | 类型别名，该function类型的可调用对象返回的类型
`argument_type` <br> `first_argument_type` <br> `second_argument_type` |  类型别名，当T有一个或两个实参时定义的类型

```c++
#include <iostream>
#include <cstdlib>
#include <string>
#include <functional>
#include <unordered_map>

float add(float a, float b) { return a + b; }
int add(int a, int b) { return a + b; }
int divide(int a, int b) { return a / b; }

struct mod
{
	int operator()(int a, int b) { return a % b; }
};

int main()
{
	// use _1, _2
	using namespace std::placeholders;
	std::unordered_map<std::string, std::function<int(int, int)> > binops = {
		{"+", static_cast<int(*)(int,int)>(add)}, // 有重载时需要指定版本
		{"-", std::minus<int>()},
		{"*", [](int a, int b) {return a * b; }},
		{"/", std::bind(divide,_1,_2)},
		{"%", mod()}
	};
	std::cout << binops["+"](5, 3) << std::endl;
	std::cout << binops["-"](5, 3) << std::endl;
	std::cout << binops["*"](5, 3) << std::endl;
	std::cout << binops["/"](5, 3) << std::endl;
	std::cout << binops["%"](5, 3) << std::endl;
	getchar();
	return EXIT_SUCCESS;
}
```

<h3 id="type_conversion">类型转换运算符</h3>

转换构造函数和类型转换运算符共同定义了类类型转换；类型转换函数的一般形式如下

```c++
operator type() const;
```

**类型转换函数必须是类的成员函数**，没有返回类型，形参列表为空。

**避免过度的使用类型转换函数，如果一定要使用，建议定义为explicit，否则会自动转换**，通常会定义一个bool类型转换。

**在下列位置时，即使是explicit类型转换，也会自动进行类型转换**：

*	if, while, do的条件位置；
*	for语句头的条件表达式；
*	逻辑与或非的运算对象；
*	条件运算符(?:)的条件表达式。

<h2 id="object_oriented_programming">面向对象程序设计</h2>

面向对象程序设计的核心思想

*	数据抽象：接口与实现分离；
*	继承：定义相似的类型并对其关系建模；
*	动态绑定：使用基类的引用或指针调用一个虚函数时，虚函数的版本由被绑定的类决定。

<h3 id="base_and_derived_class">基类与派生类</h3>

*	**私有成员在派生类中不可见**；
*	**protected成员对于友元和派生类成员是可见的，但不能被公共访问**；
*	**public继承的成员将遵循原有的访问说明符，private继承的成员将变为private，protected继承会将public成员变为protected**；
*	**用class修饰的类默认采用私有继承，用struct修饰的类默认采用公有继承**；
*	**友元关系不能被继承，就像你爸爸的朋友并不是你的朋友一样**；
*	**如果基类希望其派生类各自定义适合其自身的版本，那么这些函数应该被声明为虚函数**；
*	**任何构造函数之外的非静态函数都可以是虚函数，virtual只能出现在类内部的声明语句之前**；
*	**基类的虚函数在派生类中隐式的也是虚函数**；
*	**派生类需要对所有重新定义的虚函数进行声明，并最好在函数形参列表后增加一个override关键字(C++11)，如果有const限定符或引用限定符，那么override在最后面**；
*	**基类通常应该定义一个虚析构函数，这样动态绑定时可以调用正确的析构函数，也不会合成移动操作(派生类也不会)**；
*	**如果派生类没有覆盖其基类中的某个虚函数，那么派生类会直接继承其在基类中的版本**；
*	**派生类必须使用基类的构造函数来初始化其基类部分，即委托构造函数，默认使用基类的默认构造函数**；
*	**派生类的拷贝操作、移动操作需要使用作用域控制符拷贝或移动基类部分**；
*	**若构造函数或析构函数调用了某个虚函数，那么会使用其所属类型相对应的虚函数版本，即动态绑定无效**；
*	**基类静态成员只存在唯一的实例，若其是可访问的，那么基类和派生类均可访问**；
*	**派生类的声明不能包含类派生列表，如`: public Base`**；
*	**要继承一个类，那么该类必须被定义而不能仅声明，这条规定排除了派生自身这个错误**；
*	**在类名后跟一个关键字final可防止该类被继承**；
*	**可以将基类指针或引用绑定到派生类对象上，智能指针也支持该操作**；
*	**不存在从基类到派生类的隐式类型转换**；
*	**当使用一个派生类对象为一个基类对象初始化或赋值时，只有该派生类对象中的基类部分会被拷贝、移动或赋值**；
*	**只有当继承方式是公有继承时，用户代码才能使用派生类向基类的转换，但成员函数和友元一定能使用派生类向基类的转换**；
*	**派生类的作用域嵌套在基类作用域之内，因为派生类无法解析的名字将在基类继续寻找**；
*	**派生类的成员会隐藏同名的基类成员(即使形参列表不一致)，要想使用被隐藏的基类成员必须使用作用域运算符，所以最好不要重用定义在基类中的名字**；
*	**想要在容器中同时保存基类对象和派生类对象时，可以设计容器元素为基类的智能指针对象**；
*	**可以使用using声明改变可访问成员的可访问性，using声明可以把名字(包含除默认构造函数、拷贝和移动构造函数之外的构造函数)对应的所有可访问重载添加到派生类作用域中，重定义的版本将覆盖基类版本**。

```c++
class A
{
public:
	void func(){}
	void func(int){}
protected:
	int n;
}

class B : A        // 默认私有继承
{
public:
	using A::func; // using 声明，添加了func的所有重载
protected:
	using A::n;
}
```

<h3 id="virtual_function">虚函数</h3>

*	任何构造函数之外的非静态函数都可以是虚函数，virtual只能出现在类内部的声明语句之前；
*	**在类内部的虚函数声明语句的分号之前加上`=0`，则该虚函数被说明为纯虚函数**；
*	**含有纯虚函数的类被称为抽象基类，抽象基类不允许创建对象**；
*	**除纯虚函数外的虚函数必须被定义**，因为动态绑定时不知道哪个虚函数会被调用；
*	**纯虚函数也可以被定义，但是必须定义在类的外部**；
*	一旦某个函数被声明为虚函数，那么所有派生类中的该函数都是虚函数；
*	派生类中虚函数的形参必须与基类中被覆盖的虚函数的形参严格匹配；
*	若类的虚函数的返回类型不是类本身的指针或引用，那么派生类中该虚函数的返回类型必须与基类函数匹配；
*	**覆盖的虚函数最好加上override关键字，以免发生错误造成新定义(并会隐藏同名基类函数)而不是覆盖**；
*	**若某个虚函数被标记为final(final与override相对位置任意)，那么该虚函数不允许被覆盖**；
*	**虚函数的默认实参由调用的静态类型决定**，建议将基类和派生类中定义的默认实参设为相同；
*	**要想避免动态绑定，可以使用作用域运算符`Base::mem()`，这通常用于虚函数想要调用被覆盖的虚函数时，如果没加作用域运算符，将会无限递归**；
*	**重构就是给已定义的类加个基类(爸爸)**。

<h2 id="template_and_generic_program">模板与泛型编程</h2>

<h3 id="template_overview">模板概述</h3>

一个函数模板就是一个公式，可用来生成针对特定类型的函数模板。

*	模板定义以关键字template开始，后跟一个用方括号`<>`包起来的模板参数列表；
*	**模板参数列表不能为空**；
*	**模板内不能重用模板参数名**；
*	**类型模板参数前必须加上关键字class或typename，非类型模板参数必须指定其特定类型**
*	**非类型模板参数的实参必须是常量表达式，绑定到指针或引用非类型参数的实参必须具有静态的生存期**；
*	使用模板时，需要显式或隐式的指定模板实参，并将其绑定到模板参数上；
*	**编译器可以为函数模板自动推断其模板参数类型(但最好显式指定)，但类模板不行，必须显示指定模板实参**；
*	**如果一个非函数模板与一个函数模板提供同样好的匹配，则选择非模板版本**；
*	**模板程序应该尽量减少对实参类型的要求**，如在已使用小于运算符的情况下，需要大于运算符时，可以转换为使用小于运算符；
*	**保证传递给模板的实参支持模板所要求的所有操作，以及这些操作在模板中能正确工作，是调用者的责任**；
*	**要实例化一个模板，该模板必须是已定义的**；
*	**每个模板实例都是独立的，即使是static成员也不例外**；
*	**定义于类模板之外的成员函数必须以关键字template开始，后接类模板参数列表**；
*	**一个类模板的成员函数(即使是static成员函数)只有当程序用到它时才进行实例化，没用到的成员函数不会被实例化**；
*	**在类模板作用域内，可以直接使用模板名，而不必指定模板实参**；
*	**当需要通知编译器一个名字代表类型时，必须使用关键字typename而不能使用class**；
*	**模板参数也可以定义默认实参，规则与函数默认实参一样，右侧所有参数都必须有默认实参**；
*	**一个类里面可以定义模板成员函数，成员模板不能是虚函数**；
*	**显式实例化会实例化所有成员**；

```c++
template <typename T, int N>
inline T foo(T* p)
{
	T tmp = *p;
	//..
	return tmp;
}

template <typename T1, typename T2>
class Example
{
public:
	Example() = default;
	void do_something();
}

template <typename T1, typename T2>
void Example<T1,T2>::do_something()
{
	// do something
}

// 显式实例化
extern template foo<int>;  // 实例化声明
extern template class Example<string, int>;
template int foo(int * p); // 实例化定义
template class Example<string, int>;
```

<h3 id="template_specialization">模板特例化</h3>

当不能使用或不希望使用模板版本时，可以定义类或函数模板的一个特例化版本。

**特例化一个函数模板时，原模板的声明必须在作用域中，且必须为原模板的每个模板参数提供实参**，特例化不影响函数匹配。

**在任何使用模板实例的代码之前，特例化版本的声明也必须在作用域中**。

```c++
template <>
void func(int a, int b){
	return a < b;
}
```

默认情况下，无序容器使用`hash<key_type>`来组织元素，为使自己的数据类型也能使用无序容器，必须定义一个hash模板的特例化版本。

一个特例化的hash类必须定义：

*	一个重载的调用运算符，接受一个容器关键字类型的对象，返回`size_t`；
*	两个类型成员，`result_type`和`argument_type`，分别表示调用运算符的返回类型和参数类型；
*	默认构造函数和拷贝赋值运算符（可以隐式定义）。

```c++
struct point3d{
	float a;
	float b;
	float c;
}
// 必须在原模板定义所在的命名空间中特例化
namespace std{	

	template <>
	struct hash<point3d>
	{
		typedef size_t result_type;
		typedef point3d argument_type;
		size_t operator()(const point3d& n) const;
	};

// 重载的调用运算符必须为给定类型的对象定义一个哈希函数
// 对于一个给定值，任何时候调用此函数都应该返回相同的结果，对于不等的对象几乎总是产生不同的结果
	size_t hash<point3d>::operator()(const point3d& n) const
	{
		return hash<float>()(n.a) ^ hash<float>()(n.b) ^ hash<float>()(n.c);
	}
}
```

