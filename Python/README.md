# Python 知识要点
---
:bowtie:
---
>作者：彭侦  
>链接：<https://github.com/Chorior/groceries/blob/master/python/README.md>

本文知识来自Vamei的《从Python开始学编程》。

* 本文结构
	* [Python初识](#Python初识)
	* [Python安装](Python安装)
	* [Hello Pyhthon](#Hello Python)
	* [Python基础](#Python基础)
	* [分支与循环语句](#分支与循环语句)

## Python初识
Python是一门多范式编程语言，既能面向过程，也能面向对象，还支持函数式编程；其作者是Guido von Rossum；语法多来自于C，但又受到ABC语言的强烈影响；优点在于实用和可扩展性，故而性能相比其它语言有些差距。

## Python安装
1. Mac与Linux
	这两个系统大多预装了Python，直接打开终端，输入`python`，然后回车就能使用pytho命令行了，退出命令为`exit()`：  

			$ python
			Python 2.7.12 (default, Jul  1 2016, 15:12:24)
			[GCC 5.4.0 20160609] on linux2
			Type "help", "copyright", "credits" or "license" for more information.
			>>> exit()
			$

	如果想要运行当前目录下的某个Python文件，使用Python+空格+文件名即可：

			$ python hello.py

	如果文件不在当前目录下，那么说明文件的完整路径即可，绝对或相对皆可：

			$ python /home/pengzhen/python/hello.py
			$ pwd
			/home/pengzhen
			$ python python/hello.py

	如果要将Python文件改成一个可执行的脚本，在文件第一行加上`#!/usr/bin/env python`（这跟shell脚本有些相似），然后将其权限改为可执行即可：

			$ chmod 775 hello.py
			$ ./hello.py

2. Windows
	到[Python官网](https://www.python.org)进行下载安装，安装时选择Customize来进行个性安装，另外勾选`Add python.exe to Path`，然后打开dos就可以像Mac和Linux一样使用命令行和运行Python文件了。

## Hello Pyhthon
学习一门语言，首先要做的就是写Hello World，就像拿到一个单片机先点亮一盏led灯一样。进入Python命令行之后，每行都会有>>>提示符提醒你在后面输入命令，你输入的每一行Python语句都会被解释器直接转译运行。我们首先打印一个字符串"Hello Python":

		$ >>> print("Hello Python")

print是一个专门用来打印的函数，如果你学过C语言，那么这跟printf很像，但printf里的f是格式化Format的意思，Python没有格式化，所以没有f；  

打印“Hello Python”的另一种方式是写文件，该文件的后缀必须是.py，下面是一个示例(名为hello.py)：

		#! /usr/bin/env Python

		print("Hello Python")

然后运行：

		$ python hello.py

## Python基础
1. 注释  
	在Python文件中，每行从#开始的文字都是注释，这跟shell脚本一样，连特殊的第一行不为注释也是一样的；如果要使用多行注释，使用三个连续的双引号或但引号将注释内容括起来即可，这又跟github版的markdown有些类似：

			#! /usr/bin/env python
			```
			Author: pengzhen
			Function: print hello python
			```

			print("Hello Python") # print

	如果想在Python2中使用中文，须在程序开始前加上`# -*- coding: utf-8 -*-`:

			# -*- coding: utf-8 -*-

			print("你好！Python!")

2. 基本运算
	* 数值运算(大体与C差不多，多了乘方)

			>>> 1 + 9            # 加法。结果为10
			>>> 1.3 - 4          # 减法。结果为-2.7
			>>> 3*5              # 乘法。结果为15
			>>> 4.5/1.5          # 除法。结果为3.0
			>>> 3**2             # 乘方。结果为9
			>>> 10%3             # 求余。结果为1
			>>> "he" + "llo"     # 字符串相加。结果为"hello"
			>>> "he" * 3         # 结果为"hehehe"

	* 逻辑运算(True表示真，False表示假)

			>>> True and False   # and表示与逻辑。结果为False
			>>> True or False    # or表示或逻辑。结果为True
			>>> not True         # not表示非逻辑。结果为False

	* 判断表达式

			>>> 1 == 1           # ==，等于。结果为True
			>>> 1 != 1           # !=，不等于。结果为False
			>>> 1 < 2            # <，小于。结果为True
			>>> 1 <= 2           # <=，小于等于。结果为True
			>>> 1 > 2            # >，大于。结果为False
			>>> 1 >= 2           # >=，大于等于。结果为False

	* 运算优先级(相同优先级按从左到右顺序进行)

			括号： ()
			乘方： **
			乘除： * /
			加减： + -
			判断： == > >= < <=
			逻辑： and or not

3. 变量类型  
	Python的变量根据数据自动转换类型，这称为动态类型，像C就是静态类型，C必须事先说好变量的类型才能进行赋值。查看一个Python变量的类型使用`type(var)`函数，常见的类型包括整型、浮点型、字符串和布尔型

			$ >>> a = 10
			$ >>> type(a)

	* 序列  
		序列是一种容器类型，它有两种形式，元组(Tuple)和列表(List)，其区别在于元组元素不可改变，而列表可以。元组圆括号括起，而列表用方括号括起，一个序列的元素可以是另一个列表:

				$ >>> a = (1,2,3)   # tuple
				$ >>> a = [3,2,1]   # list
				$ >>> a = [1,(2,3)] # list

		访问列表的方式与C的数组差不多，即用下标进行访问，下标从零开始

				$ >>> a = [1,(2,3)]
				$ >>> a[0]
				$ 1
				$ >>> a[1][0]
				2
				$ >>> a[1][1]
				3

		范围引用： `序列名[下限：上限：步长]`，其范围在数学上的区间表示为`[下限，上限)`，即不包括上限

				$ >>> a = [1,2,3,4,5]
				$ >>> a[:2]
				[1,2]
				$ >>> a[3:]
				[4,5]
				$ >>> a[0:5:2]
				[1,3,5]
				$ >>> a[3,0,-1]
				[4,3,2]
				$ >>> type(a[:]) # list in python 2

		尾部引用

				$ >>> a = [1,2,3,4,5]
				$ >>> a[-1]   # 序列最后一个元素
				5
				$ >>> a[-3]   # 序列倒数第三个元素
				3
				$ >>> a[1:-1] # 序列第二个到倒数第二个元素
				[2,3,4]

	* 词典
	词典也是一个容器类型，但它不以位置作为索引，如果你知道键值对的话，那词典的类型就是键值对。使用大括号建立词典：

			$ >>> a = {"a":1,"b":2,"c":3}
			$ >>> a["a"]
			1
			$ >>> a["b"]
			2
			$ >>> a["c"]
			3
			$ >>> type(a) # dict

## 分支与循环语句
