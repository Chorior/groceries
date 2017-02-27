---
title:      "Markdown 基础语法"
subtitle:   " Hello Markdown "
date:       2017-02-27 14:00:00 +0800
header-img: "img/markdown-logo.png"
tags:
    - Markdown
---

本文参考自[daringfireball](http://daringfireball.net/projects/markdown/syntax)，加了一些自己使用markdown时的经验。

学习一门语法时，建议按照教程自己手动操作一下。

#   本文结构

*   [Markdown简介](#introduction)
*   [Markdown编辑器](#editor)
*   [Markdown基础语法](#Markdown基本语法)

<h2 id="introduction">Markdown简介</h2>

Markdown的优点是易读和易写，它的语法借鉴了一些已存在的text-to-HTML过滤器(包括Setext, atx, Textile, reStructuredText, Grutatext, and EtText)，它设计的最大灵感来源于纯文本邮件格式，它的主要意图是被用以网络格式的编写，它的语法可以说是HTML的子集，但HTML是发布的格式，Markdown是写的格式，如果有Markdown没有包含的语法,你可以直接使用HTML语法；

Markdown自动转换特殊字符:
*   &如果不在HTML中会被自动转换为`&amp`;
*   <如果没有格式的话会被自动转换为`&lt`；

但是在HTML中不会自动转换，这使得Markdown很容易进行HTML代码的编写；

<h2 id="editor">Markdown编辑器</h2>

<h3 id="editor-linux">Linux</h3>

ReText, Atom, vs code

<h3 id="editor-windows">Windows</h3>

MarkdownPad, MarkPad, MarkEditor, vs code

<h2 id="basic-syntax">Markdown基础语法</h2>

<h3 id="paragraphs">段落</h3>

段落是一个或多个连续的文本行，由一个或多个空白行分隔。（空白行是看起来像空行的任何行，即只包含空格或制表符的行被视为空白。）正常段落不应缩进空格或制表符：

```text
this is a paragraph

this is another paragraph
```

<h3 id="breaks">换行</h3>

行尾使用两个或更多空格，然后enter; 或者直接使用HTML `<br>`标签：

```text
line 1  
line 2 <br>
line 3
```

<h3 id="title">标题</h3>

Markdown支持两种样式的头文件，[Setext][1] 和 [atx][2];

[1]: http://docutils.sourceforge.net/mirror/setext.html
[2]: http://www.aaronsw.com/2002/atx/

Setext样式标题使用等号（对于第一级标题）和破折号（对于第二级标题）标注，任何数量得等号或破折号都可以正常工作，一般写三个:

```text
This is an H1
=============

This is an H2
-------------
```

Atx风格的标题在行的开头使用1-6个#字符，对应于标题级别1-6:

```text
# title1
## title2
### title3
```

你可以选择封闭式的Atx标题，只要在后面加上任意个数的#符号即可，虽然这没什么软用，但有些人觉得挺好看的：

```text
# title1 #
## title2 ##
### title3 ###
```

<h3 id="blockquotes">块引用</h3>

Markdown使用电子邮件样式>字符进行块引用，在每行前加上>即可：

```text
> This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
> consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
> Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.
>
> Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
> id sem consectetuer libero luctus adipiscing.
```

如果你懒得话，Markdown也允许你只在段落的第一行加上>

```text
> This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.

> Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
id sem consectetuer libero luctus adipiscing
```

块引用允许嵌套，只要添加相应个数个>即可：

```text
> This is the first level of quoting.
>
> > This is nested blockquote.
>
> Back to the first level.
```

块引用还可以包含其他Markdown元素，包括头，列表和代码块

```text
> ## This is a header.
> 
> 1.   This is the first list item.
> 2.   This is the second list item.
> 
> Here's some example code:
> 
>     return shell_exec("echo $input | $markdown_script");
>
> ```c++
> #include <iostream>
> int main()
> {
>   std::cout << "Hello World!\n";
>   return 0;
> }
> ```
``` 

<h3 id="lists">列表</h3>

Markdown支持有序（编号）和无序（项目符号）列表。

无序列表使用`*`号或`+`号或`-`号（可互换）作为列表标记，列表标记左侧最多缩进三个空格：

```text
*   Red
*   Green
*   Blue
```

有序列表使用数字后跟英文句点，但用于标记的数字对Markdown生成的HTML并没用什么软用，建议都写1

```text
1.  Bird
2.  McHale
3.  Parish
```

* 列表：
    * 无序列表，使用\*，+，-加空格 
    * 有序列表，数字（第一个数字最好是1,但不影响最终结果）加空格
```
    * Green   
    + Blue   
    -  Red   

    1. Green   
    3. Blue   
    8. Red 
```
* 使用`\.`显示点号,`\*`显示\*号；
* 代码块： 每行缩进4个空格或一个tab;代码块直到不再缩进或到达文章最后时结束；
* 分割线（水平线），使用3个或以上\*号或\-号：
```
    ***
    ---
    ******
    ---------------
```
* 链接，Markdown使用两种链接方式，行内链接（inline）和参考链接（refrence）:  
    * 行内链接：  
<pre>
      [an example](http://example.com/ "Title")，Title是可选的；
</pre>

    * 参考链接：
      使用两个方括号定义，第二个方括号的内容必须在另一个地方定义：  
<pre>
      [an example] [id]
      [id]: http://example.com/  "Optional Title Here"
</pre>
 或者：  
<pre>
      [an example][]
      [an example]: http://example.com/
</pre>
* 倾斜：
``` 
    _name_  
    *name*
```
* 加粗： 
```
    __name__  
    **name**
```
* 添加代码，使用\`或者\`\`\` ;  
* 添加图片：
```  
      ![Alt text](/path/to/img.jpg "Optional title")
 或者：  
      ![Alt text][id]
      [id]: url/to/image  "Optional title attribute"
```
* 自动转换链接：  
``` 
  <http://example.com/>  
  <address@example.com>
```
* 使用\\+特殊符号显示特殊符号。
