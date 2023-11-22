======
RST Document in Sublime Text
======

From https://github.com/seayxu/CheatSheet/blob/master/files/reStructuredText-Quick-Syntax.md?plain=1


章节头部由下线(也可有上线)和包含标点的标题 组合创建, 其中下线要至少等于标准文本的长度。
全加上上标或者是全不加上标，使用不同的 6 个符号的标题依次排列，则会依次生成的标题为H1-H6。

=========
一级标题
=========
二级标题
=========
一级章节
^^^^^^^^
二级章节
---------
三级章节
>>>>>>>>>
四级章节
:::::::::
五级章节
'''''''''
六级章节
""""""""


*重点(emphasis)通常显示为斜体*

**重点**

``行内文本(inline literal)通常显示为等宽文本，空格可以保留，但是换行不可以。``


列表

- 符号列表1
- 符号列表2

  + 二级符号列表1

  - 二级符号列表2

  * 二级符号列表3

* 符号列表3

+ 符号列表4

1. 枚举列表1
#. 枚举列表2
#. 枚举列表3

(I) 枚举列表1
(#) 枚举列表2
(#) 枚举列表3

A) 枚举列表1
#) 枚举列表2
#) 枚举列表3


定义1
  这是定义1的内容

定义2
  这是定义2的内容


## 字段列表(Field Lists)

:标题: reStructuredText语法说明

:作者:
 - Seay
 - Seay1
 - Seay2

:时间: 2016年06月21日

:概述: 这是一篇
 关于reStructuredText

 语法说明。


## 选项列表(Option Lists)

选项列表是一个类似两列的表格，左边是参数，右边是描述信息。当参数选项过长时，参数选项和描述信息各占一行。

选项与参数之间有一个空格，参数选项与描述信息之间至少有两个空格。

-a            command-line option "a"
-b file       options can have arguments
              and long descriptions
--long        options can be long also
--input=file  long options can also have
              arguments
/V            DOS/VMS-style options too

## 文字块(Literal Blocks)

文字块就是一段文字信息，在需要插入文本块的段落后面加上 **::**，接着一个空行，然后就是文字块了。

文字块不能定顶头写，要有缩进，结束标志是，新的一段文本贴开头，即没有缩进。

下面是文字块内容：
::

   这是一段文字块
   同样也是文字块
   还是文字块

这是新的一段。

## 行块(Line Blocks)

行块对于地址、诗句以及无装饰列表是非常有用的。行块是以 **|** 开头，每一个行块可以是多段文本。

**|** 前后各有一个空格。

下面是行块内容：
 | 这是一段行块内容
 | 这同样也是行块内容
   还是行块内容

这是新的一段。


## 块引用(Block Quotes)

块引用是通过缩进来实现的，引用块要在前面的段落基础上缩进。

通常引用结尾会加上出处(attribution)，出处的文字块开头是 **--**、**---** 、**—**，后面加上出处信息。

块引用可以使用空的注释 **..** 分隔上下的块引用。

注意在新的块和出处都要添加一个空行。

下面是引用的内容：

	“真的猛士，敢于直面惨淡的人生，敢于正视淋漓的鲜血。”

	--- 鲁迅

..

	“人生的意志和劳动将创造奇迹般的奇迹。”

	— 涅克拉索

## 文档测试块(Doctest Blocks)

文档测试块是交互式的Python会话，以 **>>>** 开始，一个空行结束。

>>> print "This is a doctest block."
This is a doctest block.

## 网格表(Grid Tables)

网格表中使用的符号有：**-**、**=**、**|**、**+**。

**-** 用来分隔行， **=** 用来分隔表头和表体行，**|** 用来分隔列，**+** 用来表示行和列相交的节点。

Grid table:

+------------+------------+-----------+
| Header 1   | Header 2   | Header 3  |
+============+============+===========+
| body row 1 | column 2   | column 3  |
+------------+------------+-----------+
| body row 2 | Cells may span columns.|
+------------+------------+-----------+
| body row 3 | Cells may  | - Cells   |
+------------+ span rows. | - contain |
| body row 4 |            | - blocks. |
+------------+------------+-----------+

## 简单表(Simple Tables)

简单表相对于网格表，少了 **|** 和 **+** 两个符号，只用 **-** 和 **=** 表示。
Simple table:

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

# 分隔符

分隔符就是一条水平的横线，是由 4 个 **-** 或者更多组成，需要添加换行。

----

# 超链接

介绍各类带有链接性质的超链接

自动超链接

reStructuredText会自动将网址生成超链接。


https://github.com/SeayXu/


## 外部超链接(External Hyperlink)

引用/参考(reference)，是简单的形式，只能是一个词语，引用的文字不能带有空格。

这篇文章来自我的Github,请参考 reference_。

.. _reference: https://github.com/SeayXu/

引用/参考(reference)，行内形式，引用的文字可以带有空格或者符号。

这篇文章来自我的Github,请参考 `SeayXu <https://github.com/SeayXu/>`_。


## 内部超链接|锚点(Internal Hyperlink)

更多信息参考 引用文档_

这里是其他内容

.. _引用文档:

这是引用部分的内容


匿名超链接(Anonymous hyperlink)

词组(短语)引用/参考(phrase reference)，引用的文字可以带有空格或者符号，需要使用反引号引起来。

这篇文章参考的是：`Quick reStructuredText`__。

.. __: http://docutils.sourceforge.net/docs/user/rst/quickref.html


## 间接超链接(Indirect Hyperlink)

间接超链接是基于匿名链接的基础上的，就是将匿名链接地址换成了外部引用名_。

SeayXu_ 是 `我的 GitHub 用户名`__。

.. _SeayXu: https://github.com/SeayXu/

__ SeayXu_


## 隐式超链接(Implicit Hyperlink)

小节标题、脚注和引用参考会自动生成超链接地址，使用小节标题、脚注或引用参考名称作为超链接名称就可以生成隐式链接。

第一节 介绍
===========

其他内容...

隐式链接到 `第一节 介绍`_，即可生成超链接。


## 替换引用(Substitution Reference)

替换引用就是用定义的指令替换对应的文字或图片，和内置指令(inline directives)类似。

这是 |logo| github的Logo，我的github用户名是:|name|。

.. |logo| image:: https://help.github.com/assets/images/site/favicon.ico
.. |name| replace:: SeayXu


## 脚注引用(Footnote Reference)

脚注引用，有这几个方式：有手工序号(标记序号123之类)、自动序号(填入#号会自动填充序号)、自动符号(填入*会自动生成符号)。

手工序号可以和#结合使用，会自动延续手工的序号。


**#** 表示的方法可以在后面加上一个名称，这个名称就会生成一个链接。

```
脚注引用一 [1]_
脚注引用二 [#]_
脚注引用三 [#链接]_
脚注引用四 [*]_
脚注引用五 [*]_
脚注引用六 [*]_

.. [1] 脚注内容一
.. [2] 脚注内容二
.. [#] 脚注内容三
.. [#链接] 脚注内容四 链接_
.. [*] 脚注内容五
.. [*] 脚注内容六
.. [*] 脚注内容七


# 注释(Comments)

注释以 **..** 开头，后面接注释内容即可，可以是多行内容，多行时每行开头要加一个空格。

..
 我是注释内容
 你们看不到我