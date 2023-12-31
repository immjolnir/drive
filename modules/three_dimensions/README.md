
# 如何描述刚体在三维空间中的运动？
- 线性代数库 Eigen
  - 提供了矩阵运算
  - Geometry 模块还提供了四元数等描述刚体运动的结构
  - Eigen 的优化非常完善

- 内积
  - 内积的结果与坐标系的选取无关

- 外积的描述
  - 外积的结果是一个向量，它的方向垂直于这个向量，大小为 |a||b|sin(a,b), 是两个向量张成的四边形的有向面积.
  - 把 a 写成反对称矩阵（skew-symmetric matrix）， 于是就是把外积写成矩阵与向量的乘法 a^b, 把它变成了线性运算。
```
        || e1 e2 e3 ||   | a2*b3 - a3*b2 |   | 0 -a3 a2 |
a * b = || a1 a2 a3 || = | a3*b1 - a1*b3 | = | a3 0 -a1 |* b = a ^ b
        || b1 b2 b3 ||   | a1*b2 - a3*b1 |   |-a2 a1 0  |
```
  - 这也意味着任何向量都对应着唯一的一个反对称矩阵
```
     | 0  -a3  a2 |
a^ = | a3  0  -a1 |
     |-a2  a1  0  |
```
 
- 欧氏变换(Euclidean Transform): 由旋转和平移组成
  - 首先考虑旋转（该向量并没有随着坐标系的旋转而发生运动), 方向余弦( Direction Cosine Matrix)
  - 其次考虑平移
```
a' = R*a + t
```
  - rotation: R12 是指 把坐标系2的向量变换到坐标系1.  由于向量乘在这个矩阵的右边，它的下标是从右读到左。
  - tranform: 
    - t12 它实际上是坐标系1 原点 指向坐标系2原点的向量， "在坐标系1下取的坐标"，所以这个 “从1到2的向量”。
    - t21 即 从 2 指向 1的向量，在坐标系2下的坐标，却并不等于 -t12, 而是和两个系的旋转还有关系
    - "我的坐标系"实际上指从世界坐标系指向自己坐标系原点的向量，在世界坐标系下取的坐标, 记作 t_wc. 同理，它也不等于 - t_cw.

- 齐次坐标系: 
  - 在三维向量的末尾加1, 将其变换成四维向量，称为齐次坐标。
  - 对于四维向量，我们可以把旋转和平移写到一个矩阵里，矩阵T 称为 变换矩阵 (Transform Matrix)
```
| a' |   | R  t |   | a |       | a |
|    | = |      | * |   | = T * |   |
| 1  |   | 0  1 |   | 1 |       | 1 |
```
  - 这个T 矩阵比较特殊，左上角为旋转矩阵，右侧为平移向量，左下角为0向量，右下角为1. 称为 特殊欧氏群(Special Euclidean Group).

