# Markdown 基础语法

本文参考自[daringfireball](http://daringfireball.net/projects/markdown/syntax)，加了一些自己使用markdown时的经验。

学习一门语法时，建议按照教程自己手动操作一下。

#   本文结构

*   [Markdown简介](#introduction)
*   [Markdown编辑器](#editor)
*   [Markdown基础语法](#basic-syntax)
    *   [段落](#paragraphs)
    *   [换行](#breaks)
    *   [标题](#title)
    *   [块引用](#blockquotes)
    *   [列表](#lists)
    *   [代码块](#code-blocks)
    *   [分割线](#split-lines)
    *   [链接](#links)
    *   [强调](#emphasis)
    *   [图片](#pictures)
    *   [其它](#basic-others)
*   [github扩展](#github-style)
    *   [任务列表](#task-lists)
    *   [表情符号](#emojis)
    *   [表格](#tables)
*   [Markdown总结](#summary)

<h2 id="introduction">Markdown简介</h2>

Markdown的优点是易读和易写，它的语法借鉴了一些已存在的text-to-HTML过滤器(包括Setext, atx, Textile, reStructuredText, Grutatext, and EtText)，它设计的最大灵感来源于纯文本邮件格式，它的主要意图是被用以网络格式的编写，它的语法可以说是HTML的子集，但HTML是发布的格式，Markdown是写的格式，如果有Markdown没有包含的语法,你可以直接使用HTML语法。

在HTML中，有两个字符需要特殊处理：<和&。左尖括号用于启动标签; ＆符号用于表示HTML实体。如果要将它们用作文字字符，则必须将它们转义为实体，例如`&lt;`和`&amp;`。

Markdown允许你自然地使用这两个字符，照顾所有必要的转义。如果使用＆符作为HTML实体的一部分，则它保持不变; 否则会被翻译成`&amp;`。

但是在链接或代码中，<和&总是自动编码，这使得Markdown很容易进行HTML代码的编写。

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
> github style: 
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

无序列表使用`*`号或`+`号或`-`号（可互换）作为列表标记。列表标记左侧最多缩进三个空格；列表标记右侧必须跟有一个或多个空格或制表符，建议使用制表符（当列表后跟缩进代码块时比较好看）：

```text
*   Red
*   Green
*   Blue
```

有序列表使用数字后跟英文句点，但用于标记的数字对Markdown生成的HTML并没用什么软用，建议都写1比较方便

```text
1.  Bird
2.  McHale
3.  Parish
```

如果列表文字较多，建议每行加个缩进，这样会比较好看：

```text
*   Lorem ipsum dolor sit amet, consectetuer adipiscing elit.
    Aliquam hendrerit mi posuere lectus. Vestibulum enim wisi,
    viverra nec, fringilla in, laoreet vitae, risus.
*   Donec sit amet nisl. Aliquam semper ipsum sit amet velit.
    Suspendisse id sem consectetuer libero luctus adipiscing.
```

如果列表项中间有空行,那么在输出 HTML 时 Markdown 就会将项目内容用`<p>`标签包起来：

```text
markdown:

*   Bird
*   Magic

html:

<ul>
<li>Bird</li>
<li>Magic</li>
</ul>

markdown:

*   Bird

*   Magic

html:

<ul>
<li><p>Bird</p></li>
<li><p>Magic</p></li>
</ul>
```

列表项可以由多个段落组成。列表项中的每个后续段落必须缩进为4个空格或一个制表符，如果你懒得话，也可以只缩进每段第一行：

```text
1.  This is a list item with two paragraphs. Lorem ipsum dolor
    sit amet, consectetuer adipiscing elit. Aliquam hendrerit
    mi posuere lectus.

    Vestibulum enim wisi, viverra nec, fringilla in, laoreet
    vitae, risus. Donec sit amet nisl. Aliquam semper ipsum
    sit amet velit.

2.  Suspendisse id sem consectetuer libero luctus adipiscing.
```

要在列表项中放置块引用，>分隔符需要缩进：

```text
*   A list item with a blockquote:

    > This is a blockquote
    > inside a list item.
```

**要将缩进代码块放在列表项中，代码块需要缩进两次 - 8个空格或2个制表符（但是github版用反引号标记的代码块却不需要）：**

````text
*   A list item with a code block:

        <code goes here>

*   A list item with a github code block:

```text
    I'm a code. 
```
````

如果你有一句话以数字结尾，那么可能触发一个有序列表:

```text
1986. What a great season.
```

解决方法是使用反斜杠转义句点：

```text
1986\. What a great season.
```

<h3 id="code-blocks">代码块</h3>

如果要标记一小段行内代码，你可以用反引号把它包起来:

```text
Use the `printf()` function.
```

要在代码范围内包含文字反引号字符，可以使用多个反引号作为开始和结束分隔符：

```text
``There is a literal backtick (`) here.``
```

要在Markdown中生成代码块，只需将该块的每一行缩进至少4个空格或1个制表符：

```text
This is a normal paragraph:

    This is a code block.
```

记住，要将缩进代码块放在列表项中，代码块需要缩进两次 - 8个空格或2个制表符。

```text
*   A list item with a code block:

        <code goes here>
```

代码块继续，直到它到达没有缩进的行（或到达文章结尾）。

github版的代码块支持使用反引号标记，并且可以选择添加语言标识ID将代码块高亮显示，支持的语言可以在[这里](https://github.com/github/linguist/blob/master/lib/linguist/languages.yml)找到:

`````text
```ruby
require 'redcarpet'
markdown = Redcarpet.new("Hello World!")
puts markdown.to_html
```
`````

如果你要在代码块中插入多个反引号（如Python的多行注释标记），那么用于Markdown代码块的反引号个数必须要多于代码块中的反引号个数:

``````text
`````text
```ruby
require 'redcarpet'
markdown = Redcarpet.new("Hello World!")
puts markdown.to_html
```
`````
``````

<h3 id="split-lines">分割线</h3>

你可以在一行中用三个以上的星号、减号、底线来建立一个分隔线，行内不能有其他东西。你也可以在星号或是减号中间插入空格。

```text
* * *

***

*****

- - -

---------------------------------------
```

<h3 id="links">链接</h3>

Markdown 支持两种形式的链接语法： 行内式和参考式。两种样式的链接文本都由第一个[方括号]包含。

<h4 id="link-inline">行内式</h4>

圆括号包含链接的URL:

```text
This is [an example](http://example.com/ "Title") inline link.

[This link](http://example.net/) has no title attribute.
```

<h4 id="link-reference">参考式</h4>

参考式链接使用两组方括号，第二组方括号放置您选择的标签以标识链接；方括号间可以用空格分割:

```text
This is [an example][id] reference-style link.

This is [an example] [id] reference-style link.
```

然后，在文档中的任何位置（建议放在段落下方，这样比较好管理），您可以这样定义您的链接标签：

```text
[id]: http://example.com/  "Optional Title Here"
```

链接内容定义的形式为：

*   包含链接标识符的方括号（可选地使用最多三个空格从左边距缩进）;
*   然后跟个冒号；
*   后跟一个或多个空格（或制表符）；
*   后面是链接的URL
*   可选地后跟链接的标题属性，用双引号或单引号括起来、或括在括号中。

链接URL可以可选地用尖括号括起来：

```text
[id]: <http://example.com/>  "Optional Title Here"
```

链接标识符可以由字母，数字，空格和标点符号组成，**但它们不区分大小写**，下面这两个链接是一样的

```text
[link text][a]
[link text][A]
```

隐含链接可以省略链接标识符，这时链接文本本身被用作链接标识符，然后使用一组空的方括号：

```text
[Google][]

[Google]: http://google.com/
```

隐含链接还可以使用多个单词的链接文本：

```text
Visit [Daring Fireball][] for more information.

[Daring Fireball]: http://daringfireball.net/
```

使用Markdown的参考链接，使得源文档更像是在浏览器中呈现的最终输出。通过允许您将与标记相关的元数据移出段落，您可以添加链接，而不会中断文章的叙述流程。

<h3 id="emphasis">强调</h3>

Markdown将星号（*）和下划线（_）作为强调的标记符号。

用一个*或_包含的文本在HTML输出中会发生倾斜:

```text
*single asterisks*

_single underscores_
```

用两个*或_包含的文本在HTML输出中会被加粗：

```text
**double asterisks**

__double underscores__
```

你可以使用任何你喜欢的风格; 唯一的限制是，你必须在前后使用使用相同的字符。

强调还可以在单词的中间使用：

```text
un*frigging*believable
```

如果不需要强调，或强调的文本中含有星号或下划线，那么需要使用进行反斜杠转义：

```text
\*this text is surrounded by literal asterisks\*

*\*this text is surrounded by literal asterisks\**
```

<h3 id="pictures">图片</h3>

Markdown 使用一种和链接很相似的语法来标记图片，同样也允许两种样式： 行内式和参考式。

<h4 id="picture-inline">行内式</h4>

```text
![Alt text](/path/to/img.jpg)

![Alt text](/path/to/img.jpg "Optional title")
```

<h4 id="picture-reference">参考式</h4>

```text
![Alt text][id]

[id]: url/to/image  "Optional title attribute"
```

Markdown没有用于指定图像尺寸的语法; 如果这对你很重要，你可以使用HTML的`<img>`标签。

<h3 id="basic-others">其它</h3>

<h4 id="auto-link">自动链接</h4>

Markdown支持为URL和电子邮件地址创建“自动”链接的快捷方式，只需用尖括号括起来即可：

```text
<http://example.com/>
<address@example.com>
```

<h4 id="backslash-escapes">反斜杠转义</h4>

Markdown为以下字符提供反斜杠转义：

```text
\   反斜线
`   反引号
*   星号
_   底线
{}  花括号
[]  方括号
()  括弧
#   井号
+   加号
-   减号
.   英文句点
!   惊叹号
```

<h2 id="github-style">github扩展</h2>

<h3 id="task-lists">任务列表</h3>

要创建任务列表，先列出列表项[ ]。要将任务标记为完成，请使用[x]。

```text
- [x] Finish my changes
- [ ] Push my commits to GitHub
- [ ] Open a pull request
```

- [x] Finish my changes
- [ ] Push my commits to GitHub
- [ ] Open a pull request

如果任务列表项描述以括号开头，则需要使用反斜杠进行转义：

```text
- [ ] \(Optional) Open a followup issue
```

- [ ] \(Optional) Open a followup issue

<h3 id="emojis">表情符号</h3>

您可以通过键入支持的表情符号代码添加emoji，支持的表情符号可以在[这里](http://www.webpagefx.com/tools/emoji-cheat-sheet/)找到。

```text
:+1: This PR looks great - it's ready to merge! :shipit:
```

:+1: This PR looks great - it's ready to merge! :shipit:

<h3 id="tables">表格</h3>

您可以通过组合单词列表​​并用连字符-（对于第一行）分隔，然后用管道符号\|分隔每个列来创建表，github会自动控制长宽：

```text
First Header | Second Header
------------ | -------------
Content from cell 1 | Content from cell 2
Content in the first column | Content in the second column
```

First Header | Second Header
------------ | -------------
Content from cell 1 | Content from cell 2
Content in the first column | Content in the second column

更多github扩展请点击 <https://guides.github.com/features/mastering-markdown/>

<h2 id="summary">Markdown总结</h2>

推荐一个不错的Markdown文章书写格式：

```text
#   标题

##  本文结构

*   [简介](#overview)
*   [环境搭建](#environment)
*   [操作流程](#operation)
    *   [第一步](#step1)
    *   [第二步](#step2)
    。。。
*   [总结](#summary)

<h2 id="overview">简介</h2>

所有标题建议使用HTML手动设置标识ID，默认的ID是不支持中文的，跳转时会发生问题。

<h2 id="environment">环境搭建</h2>

标识ID最好使用小写字母，多个单词使用下划线链接，这样复制粘贴导航栏时比较方便，也比较好改。

<h2 id="operation">操作流程</h2>

<h3 id="step1">第一步</h3>

建议使用github版的Markdown.

<h3 id="step2">第二步</h3>

你也可以使用其他格式书写Markdown文章。

。。。

<h2 id="summary">总结</h2>

开始使用Markdown写文章吧！

```