===================
RestructuredText
===================


RestructuredText (reST) is a markup language, it’s name coming from that it’s considered a revision and reinterpretation of two other markup languages, Setext and StructuredText.

https://github.com/pydanny/restructuredtext

https://restructuredtext.readthedocs.io/en/latest/

reStructuredText 是一种轻量级的文本标记语言, 直译为: 重构建的文本, 为 Python 中 Docutils 项目的一部分. 其一般保存的文件以 .rst 为后缀. 在必要的时候, .rst 文件可以被转化成 PDF 或者 HTML 格式, 也可以由 Sphinx 转化为 LaTex , man 等格式, 现在被广泛的用于程序的文档撰写.

https://www.sphinx-doc.org/en/master/



Sphinx, like Python, uses meaningful whitespace. Blocks of content are structured based on the indention level they are on. You can see this concept with our code-block directive above.

.. code-block:: bash

    mkvirtualenv crawler
    pip install crawler


$ pip install Sphinx
$ sphinx-quickstart



... https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html#sections


a_new_link_

.. _a_new_link:  https://github.com/nkypy/blog/issues/18


one asterisk: *text* for emphasis (italics)

two asterisks: **text** for strong emphasis (boldface), and

backquotes: ``text`` for code samples.

Lists and Quote-like bloks

* This is a bulleted list.
* It has two items, the second
  item uses two lines.

1. This is a numbered list.
2. It has two items too.


#. This is a numbered list.
#. It has two items too.


Nested lists are possible, but be aware that they must be separated from the parent list items by blank lines:

* this is
* a list

  * with a nested list
  * and some subitems

* and here the parent list continues

| These lines are
| broken exactly like in
| the source file.


Literal blocks

This is a normal text paragraph. The next paragraph is a code sample::


   It is not processed in any way, except
   that the indentation is removed.

   It can span multiple lines.


This is a normal text paragraph again.


+------------------------+------------+----------+----------+
| Header row, column 1   | Header 2   | Header 3 | Header 4 |
| (header rows optional) |            |          |          |
+========================+============+==========+==========+
| body row 1, column 1   | column 2   | column 3 | column 4 |
+------------------------+------------+----------+----------+
| body row 2             | ...        | ...      |          |
+------------------------+------------+----------+----------+

This is a paragraph that contains `a link`_.

.. _a link: https://domain.invalid/

=================
This is a heading
=================

.. This is a comment.


..
   This whole indented block
   is a comment.

   Still in the comment.


- 这里是列表的第一个列表项

- 这是第二个列表项

- 这是第三个列表项

  - 这是缩进的第一个列表项
    注意, 这里的缩进要和当前列表项的缩进同步

- 第一级的第四个列表项

- 列表项之间要用个空格来分割


下面是我们的测试代码:

::

    for i in [1,2,3,4,5]:
        print i
    # 代码块测试

很简单的代码块测试.

.. Strong Emphasis

This is **Strong Text**. HTML tag is strong.粗体

.. Italic, Emphasis

This is *Emphasis* Text.这个HTML使用em, 斜体

.. Interpreted Text

This is `Interpreted Text`. 注意, 这个HTML一般用<cite>表示

.. Inline Literals

This is ``Inline Literals``. HTML tag is <tt>. 等宽字体.

来点补充, 如果你需要在文档中插入超链接, 那么你可以像下面这样:

我这里是一个 链接_.

.. _链接: http://blog.inkypy.com


=========
一级标题
=========


二级标题
=========


一级标题
^^^^^^^^

二级标题
---------


三级标题
>>>>>>>>>


四级标题
:::::::::



