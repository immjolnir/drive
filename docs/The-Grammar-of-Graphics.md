# The Grammar of Graphics

- https://zhuanlan.zhihu.com/p/86416111
  - 基本思路是，把图表的主体看成是数据和几何图形的视觉特征绑定的结果；同时，把图表看成一些简单的相互正交的特征组合而成的结果。比如说，数据点视觉特征（visual cue）和坐标系（coordinates）这两个特征的最常见的10（7+3）个可能的取值，就可以组合成21（7x3）种不同的图形类别。
  - 目前为止，Leland Wilkinson首创的这一套图形语法系统已经有了不少的实现，如ggplot, 上文提到的Vega，Python世界的Bokeh，Tableau的VizQL，IBM曾经主推的vizJSON，还有不久前阿里出的G2，等等。不过图形语法严格来说只是一种思想而不是一个真正意义上实现标准，所以很不幸这些实现是无法互相兼容的。有些实现为了兼顾自己的实际使用场景，也并没有完全遵守图形语法系统的原始定义。

  - 形式化系统的实现的源码: 
    - [ggplot2](https://github.com/tidyverse/ggplot2)

    - [vega](https://github.com/vega/vega)
      - https://vega.github.io/editor/
      - [altair](https://github.com/altair-viz/altair): Declarative statistical visualization library for Python

    - [Lyra](https://github.com/vega/lyra)
      - [living-papers](https://github.com/uwdata/living-papers.git)
      - [living-papers-template](https://github.com/uwdata/living-papers-template)

    - [bokeh](https://github.com/bokeh/bokeh): Interactive Data Visualization in the browser, from Python
      - https://bokeh.org/
      - https://github.com/MathOnco/Mistic

    - [VizQL](https://www.tableau.com/drive/what-is-vizql): VizQL is a visual query language for databases.


    - [G2](https://github.com/antvis/g2/): A concise and progressive visualization grammar.


# [Altair + Vega + Justcharts = Interactive Visualisations in Static Websites](https://blog.4dcu.be/programming/2021/05/03/Interactive-Visualizations.html) 
