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

**C++程序不应该使用库函数rand，而应使用`default_random_engine`类和恰当的分布类对象(`<random>`)**。随机数发生器是指分布对象与引擎对象的组合。

```c++
uniform_int_distribution<unsigned> u(0, 9);
default_random_engine e;
for (int i = 0; i < 10; ++i) {
	cout << u(e) << " ";
}
```
