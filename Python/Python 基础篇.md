# Python 基础篇

Python 凭借其实用性与可扩展性，在各个行业飞速发展，**由其是数据科学(Data Science)、机器学习(Machine Learning)和学术研究(Academic Research)**，这三个方面都是当前 IT 行业最为火热的领域，所以熟悉甚至精通 Python 将使你更容易的找到心仪的工作、同时也能使你的工作和生活更加灵活和便捷。

# 本文结构

* [Python 初识](#overview)
* [Python 安装](#installation)
	*	[Linux](#linux)
	*	[Windows](#windows)
	*	[IDE](#integrated_development_environment)
* [Hello Python3](#hello-python3)
* [Python 基本数据类型与运算](#python-basis)
	*	[注释](#annotation)
	*	[基本运算](#basic-operation)
	*	[变量类型](#variable-type)
		*	[序列](#tuple-and-list)
		*	[词典](#dictionary)
* [分支与循环语句](#loop-and-branch)
	*	[分支语句](#branch)
	*	[循环语句](#loop)
* [函数](#function)
	*	[函数的定义](#function-definition)
	*	[函数的调用](#function-call)
	*	[函数的作用域](#function-scopes)
* [模块](#module)
* [异常处理](#exception)
* [类](#class)
* [各种对象](#object)
* [文件](#file)

<h2 id="overview">Python初识</h2>

Python 是一门跨平台的多范式编程语言，支持面向过程、面向对象、函数式及命令式编程，其作者是 Guido van Rossum。

Python 的优点在于其实用性和可扩展性，故而性能相比其它语言有些差距，但随着电脑速度的不断加快，性能的缺点逐渐被其优点所覆盖。

你可能在 Python2 和 Python3 间艰难的抉择，但是查看 [Python2orPython3](https://wiki.python.org/moin/Python2orPython3/) 后你应该知道：**Python3 才是 Python 的未来**。

<h2 id="installation">Python安装</h2>

<h3 id="linux">Linux</h3>

Linux 一般预装了 Python，直接打开终端，输入 `python` 后回车，你将看到安装的 Python 版本；最后的 `>>>` 是一个提示符，表示你能够继续输入 Python 命令；**退出 Python 命令行的命令是 `exit()`，快捷键是 `Ctrl + D`**：

```bash
$ python
Python 2.7.12 (default, Nov 19 2016, 06:48:10)
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

可以看到，默认的 Python 命令行是 python2，这并不是我们想要的，**要检查系统是否安装了 Python3，尝试运行命令 python3**：

```bash
$ python3
Python 3.5.2 (default, Sep 14 2017, 22:51:06)
[GCC 5.4.0 20160609] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

如果没有安装 python3 的话，执行如下命令来安装 python3:

```bash
$ sudo apt-get install python3.5
```

你可以把一系列 Python 命令写在一个后缀名为 `.py` 的文件中，然后使用 `python+空格+文件名` 来执行这些命令：

```bash
$ python3 hello.py
```

如果文件不在当前目录下，那么说明文件的完整路径即可，绝对或相对皆可：

```bash
$ python3 /home/pengzhen/python3/hello.py
$ pwd
/home/pengzhen
$ python3 python3/hello.py
```

如果要将 Python 文件改成一个可执行的脚本，在文件第一行加上 `#!/usr/bin/env python3`（这跟shell脚本有些相似），然后将其权限改为可执行即可：

```bash
$ chmod 755 hello.py
$ ./hello.py
```

<h3 id="windows">Windows</h3>

到[Python官网](https://www.python.org/downloads/)下载对应的安装包，安装时选择 Customize 来进行个性安装，另外勾选 `Add python.exe to Path`。

安装完成后，打开终端，输入 python 即可进入 python 命令行了：

```cmd
>python
Python 3.6.3 (v3.6.3:2c5fed8, Oct  3 2017, 18:11:49) [MSC v.1900 64 bit (AMD64)]
 on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> ^Z
```

与 Linux 不同，**Windows 下退出 Python 命令行的快捷键是 `Ctrl + Z`**。

<h3 id="integrated_development_environment">IDE</h3>

安装好 Python3 之后，选择一个舒适的 IDE 来编写 Python 程序至关重要。你可以查看 [SOF](https://stackoverflow.com/questions/81584/what-ide-to-use-for-python) 进行选择；但 Visual Studio 一直是我认为的最好的 IDE，没有之一，所以在这里**建议使用 [vscode](https://github.com/Microsoft/vscode) 作为 Python 的 IDE**。

vscode 是一款强大的、开源的跨平台文本编辑器，它拥有众多的插件来进行扩展，建议安装 Atom One Dark Theme 优化主题、vscode-icons 来支持各种文件图标、以及 python 来支持 python 编写与调试相关。

要应用安装的主题和图标插件的话，需要到用户设置中进行设置：

```yml
"workbench.colorTheme": "Atom One Dark",
"workbench.iconTheme": "vscode-icons"
```

<h2 id="hello-python3">Hello Python3</h2>

学习一门编程语言，首先要做的就是写 Hello World，就像拿到一个单片机先点亮一盏led灯一样。进入 Python 命令行之后，每行都会有 `>>>` 提示符提醒你在后面输入命令，你输入的每一行Python语句都会被解释器直接转译运行。

我们首先打印一个字符串 "Hello Python3!":

```bash
$ >>> print("Hello Python3!")
```

print 是一个专门用来打印的函数，如果你学过C语言，那么这跟 printf 很像，但 printf 里的f是格式化 Format 的意思，Python 没有格式化，所以没有f；  

打印 "Hello Python" 的另一种方式是写文件，该文件的后缀必须是 `.py`，下面是一个示例：

```python
#! /usr/bin/env Python3

print("Hello Python3!")
```

然后运行：

```bash
$ python3 hello.py
```

<h2 id="python-basis">Python 基本数据类型与运算</h2>

<h3 id="annotation">注释</h3>

在Python文件中，每行从#开始的文字都是注释，这跟shell脚本一样，连特殊的第一行不为注释也是一样的；如果要使用多行注释，使用三个连续的双引号或单引号将注释内容括起来即可：

`````bash
#! /usr/bin/env python
'''
Author: pengzhen
Function: print hello python
'''	
print("Hello Python") # print
`````

如果想在Python2中使用中文，须在程序开始前加上`# -*- coding: utf-8 -*-`:

```bash
# -*- coding: utf-8 -*-

print("你好！Python!")
```

<h3 id="basic-operation">基本运算</h3>

#### 数值运算(大体与C差不多，多了乘方)

```bash
>>> 1 + 9            # 加法。结果为10
>>> 1.3 - 4          # 减法。结果为-2.7
>>> 3*5              # 乘法。结果为15
>>> 4.5/1.5          # 除法。结果为3.0
>>> 3**2             # 乘方。结果为9
>>> 10%3             # 求余。结果为1
>>> "he" + "llo"     # 字符串相加。结果为"hello"
>>> "he" * 3         # 结果为"hehehe"
```

#### 逻辑运算(True表示真，False表示假)

```bash
>>> True and False   # and表示与逻辑。结果为False
>>> True or False    # or表示或逻辑。结果为True
>>> not True         # not表示非逻辑。结果为False
```

#### 判断表达式

```bash
>>> 1 == 1           # ==，等于。结果为True
>>> 1 != 1           # !=，不等于。结果为False
>>> 1 < 2            # <，小于。结果为True
>>> 1 <= 2           # <=，小于等于。结果为True
>>> 1 > 2            # >，大于。结果为False
>>> 1 >= 2           # >=，大于等于。结果为False
```

#### 运算优先级(相同优先级按从左到右顺序进行)

```text
括号： ()
乘方： **
乘除： * /
加减： + -
判断： == > >= < <=
逻辑： and or not
```

<h3 id="variable-type">变量类型</h3>

Python的变量根据数据自动转换类型，这称为动态类型，像C就是静态类型，C必须事先说好变量的类型才能进行赋值。查看一个Python变量的类型使用`type(var)`函数，常见的类型包括整型、浮点型、字符串和布尔型

```bash
$ >>> a = 10
$ >>> type(a)
```

<h4 id="tuple-and-list">序列</h4>

序列是一种容器类型，它有两种形式，元组(Tuple)和列表(List)，其区别在于元组元素不可改变，而列表可以。字符串是特殊的元组。元组圆括号括起，而列表用方括号括起，一个序列的元素可以是另一个列表:

```bash
$ >>> a = (1,2,3)   # tuple
$ >>> a = [3,2,1]   # list
$ >>> a = [1,(2,3)] # list
```

访问列表的方式与C的数组差不多，即用下标进行访问，下标从零开始

```bash
$ >>> a = [1,(2,3)]
$ >>> a[0]
1
$ >>> a[1][0]
2
$ >>> a[1][1]
3
```

范围引用： `序列名[下限：上限：步长]`，其范围在数学上的区间表示为`[下限，上限)`，即不包括上限，默认步长为1

```bash
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
```

尾部引用：

```bash
$ >>> a = [1,2,3,4,5]
$ >>> a[-1]   # 序列最后一个元素
5
$ >>> a[-3]   # 序列倒数第三个元素
3
$ >>> a[1:-1] # 序列第二个到倒数第二个元素
[2,3,4]
```

<h4 id="dictionary">词典</h4>

词典也是一个容器类型，但它不以位置作为索引，如果你知道键值对的话，那词典的类型就是键值对。使用大括号建立词典：

```bash
$ >>> a = {"a":1,"b":2,"c":3}
$ >>> a["a"]
1
$ >>> a["b"]
2
$ >>> a["c"]
3
$ >>> type(a) # dict
```

<h2 id="loop-and-branch">分支与循环语句</h2>

<h3 id="branch">分支语句</h3>

关键字if、else、elif是Python的分支语句，从属于它们的代码必须要有四个空格的缩进，且if、else、elif判断后面需要加上一个引号

```python
if i < -1:
	x = -2
	y = -1
elif i <= 0:
	x = -1
	y = 0
else:
	x = 0
	y = 1
```

<h3 id="loop">循环语句</h3>

#### for循环

```python
for a in [1,2,True]:
	print(a)
for i in range(5):
	print(i)            # 0,1,2,3,4，range(n)从0开始计数
for i in range(1,5):
	print(i)            # 1,2,3,4，range(n,m)在数学区间上的定义为前闭后开，即[n,m)
```

#### while循环

```python
i = 0
while i < 10:
	print("i = ", i)    # ('i = ',0),...,('i = ',9)
	i = i + 1
```

#### 跳过或终止

continue用于跳过循环的当次执行，转而进行下一次循环；break用于停止剩余的循环。

```python
for i in range(10):
	if i == 2:
		continue
	if i == 8:
		break

	print(i)            # 0,1,3,4,5,6,7
```

<h2 id="function">函数</h2>

<h3 id="function-definition">函数的定义</h3>

在关键字def后跟函数名，接着是参数列表，即使没有参数也必须带有括号；函数执行到return时就会结束，不管它后面是否含有其他函数语句；return并不是必需的，若没有return，则函数返回None，None是Python中的空数据；return也可以返回多个值，用逗号进行分隔；如果要给函数添加说明文档，在函数内容一开始的时候添加一个有缩进的多行注释即为函数说明文档，可以使用`help()`函数进行查看；

```python
def f(a,b):
	'''
	return the square sum of two arguments
	'''
	return a**2 + b**2

def f1():
	return 1,2,3

help(f)                     # return the square sum of two arguments
c = f(2,3)                  # c = 13
print(f1())                 # (1,2,3)
print(type(f1()))           # tuple
```

<h3 id="function-call">函数的调用</h3>

函数的基本调用是直接传入相应个数的实参到相应的函数，函数会根据传入数据的位置确认参数(这里传入的是引用，Python中的变量都是引用，引用的是实际存在内存中的对象，对象的id可以使用`id(var)`进行查看，当传递进函数的是可改变变量(如列表和词典)时，可以在函数内部对变量进行修改，否则只是改变了引用的对象，外部变量的引用并不会发生改变，这在后面会继续深入)；

```python
def f(a,b,c):
	print(a)
	print(b)
	print(c)

f(1,2,3)                    # print 1,2,3
```

函数还可以根据关键字进行传参，这里的关键字就是函数参数列表中个参数的名字，你可以随意以任何顺序为关键字赋值；你也可以将位置传递与关键字传递混合使用，但位置参数要出现在关键字参数之前；函数参数可以设置默认值，当不提供这些默认参数的值时，将使用默认值，C里面的默认参数是要从右边第一个开始直到结束，且中间不得穿插普通参数的；  

```python
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
```

当函数参数个数不定的时候，还可以进行包裹传参，包裹传参也有位置和关键字两种形式，它们也可以混用；传递元组时在函数参数前加上一个星号，传递词典时加上两个星号：包裹传参与基本传参可以混用，但参数出现的顺序必须是：位置->关键字->包裹位置->包裹关键字；

```python
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
```

解包裹传参，若一个元组的元素个数与某个函数参数个数相同，那么使用一个星号加上这个元组名可以作为这个函数的参数，对应位置传参；词典则是加上两个星号，对应关键字传参；

```python
def f(a,b,c):
	print(a,b,c)

l = (2,3,4)
f(* l)                      # print (2,3,4)

x = { 'c': 2, 'b': 1, 'a': 9 }
f(** x)                     # print (9,1,2)
```

函数可以使用递归

```python
def f(n):
	if(n == 1):
		return 1
	else:
		return n + f(n - 1)

print(f(3))                 # print 6
```

<h3 id="function-scopes">函数的作用域</h3>

函数可以看到外部已经存在的变量，如果想要修改外部全局变量，在函数内部在相应变量前加上global；如果本函数有与外面的重名的变量，那么优先使用本函数的变量；当函数返回时，相关参数会被清空；

```python
def f():
	print(info)
	a = "hahah"
	print(a)

	global b
	b = 10

info = "heheh"
a = "hhh"
b = 2
f()                         # print heheh, hahah
print(b)                    # print 10
```

<h2 id="module">模块</h2>

在Python中，一个.py文件就是一个模块，通过模块，你可以调用其他文件中的函数甚至数据；引入模块的方式与java差不多，使用关键字from和import进行引入；关于引入模块时，Python的搜索路径：首先是当前文件夹，然后是标准库(引入标准库模块不需要from)，再然后是环境变量PYTHONPATH所包含的路径；你可以通过引入sys，然后打印出sys.path来查看所有搜索的路径；

#### first,py

```python
def laugh():
	print("hahaha")

info = "hehe"
laugh()                      # print hahaha
```

#### second.py

```python
from first import laugh      # print hahaha
from first import info

laugh()                      # print hahaha
print(info)                  # print hehe
```

运行second.py，可以发现，输出打印了两行hahaha，这说明在引入first的时候，同时引入了first文件中的其它程序语句，这不是我们想要的，我们只想运行当前程序中的语句；这个时候我们就需要知道一些线程的知识，当某个程序是主线程的时候，那个线程的`__name__`属性为`__main__`，所以我们修改first.py：

```python
def laugh():
	print("hahaha")

info = "hehe"

if(__name__ == "__main__"):			
	laugh()                      # print hahaha
```

<h2 id="exception">异常处理</h2>

异常处理的目地有两个：一个是让程序中止前进行更多的操作；另一个是让程序犯错后能够继续正常运行；Python中的异常处理与C、java都差不多:

```python
while True:
	inputStr = input("input a number:") # input()，内置函数，用于接收命令行输入
	try:
		num = float(inputStr)           # 参数为字符串会触发ValueError异常
		print("input number: ", num)
		print(10/num)                   # 当num为0时，会触发ZeroDivisionError异常
	except ValueError:
		print("Illegal input.Try again.")
	except ZeroDivisionError:
		print("Illegal devision by zero.Try again")
```

异常处理的完整语法形式为：

```python
try:
	...
except exception0:       # 如果except后面没跟任何参数，那么所有的异常都交由该段程序处理
	...
except exception1:
	...
	...
else:
	...
finally:                 # 无论是否有异常最后都要做的事
	...
```

如果存在本段异常处理不能处理的异常，那么该异常会继续向上层抛出，直到被捕捉或造成主程序报错；关键字raise用于主动抛出异常；

```python
def f():
	try:
		1/0
	except ValueError:
		print("ValueError")
	except ZeroDivisionError:
		print("Illegal devision by zero")
		raise ZeroDivisionError()

try:
	f()
except ZeroDivisionError:
	print("Illegal devision by zero")

# 结果是Illegal devision by zero * 2
```

<h2 id="class">类</h2>

使用class关键字定义一个类，类中方法的第一个参数必须是self(即使没有参数)，这个self用于引用自身，然后是该方法需要的参数，在调用类中的方法时，不需要对self传值；类中的属性和方法使用对象.属性或对象.方法进行调用，这又与C和java类似；

```python
class Bird(object):
	feather = True

	def chirp(self,sound):
		print(sound)

summer = Bird()
print(summer.feather)        # print True
summer.chirp("jiojiojio")    # print jiojiojio
```

在类下方直接赋初值的属性称为类属性，在方法内部赋值的属性称为对象属性，为一个类添加对象属性很方便，直接使用`self.new_property`即可添加，这是因为类的对象属性由一个叫`__dict__`的词典进行管理，属性名为键，属性值为值，你可以通过打印`your_object.__dict__`查看这个词典；当你使用`self.new_property`时，Python查找这个词典，若没有找到`new_property`，则添加这个属性；

```python
class Bird(object):
	feather = True

	def chirp(self,sound):
		self.sound = sound   # 为Bird添加sound对象属性

summer = Bird()
summer.chirp("jiojiojio")
print(summer.sound)          # print jiojiojio
print(summer.__dict__)       # print {'sound': 'jiojiojio'}
```

Python有一系列特殊方法，如`__init__()`,`__add__()`,`__dict__()`等，这里我们只稍微讲几个；Python会在每次创建对象时自动调用`__init__()`方法；，如果类中定义了`__add__()`方法，那么类对象可以进行加法运算，相加时就是调用这个`__add__()`方法；相对的有`__sub__()`方法，对应减法；

```python
class Bird(object):
	def __init__(self,sound):
		self.sound = sound

summer = Bird("jiojiojio")
print(summer.sound)          # print jiojiojio
```

继承类：上面类名后都有一个括号，括号里是object，代表上面的类都是继承自object类，所以如果要继承一个类，在类名后的括号里写上要继承的类即可；继承类对象可以使用父类的属性和方法，继承类也可以覆盖父类的属性和方法，继承类当然也可以新增自己的属性和方法；可以在类里使用`super(current_class,self).function()`(python 2)或者`super().function()`(python 3)调用父类中被覆盖的方法;

```python
class Bird(object):
	def chirp(self):
		print("jiojiojio")

class Chicken(Bird):
	def chirp(self):
		super(Chicken,self).chirp()
		print("jijiji")

summer = Chicken()
summer.chirp()               # print jiojiojio, jijiji
```

<h2 id="object">各种对象</h2>

所谓对象，就是类的一个实例，你可以使用`dir()`函数来查看一个类或者对象的所有属性，你也可以使用`help()`查看函数或类的说明文档。类的说明文档与函数的说明文档类似，在类定义下用多行注释即可：

```python
class HelpDemo(object):
	'''
	This is a demo for using help() on a class
	'''

	pass                     # 关键字passa，用于说明该语法结构中什么都不做

help(HelpDemo)
```

可以通过词典的`key()`方法循环遍历每个元素的键，通过词典的`values()`循环遍历每个元素的值，或者通过`items()`直接遍历每个元素，`clear()`直接清空词典：

```python
my_dict = {"a":1, "b":2}

for k in my_dict.keys():
	print(k)                 # a, b

for v in my_dict.values():
	print(v)                 # 1, 2

for k,v in my_dict.items():
	print(k,v)               # ('a', 1), ('b', 2)
```

循环对象：当类中包含有`__next__()`方法(python 2.7中为`next()`方法)时，一个该类的对象可以通过该方法生成下一个对象，当生成过循环所有的结果之后，该方法会抛出`StopIteration`异常。当for使用循环对象时，每次循环的时候会调用`__next__()`方法，直到出现`StopIteration`异常。内置函数`iter()`可以将一个序列或词典转换为循环对象：

```python
my_iter = iter([1,2])
print(my_iter.next())        # print 1
print(my_iter.next())        # print 2
print(my_iter.next())        # StopIteration 异常

for item in iter([1,2]):     # python 2.7的for循环直接从序列出对象。不会转成循环对象
	print(item)
```

循环对象的好处：所有要使用的元素在循环过程中逐渐生成，节省了空间，并且提高了效率。python 3的`range()`返回的就是一个循环对象，但python 2.7中返回的是一个序列。  
可以借助生成器来定义循环对象。生成器的编写方法与函数定义类似，只是将return改为yield。生成器中可以有多个yield，当生成器遇到一个yield时，会暂停运行生成器，返回yield后面的值，再次调用生成器时，从上次暂停的地方继续运行，直到下一个yield：

```python
def gen():
	yield 0
	yield 2
	yield 4

a = gen()
type(a)                      # generator
a.next()                     # 0
a.next()                     # 2
a,next()                     # 4
a.next()                     # StopIteration 异常
```

函数对象：任何一个有`__call__()`特殊方法的对象都是函数对象：

```python
class Sample(object):
	def __call__(self,a):
		return a + 5

add_five = Sample()
add_five(2)                  # 7
```

模块对象：

```python
import time
import my_time
import datetime as dt

time.sleep(10)
my_time.sleep(10)
dt.datetime(2016,12,24,21,20)
```

可以将功能相似的模块放在同一个目录下，构成一个模块包：`import this_dir.module`。`this_dir`文件夹必须包含一个`__init__.py`文件，用于提醒python，该文件夹是一个模块包，`__init__.py`可以为空。每个模块对象都有一个`__name__`属性，用于记录模块名。当一个.py文件作为主程序运行时，合格文件会有一个对应的模块对象，该模块对象的`__name__`属性为`"__main__"`。

```python
import time

print(time.__name__)         # time
```

<h2 id="file">文件</h2>
