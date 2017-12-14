# Python 知识要点

Python 凭借其实用性与可扩展性，在各个行业飞速发展，**由其是数据科学(Data Science)、机器学习(Machine Learning)和学术研究(Academic Research)**，这三个方面都是当前 IT 行业最为火热的领域，所以熟悉甚至精通 Python 将使你更容易的找到心仪的工作、同时也能使你的工作和生活更加灵活和便捷。

# 本文结构

* [Python 初识](#overview)
* [Python 安装](#installation)
	*	[Linux](#linux)
	*	[Windows](#windows)
	*	[IDE](#integrated_development_environment)
* [Hello Python3](#hello_python3)
* [Python 基本数据类型与运算](#python_basis)
	*	[注释](#annotation)
	*	[基本运算](#basic_operation)
	*	[变量类型](#variable_type)
		*	[字符串](#string)
		*	[序列](#tuple_and_list)
		*	[词典](#dictionary)
		*	[集合](#set)
* [分支与循环语句](#loop_and_branch)
	*	[分支语句](#branch)
	*	[循环语句](#loop)
* [函数](#function)
	*	[函数的定义](#function_definition)
	*	[函数的调用](#function_call)
	*	[函数的作用域](#function_scopes)
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

```python
$ python
Python 2.7.12 (default, Nov 19 2016, 06:48:10)
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

可以看到，默认的 Python 命令行是 python2，这并不是我们想要的，**要检查系统是否安装了 Python3，尝试运行命令 python3**：

```python
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

```python
>python
Python 3.6.3 (v3.6.3:2c5fed8, Oct  3 2017, 18:11:49) [MSC v.1900 64 bit (AMD64)]
 on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> ^Z
```

与 Linux 不同，**Windows 下退出 Python 命令行的快捷键是 `Ctrl + Z + Enter`**。

<h3 id="integrated_development_environment">IDE</h3>

安装好 Python3 之后，选择一个舒适的 IDE 来编写 Python 程序至关重要。你可以查看 [SOF](https://stackoverflow.com/questions/81584/what-ide-to-use-for-python) 进行选择；但 Visual Studio 一直是我认为的最好的 IDE，没有之一，所以在这里**建议使用 [vscode](https://github.com/Microsoft/vscode) 作为 Python 的 IDE**。

vscode 是一款强大的、开源的跨平台文本编辑器，它拥有众多的插件来进行扩展，建议安装 Atom One Dark Theme 优化主题、vscode-icons 来支持各种文件图标、以及 python 来支持 python 编写与调试相关。

要应用安装的主题和图标插件的话，需要到用户设置中进行设置：

```yml
"workbench.colorTheme": "Atom One Dark",
"workbench.iconTheme": "vscode-icons"
```

<h2 id="hello_python3">Hello Python3</h2>

学习一门编程语言，首先要做的就是写 Hello World，就像拿到一个单片机先点亮一盏led灯一样，这是为了检验开发环境是否正确可用。

进入 Python 命令行之后，每行都会有 `>>>` 提示符提醒你在后面输入命令，你输入的每一行Python语句都会被解释器直接转译运行。

我们首先打印一个字符串 "Hello Python3!":

```python
>>> print("Hello Python3!")
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

<h2 id="python_basis">Python 基本数据类型与运算</h2>

<h3 id="annotation">注释</h3>

在 Python 文件中，**每行从#开始的文字都是注释**，这跟 shell 脚本一样；如果要使用**多行注释，使用三个连续的双引号或单引号将注释内容括起来**即可：

`````python
#! /usr/bin/env python3
'''
Author: pengzhen
Function: print hello python3
'''	
print("Hello Python3!") # print
`````

<h3 id="basic_operation">基本运算</h3>

#### 数值运算(注意除法和乘方)

```python
>>> 1 + 9            # 加法。结果为10
>>> 1.3 - 4          # 减法。结果为-2.7
>>> 3*5              # 乘法。结果为15
>>> 3/2              # 除法。结果为1.5
>>> 3**2             # 乘方。结果为9
>>> 10%3             # 求余。结果为1
```

#### 逻辑运算(True表示真，False表示假)

```python
>>> True and False   # and表示与逻辑。结果为False
>>> True or False    # or表示或逻辑。结果为True
>>> not True         # not表示非逻辑。结果为False
```

#### 判断表达式

```python
>>> 1 == 1           # ==，等于。结果为True
>>> 1 != 1           # !=，不等于。结果为False
>>> 1 < 2            # <，小于。结果为True
>>> 1 <= 2           # <=，小于等于。结果为True
>>> 1 > 2            # >，大于。结果为False
>>> 1 >= 2           # >=，大于等于。结果为False
```

#### 运算优先级(相同优先级按从左到右顺序运行)

```text
括号： ()
乘方： **
乘除： * /
加减： + -
判断： == > >= < <=
逻辑： and or not
```

<h3 id="variable_type">变量类型</h3>

**Python 的变量会根据数据自动转换类型，这称为动态类型**，像C就是静态类型，C必须事先说好变量的类型才能进行赋值。

Python 变量名的规则与C一样，**只能包含字母、数字和下划线，且数字不能打头，也不能与关键字或函数重名**。

你可以**使用 `type(var)` 函数查看一个 Python 变量的类型**，常见的标准类型包括整型(int)、浮点型(float)、布尔型(bool)、字符串(str)、列表(list)、元组(tuple)、词典(dict)和集合(set)。

```python
>>> a = 10; type(a)
<class 'int'>
>>> a = 1.0; type(a)
<class 'float'>
>>> a = True; type(a)
<class 'bool'>
>>> a ="hello"; type(a)
<class 'str'>
>>> a = b"hello"; type(a)
<class 'bytes'>
>>> a = 2 + 3.8j; type(a)
<class 'complex'>
```

<h4 id="string">字符串</h4>

Python 中的**字符串可以用单引号或双引号括起**，这使得你可以在双引号字符串中使用单引号而无需使用转义字符 `\`，反之亦可：

```python
>>> print("I'm OK")
I'm OK
>>> print('"Python is awesome"')
"Python is awesome"
```

如果字符串里既包括单引号又包括双引号的话，你需要使用转义字符 `\`：

```python
>>> print('I\'m "OK"')
I'm "OK"
```

**多行字符串除了可以使用常见的 `\n`之外，还可以使用三个单引号或双引号括起**：

```python
>>> print('hello\nworld')
hello
world
>>> print('''hello
... world''')       # ... 是提示符，表示可以继续输入
hello
world
```

常用函数：

函数 | 功能(空白：空格、制表符(\t)和换行符(\n))
---------| ---------------------------------
`str1 + str2` | 返回 str1 和 str2 拼接后的字符串
`str * N` | 返回 N 个 str 拼接后的字符串
`title()` | 返回将字符串中所有的单词的首字母大写，其它字母小写的字符串
`upper()` <br> `lower()` | 返回所有字母大写或小写的字符串
`rstrip()` | 返回删除字符串末尾空白后的字符串
`lstrip()` | 返回删除字符串开头空白后的字符串
`strip()` | 返回删除字符串两端空白的字符串

我们经常需要将数字或其它非字符串类型表示为字符串，这时我们可以使用 `str(var)`：

```python
>>> str(True)
'True'
>>> str(2.0)
'2.0'
```

<h4 id="tuple_and_list">序列</h4>

序列是一种容器类型，你**可以将任何东西加入到序列之中**。序列有两种形式，元组(Tuple)和列表(List)，其区别在于**元组元素不可改变，而列表可以**。

**字符串是特殊的元组**。

**元组用圆括号括起，而列表用方括号括起，元素间使用逗号进行分隔**：

```python
>>> a = (1,2,3)   # tuple
>>> a = [3,2,1]   # list
>>> a = [1,(2,3)] # list
```

**序列是有序集合**，访问它的方式与C的数组差不多，即**用下标进行访问，下标从零开始**：

```python
>>> a = [1,(2,3)]
>>> a[0]
1
>>> a[1][0]
2
>>> a[1][1]
3
```

范围引用：**`序列名[下限：上限：步长]`，其范围在数学上的区间表示为`[下限，上限)`，即不包括上限，默认下限为0、上限为序列最大下标加一、步长为1**：

```python
>>> a = [0,1,2,3,4]
>>> a[:] # 广泛应用于复制列表，因为b=a是引用
[0, 1, 2, 3, 4]
>>> a[:2]
[0, 1]
>>> a[3:]
[3, 4]
>>> a[3:5]
[3, 4]
>>> a[0:5:2]
[0, 2, 4]
>>> a[3:0:-1]
[3, 2, 1]
>>> type(a)
<class 'list'>
```

尾部引用：

```python
>>> a = [0,1,2,3,4]
>>> a[-1]   # 序列最后一个元素
4
>>> a[-3]   # 序列倒数第三个元素
2
>>> a[1:-1] # 序列第二个到倒数第二个元素
[1, 2, 3]
```

列表常用函数：

函数 | 功能
---------| ---------------------------------
`len(list)` | 获取列表长度，也适用于元组
`item in list` | 判断item是否存在于列表之中
`del list[i]` | 删除索引为i的元素
`pop()` | 删除列表**末尾**的元素，并返回它
`list.pop(i)` | 删除索引为i的元素，并返回它
`list.remove(value)` | 删除第一个与value相等的元素
`list.insert(i,item)` | 在索引为i的位置插入一个item
`list.sort()` | 将元素按从小到大顺序进行排序，元素间必须是可比的
`sorted(list)` | 返回排序后的列表，原列表顺序不变
`list.reverse()` | 反转列表的排列顺序
`min(list)` <br> `max(list)` <br> `sum(list)` | 只对数字列表有效

我们经常需要判断序列是否为空，你可以使用 `len` 函数，但你可以直接将序列名用作条件表达式，当序列为空时返回 False：

```python
>>> a = []
>>> not a
True
```

有关序列遍历的操作，可以查看下面的[循环语句](#loop)。

<h4 id="dictionary">词典</h4>

词典也是一个容器类型，但它不以位置作为索引，如果你知道键值对的话，那**词典的类型就是键值对**。

**词典使用大括号括起，键与值之间使用冒号分隔，键值对之间使用逗号分隔。由于词典不是有序集合，所以你只能按关键字进行访问**：

```python
>>> a = {"a":1,"b":2,"c":3}
>>> a['a']
1
>>> a['b']
2
>>> a['c']
3
>>> type(a)
<class 'dict'>
```

当你**为一个词典中不存在的关键字赋值时，实际上赋值结束之后，相当于为词典新添加了一个键值对**：

```python
>>> a = {}
>>> a['a'] = 1
>>> a
{'a': 1}
```

删除键值对：

```python
>>> a = { 'a':1,'a':2}
>>> a
{'a': 2}
>>> del a['a']
>>> a
{}
```

在查看完下面的[循环语句](#loop)后，我们需要对词典进行遍历：

**遍历所有键值对**：

```python
a = {'a':1,'b':2,'c':3}

for key,value in a.items():
    print(key,value)
```

**遍历所有键**：

```python
a = {'a':1,'b':2,'c':3}

for key in a.keys():
    print(key)
```

**遍历所有值**：

```python
a = {'a':1,'b':2,'c':3}

for value in a.values():
    print(value)
```

<h4 id="set">集合</h4>

**集合类似于列表，但每个元素都是独一无二的**：

```python
>>> a = {1,2,1,2,3}
>>> a
{1, 2, 3}
>>> type(a)
<class 'set'>
>>> a = [1,2,2,1]
>>> set(a)
{1, 2}
```

<h2 id="loop_and_branch">分支与循环语句</h2>

<h3 id="branch">分支语句</h3>

关键字 if、elif、else 是 Python 的分支语句，**从属于它们的代码必须要有四个空格的缩进，且后面需要加上一个引号**：

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

多个条件可以使用逻辑符 and,or,not：

```python
if i >= - 1 and i <= 0:
    i = -1
if i <= - 1 or i >= 0:
    i = 0
if not i > 0:
    i = -1
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
    print("i =", i)
    i = i + 1
```

#### 跳过或终止

continue 用于跳过循环的当次执行，转而进行下一次循环；break 用于停止剩余的循环。

```python
for i in range(10):
    if i == 2:
        continue
    if i == 8:
        break

    print(i)            # 0,1,3,4,5,6,7
```

#### 列表解析

```python
>>> squares = [value**2 for value in range(5)] # 表达式用于生成需要存储的值
>>> squares
[0, 1, 4, 9, 16]
```

<h2 id="function">函数</h2>

函数是带名字的代码块，用以执行某项特定的任务。

例如当你需要**获取用户输入时，你可以调用 `input(str)`**，其中 str 用于描述输入说明：

```python
>>> name = input('Enter your name: ')
Enter your name: pengzhen
>>> name
'pengzhen'
>>> type(name)
<class 'str'>
```

<h3 id="function_definition">函数的定义</h3>

*	**关键字 def 告诉 Python 你要定义一个函数，后跟函数名，接着是参数列表，即使没有参数也必须带有括号**；
*	**定义以冒号结尾，后面所有的缩进行构成函数体**；
*	如果要给函数添加说明文档，**在函数体开始的时候添加一个有缩进的多行注释即为函数说明文档**，之后可以使用 `help(func)` 函数进行查看；
*	函数执行到 return 时就会结束，不管它后面是否含有其他函数语句；
*	**return 可以返回多个值，用逗号进行分隔，返回类型为元组**；
*	return 并不是必需的，若没有 return，则函数返回 None，None 是 Python 中的空数据。

```python
#! /usr/bin/env python3

def f(a,b):
    '''
    返回两个数的平方和
    '''
    return a**2 + b**2

def f1():
    return 1,2,3

help(f)                     # 按q退出
print('f(2,3) = ',f(2,3))   # 13
print(f1())                 # (1,2,3)
print(type(f1()))           # tuple
```

<h3 id="function_call">函数的调用</h3>

**函数的基本调用是直接传入相应个数的实参到相应的函数，函数会根据传入数据的位置确认参数**(这里传入的是引用，Python 中的变量都是引用，引用的是实际存在内存中的对象，**对象的id可以使用 `id(var)` 进行查看**，**当传入的是可改变变量(如列表和词典)时，可以在函数内部对变量进行修改，否则只是改变了引用的对象**，这在后面会继续深入)；

实参未发生变化：

```python
#! /usr/bin/env python3

def f(a,b):
    print(a,b)

    a = 3
    b = 4

a = 1
b = 2

f(a,b)      # 1 2

print(a,b)  # 1 2
```

实参被修改了(**避免修改传入 `a[:]`**)：

```python
#! /usr/bin/env python3

def f(a,b):
    a.insert(0,1)
    b.clear()

a = []
b = [1,2]

f(a,b)

print(a)    # [1]
print(b)    # []
```

**函数还可以根据关键字进行传参**，这里的关键字就是函数参数列表中各参数的名字，你**可以随意以任何顺序为关键字赋值**；你也**可以将位置传递与关键字传递混合使用，但位置参数要出现在关键字参数之前**；函数参数可以设置默认值，当不提供这些默认参数的值时，将使用默认值，**默认参数必须排列在没有默认值的形参之后**：

```python
#! /usr/bin/env python3

def f(a,b,c,d=3,e=2,f=1):
    print(a,b,c,d,e,f)

f(f=5,e=4,d=3,c=2,b=1,a=0)  # 0 1 2 3 4 5
f(5,4,e=1,f=0,d=2,c=3)      # 5 4 3 2 1 0
f(3,2,1)                    # 3 2 1 3 2 1
```

**当函数参数个数不定的时候，还可以进行包裹传参**，包裹传参也有位置和关键字两种形式，它们也可以混用：

*	**形参名前加上一个星号表示传入的所有参数都将被加入到一个空元组里面**，这个元组的名字就是该形参的名字；
*	**形参名前加上两个星号表示传入的所有键值对都将被加入到一个空词典里面**，这个词典的名字就是该形参的名字；
*	**包裹传参与基本传参可以混用，但参数出现的顺序必须是：位置->关键字->包裹位置->包裹关键字**。

```python
#! /usr/bin/env python3

def f0(* a):
    print(type(a))
    print(a)

def f1(** a):
    print(type(a))
    print(a)

def f2(* a,** b):
    print(a)
    print(b)
	
def f3(a,*b,**c):
    print(a)
    print(b)
    print(c)

print('f0(1,2,3):')
f0(1,2,3)   # (1, 2, 3)

print('\nf1(a=1,b=2):')
f1(a=1,b=2) # {'a': 1, 'b': 2}

print('\nf2(1,2,3,c=4,d=5):')
f2(1,2,3,c=4,d=5)

print('\nf3(1,2,3,4,e=5,f=6):')
f3(1,2,3,4,e=5,f=6)
```

解包裹传参，**若一个元组的元素个数与某个函数参数个数相同，那么使用一个星号加上这个元组名可以作为这个函数的参数，对应位置传参；词典则是加上两个星号，对应关键字传参**：

```python
#! /usr/bin/env python3

def f(a,b,c):
    print(a,b,c)

l = (2,3,4)
f(*l)                      # 2 3 4

x = { 'c': 2, 'b': 1, 'a': 9 }
f(**x)                     # 9 1 2
```

函数可以递归：

```python
#! /usr/bin/env python3

def f(n):
    if(n == 1):
        return 1
    else:
        return n + f(n - 1)

print(f(3))                 # 3+2+1
```

<h3 id="function_scopes">函数的作用域</h3>

*	**函数可以看到外部已经存在的变量**，如果**想要修改外部全局变量，在函数内部在相应变量前加上 global**；
*	**如果本函数有与外面的重名的变量，那么优先使用本函数的变量**；
*	当函数返回时，相关参数会被清空。

```python
#! /usr/bin/env python3

def f():
    print(info)

    a = "hahah"

    global b
    b = 10

info = "heheh"
a = "hhh"
b = 2

f()                         # heheh

print(a)                    # hhh
print(b)                    # 10
```

<h2 id="module">模块</h2>

*	在 Python 中，**一个 `.py` 文件就是一个模块**，通过模块，你可以调用其他文件中的函数甚至数据；
*	使用关键字 import 引入模块；
*	**关于引入模块时，Python 的搜索路径：首先是当前文件夹，然后是标准库，再然后是环境变量 PYTHONPATH 所包含的路径**；

你可以通过引入 sys，然后打印出 `sys.path` 来查看所有搜索的路径：

```python
>>> import sys
>>> sys.path
['', '/usr/lib/python35.zip', '/usr/lib/python3.5', '/usr/lib/python3.5/plat-x86_64-linux-gnu', 
'/usr/lib/python3.5/lib-dynload', '/usr/local/lib/python3.5/dist-packages', '/usr/lib/python3/dist-packages']
```

假设我们有一个 laugh.py 文件：

```python
def laugh1():
    print("laugh1")

def laugh2():
    print("laugh2")

msg = "hah"
laugh1()
```

#### 导入整个模块

```python
#! /usr/bin/env python3

import laugh

laugh.laugh1()
laugh.laugh2()
print(laugh.msg)
```

#### 导入特定的函数或变量

```python
#! /usr/bin/env python3

from laugh import laugh1,msg

laugh1()
print(msg)
```

#### 使用 as 给模块指定别名

```python
#! /usr/bin/env python3

import laugh as l

l.laugh1()
l.laugh2()
print(l.msg)
```

#### 导入模块中的所有函数和变量

```python
#! /usr/bin/env python3

from laugh import *

laugh1()
laugh2()
print(msg)
```

运行上面的 `.py` 文件，可以发现，每次输出都会打印出 laugh1，这说明**在引入模块的时候，同时引入了模块中的其它程序语句**，这不是我们想要的，我们只想运行当前程序中的语句。这个时候我们就需要知道一些线程的知识，**当某个程序是主线程的时候，那个线程的 `__name__` 属性为 `__main__`**，所以我们修改 laugh.py：

```python
def laugh1():
    print("laugh1")

def laugh2():
    print("laugh2")

msg = "hah"

if(__name__ == "__main__"):			
	laugh1()
```

<h2 id="exception">异常处理</h2>

**异常处理的目地有两个：一个是让程序中止前进行更多的操作；另一个是让程序犯错后能够继续正常运行**:

```python
#! /usr/bin/env python3

while True:
    inputStr = input("输入一个数字:")
    try:
        num = float(inputStr)           # 参数为字符串会触发ValueError异常
        print('1 /',num,'=',1/num)      # 当num为0时，会触发ZeroDivisionError异常
    except ValueError:
        print("输入不可用，请重新输入")
    except ZeroDivisionError:
        print("输入不能为0，请重新输入")
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

**如果存在本段异常处理不能处理的异常，那么该异常会继续向上层抛出，直到被捕捉或造成主程序报错**：

```python
#! /usr/bin/env python3

def f():
    try:
        1/0
    except ValueError:
        print("ValueError")
    except ZeroDivisionError:
        print("Illegal devision by zero")
        raise ZeroDivisionError()  # 主动抛出异常

try:
    f()
except ZeroDivisionError:
    print("Illegal devision by zero")
finally:
    print('finished')
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
