# Python 知识要点
---
:bowtie:
---
>作者：彭侦  
>链接：<https://github.com/Chorior/groceries/blob/master/Python/README.md>

本文知识来自Vamei的《从Python开始学编程》。

* 本文结构
	* [Python初识](#Python初识)
	* [Python安装](#Python安装)
	* [Hello Python](#Hello-Python)
	* [Python基础](#Python基础)
	* [分支与循环语句](#分支与循环语句)
	* [函数](#函数)

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

## Hello Python

学习一门编程语言，首先要做的就是写Hello World，就像拿到一个单片机先点亮一盏led灯一样。进入Python命令行之后，每行都会有>>>提示符提醒你在后面输入命令，你输入的每一行Python语句都会被解释器直接转译运行。我们首先打印一个字符串"Hello Python":

		$ >>> print("Hello Python")

print是一个专门用来打印的函数，如果你学过C语言，那么这跟printf很像，但printf里的f是格式化Format的意思，Python没有格式化，所以没有f；  

打印“Hello Python”的另一种方式是写文件，该文件的后缀必须是.py，下面是一个示例(名为hello.py)：

		#! /usr/bin/env Python

		print("Hello Python")

然后运行：

		$ python hello.py

## Python基础

1. 注释  
	在Python文件中，每行从#开始的文字都是注释，这跟shell脚本一样，连特殊的第一行不为注释也是一样的；如果要使用多行注释，使用三个连续的双引号或但引号将注释内容括起来即可，这又跟github版markdown的代码块有些类似：

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
				1
				$ >>> a[1][0]
				2
				$ >>> a[1][1]
				3

		范围引用： `序列名[下限：上限：步长]`，其范围在数学上的区间表示为`[下限，上限)`，即不包括上限，默认步长为1

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

		尾部引用：

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

1. 分支语句    
关键字if、else、elif是Python的分支语句，从属于它们的代码必须要有四个空格的缩进，且if、else、elif判断后面需要加上一个引号

		if i < -1:
			x = -2
			y = -1
		elif i <= 0:
			x = -1
			y = 0
		else:
			x = 0
			y = 1

2. 循环语句
	* for循环

			for a in [1,2,True]:
				print(a)
			for i in range(5):
				print(i)            # 0,1,2,3,4，range(n)从0开始计数
			for i in range(1,5):
				print(i)            # 1,2,3,4，range(n,m)在数学区间上的定义为前闭后开，即[n,m)

	* while循环

			i = 0
			while i < 10:
				print("i = ", i)    # ('i = ',0),...,('i = ',9)
				i = i + 1  

	* 跳过或终止  
	continue用于跳过循环的当次执行，转而进行下一次循环；break用于停止剩余的循环。

			for i in range(10):
				if i == 2:
					continue
				if i == 8:
					break

				print(i)            # 0,1,3,4,5,6,7

## 函数

1. 函数的定义  
在关键字def后跟函数名，接着是参数列表，即使没有参数也必须带有括号；函数执行到return时就会结束，不管它后面是否含有其他函数语句；return并不是必需的，若没有return，则函数返回None，None是Python中的空数据；return也可以返回多个值，用逗号进行分隔；如果要给函数添加说明文档，在函数内容一开始的时候添加一个有缩进的多行注释即为函数说明文档，可以使用`help()`函数进行查看；

		def f(a,b):
			'''return the square sum of two arguments'''
			return (a**2 + b**2)

		def f1():
			return 1,2,3

		help(f)                     # return the square sum of two arguments
		c = f(2,3)                  # c = 13
		print(f1())                 # (1,2,3)
		print(type(f1()))           # tuple

2. 函数的调用  
	* 函数的基本调用是直接传入相应个数的实参到相应的函数，函数会根据传入数据的位置确认参数(这里传入的是引用，Python中的变量都是引用，引用的是实际存在内存中的对象，对象的id可以使用`id(var)`进行查看，当传递进函数的是可改变变量(如列表和词典)时，可以在函数内部对变量进行修改，否则只是改变了引用的对象，外部变量的引用并不会发生改变，这在后面会继续深入)；

			def f(a,b,c):
				print(a)
				print(b)
				print(c)

			f(1,2,3)                    # print 1,2,3

	* 函数还可以根据关键字进行传参，这里的关键字就是函数参数列表中个参数的名字，你可以随意以任何顺序为关键字赋值；你也可以将位置传递与关键字传递混合使用，但位置参数要出现在关键字参数之前；函数参数可以设置默认值，当不提供这些默认参数的值时，将使用默认值，C里面的默认参数是要从右边第一个开始直到结束，且中间不得穿插普通参数的；  

			def f(a,b,c,d=3,e=2,f=1):
				print(a)
				print(b)
				print(c)
				print(d)
				print(e)
				print(f)

			f(f=5,e=4,d=3,c=2,b=1,a=0)  # print 0,1,2,3,4,5
			f(1,2,e=3,f=4,d=5,c=6)      # print 1,2,6,5,3,4
			f(3,2,1)                    # print 3,2,1,3,2,1

	* 当函数参数个数不定的时候，还可以进行包裹传参，包裹传参也有位置和关键字两种形式，它们也可以混用；传递元组时在函数参数前加上一个星号，传递词典时加上两个星号：包裹传参与基本传参可以混用，但参数出现的顺序必须是：位置->关键字->包裹位置->包裹关键字；

			def f(* a):
				print(type(a))
				print(a)

			def f1(** a):
				print(type(a))
				print(a)

			def f2(* a,** b):
				print(a)
				print(b)

			f(1,3,4)                    # tuple, (1,3,4)
			f1(a=1,b=2)                 # dict, {'a': 1, 'b': 2}
			f2(1,2,3,a=3,b=2)           # (1,2,3), {'a': 3, 'b': 2}

	* 解包裹传参，若一个元组的元素个数与某个函数参数个数相同，那么使用一个星号加上这个元组名可以作为这个函数的参数，对应位置传参；词典则是加上两个星号，对应关键字传参；

			def f(a,b,c):
				print(a,b,c)

			l = (2,3,4)        
			f(* l)                      # print (2,3,4)

			x = { 'c': 2, 'b': 1, 'a': 9 }
			f(** x)                     # print (9,1,2)

3. 函数可以使用递归

		def f(n):
			if(n == 1):
				return 1
			else:
				return n + f(n - 1)

		print(f(3))                 # print 6

4. 函数的作用域  
函数可以看到外部已经存在的变量，但不能改变外部变量；如果本函数有与外面的重名的变量，那么优先使用本函数的变量；当函数返回时，相关参数会被清空；

		def f():
			print(info)
			a = "hahah"
			print(a)

		info = "heheh"
		a = "hhh"
		f()                         # print heheh, hahah
