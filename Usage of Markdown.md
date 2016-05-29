# Usage of Markdown
---
1. 简介：  
  Markdown的优点是易读和易写，它的语法借鉴了一些已存在的text-to-HTML过滤器(包括Setext, atx, Textile, reStructuredText, Grutatext, and EtText)，它设计的最大灵感来源于纯文本邮件格式，它的主要意图是被用以网络格式的编写，它的语法可以说是HTML的子集，但HTML是发布的格式，Markdown是写的格式，如果有Markdown没有包含的语法。你可以直接使用HTML语法；  
  Markdown自动转换特殊字符:  
    &如果不在HTML中会被自动转换为`&amp`;  
    <如果没有格式的话会被自动转换为`&lt`；  
  但是在HTML中不会自动转换，需要自己转换，这使得Markdown很容易进行HTML代码的编写；
2. 编辑器：  
  1. linux:  
      ReText,Atom  
  2. Windows:  
      MarkdownPad,MarkPad,MarkEditor
3. 段落：用空格或者tab缩进;
4. 换行：  
  1. 使用`<br></br>`;  
  2. 行尾使用两个或更多空格，然后enter;
5. 两种标题格式：
  1. Setext-style:  
    一级标题用等号标注，二级标题用破折号，如下所示：
<pre>
      This is an H1  
      =============  

      This is an H2  
      -------------  
</pre>  
  2. Atx-style:  
  一到六个#号，用以代表1-6级标题：
  <pre>
  # title1
  ## title2
  ### title3
  </pre>
  你可以选择关闭Atx-style标题，只要在后面加上相应#符号即可：
  <pre>
  # title1 #
  ## title2 ##
  ### title3 ###
  </pre>
6. 引用： 使用<符号，嵌套引用加上相应个数的<即可：
<pre>
    > This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
    >> consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
    >>> Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.
    >
    > Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
    > id sem consectetuer libero luctus adipiscing.
</pre>
7. 列表：
  1. 无序列表，使用*，+，-加空格：  
    ` * Green `  
    ` + Blue `  
    `-  Red `  
  2. 有序列表，数字（第一个数字最好是1,但不影响最终结果）加空格：  
    ` 1. Green `  
    ` 3. Blue `  
    ` 8. Red `  
8. 使用`\.`显示点号,`\*`显示*号；
9. 代码块： 每行缩进4个空格或一个tab;代码块直到不再缩进或到达文章最后时结束；
10. 分割线（水平线），使用3个或以上*号或号：
  <pre>
  ***
  ---
  ******
  ---------------
  </pre>
11. 链接，Markdown使用两种链接方式，行内链接（inline）和参考链接（refrence）:  
  1. 行内链接：  
      `[an example](http://example.com/ "Title")`，Title是可选的；
  2. 参考链接：
      使用两个方括号定义，第二个方括号的内容必须在另一个地方定义：  
      <pre>
        `[an example] [id]`
        `[id]: http://example.com/  "Optional Title Here"`
      </pre>
      或者：  
      <pre>
        `[an example][]`
        `[an example]: http://example.com/`
      </pre>
12. 倾斜：  
    `_name_`  
    `*name*`
13. 加粗：  
    `__name__`  
    `**name**`
14. 添加代码，使用\`或者\`\` ;  
15. 添加图片：  
  `![Alt text](/path/to/img.jpg "Optional title")`  
  或者：  
  `![Alt text][id]`
  `[id]: url/to/image  "Optional title attribute"`
16. 自动转换链接：  
  `<http://example.com/>`  
  `<address@example.com>`
17. 使用\\+特殊符号显示特殊符号。