五级标题
'''''''''


六级标题
""""""""
 
段落是被空行分割的文字片段，左侧必须对齐（没有空格，或者有相同多的空格）。

缩进的段落被视为引文。


选项列表(Option Lists)#
选项列表是一个类似两列的表格，左边是参数，右边是描述信息。当参数选项过长时，参数选项和描述信息各占一行。

选项与参数之间有一个空格，参数选项与描述信息之间至少有两个空格。


-a            command-line option "a"
-b file       options can have arguments
              and long descriptions
--long        options can be long also
--input=file  long options can also have
              arguments
/V            DOS/VMS-style options too


文字块(Literal Blocks)#
文字块就是一段文字信息，在需要插入文本块的段落后面加上 ::，接着一个空行，然后就是文字块了。

文字块不能定顶头写，要有缩进，结束标志是，新的一段文本贴开头，即没有缩进。

下面是文字块内容：
::
 
   这是一段文字块
   同样也是文字块
   还是文字块
 
这是新的一段。


下面是引用的内容：
 
    “真的猛士，敢于直面惨淡的人生，敢于正视淋漓的鲜血。”
    --- 鲁迅

    “人生的意志和劳动将创造奇迹般的奇迹。”
    — 涅克拉索


文档测试块是交互式的Python会话，以 >>> 开始，一个空行结束。

>>> print "This is a doctest block."
This is a doctest block.

隐式超链接(Implicit Hyperlink)#
小节标题、脚注和引用参考会自动生成超链接地址，使用小节标题、脚注或引用参考名称作为超链接名称就可以生成隐式链接。

第一节 介绍
===========
 
其他内容...
 
隐式链接到 `第一节 介绍`_，即可生成超链接。



替换引用(Substitution Reference)#
替换引用就是用定义的指令替换对应的文字或图片，和内置指令(inline directives)类似。

这是 |logo| github的Logo，我的github用户名是:|name|。
 
.. |logo| image:: https://help.github.com/assets/images/site/favicon.ico
.. |name| replace:: SeayXu


.. figure:: https://longzeping.github.io/img/apple-touch-icon.png
    :width: 32

    图：GitHub logo

GitHub Logo: |logo|

带链接的图片：
  |imglink|_

下图向右浮动：
   .. image:: https://longzeping.github.io/img/apple-touch-icon.png
      :align: right

.. |logo| image:: https://longzeping.github.io/img/apple-touch-icon.png
.. |imglink| image:: https://longzeping.github.io/img/apple-touch-icon.png
.. _imglink: https://github.com/


`Why ROS2? <http://design.ros2.org/articles/why_ros2.html>`_
--------------------------------------------------------------

* rep_0012_

.. _rep_0012: https://ros.org/reps/rep-0012.html
First-Level Title
=================

Second-Level Title
------------------

Third-Level Title
'''''''''''''''''

If there are more than three levels of sections in your REP, you may insert overline/underline-adorned titles for the first and second levels as follows:

============================
First-Level Title (optional)
============================

-----------------------------
Second-Level Title (optional)
-----------------------------

Third-Level Title
=================

Fourth-Level Title
------------------

Fifth-Level Title
'''''''''''''''''
You shouldn't have more than five levels of sections in your REP. If you do, you should consider rewriting it.

Simple tables are easy and compact:

=====  =====  =======
  A      B    A and B
=====  =====  =======
False  False  False
True   False  False
False  True   False
True   True   True
=====  =====  =======

There must be at least two columns in a table (to differentiate from section titles). Column spans use underlines of hyphens ("Inputs" spans the first two columns):

=====  =====  ======
   Inputs     Output
------------  ------
  A      B    A or B
=====  =====  ======
False  False  False
True   False  True
False  True   True
True   True   True
=====  =====  ======


Text in a first-column cell starts a new row. No text in the first column indicates a continuation line; the rest of the cells may consist of multiple lines. For example:

=====  =========================
col 1  col 2
=====  =========================
1      Second column of row 1.
2      Second column of row 2.
       Second line of paragraph.
3      - Second column of row 3.

       - Second item in bullet
         list (row 3, column 2).
=====  =========================

Hyperlinks

In this paragraph, we refer to the `ROS web site`_.

.. _ROS web site: https://ros.org/


The reference text and the target text must match (although the match is case-insensitive and ignores differences in whitespace). Note that the underscore trails the reference text but precedes the target text. If you think of the underscore as a right-pointing arrow, it points away from the reference and toward the target.


The same mechanism can be used for internal references. Every unique section title implicitly defines an internal hyperlink target. We can make a link to the Abstract section like this:

Here is a hyperlink reference to the `Abstract`_ section.  The
backquotes are optional since the reference text is a single word;
we can also just write: Abstract_.


Images
If your REP contains a diagram, you may include it in the processed output using the "image" directive:

.. image:: diagram.png

Any browser-friendly graphics format is possible: .png, .jpeg, .gif, .tiff, etc.
