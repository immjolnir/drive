# C++ IO Format

使用控制符控制输出格式

- dec
- hex
- oct

- setbase(n): 设置整数的基数为n(n只能是16，10，8之一)

- setfill(c): 设置填充字符c，c可以是字符常量或字符变量

- setprecision(n): 设置实数的精度为n位。在以一般十进制小数形式输出时，n代表有效数字。在以fixed(固定小数位数)形式和scientific(指数)形式输出时，n为小数位数。
  - 高精度向低精度转换超出位数会被舍弃

- setw(n): 设置字段宽度为n位。

- setiosflags:
    - setiosflags(ios::fixed): 设置浮点数以固定的小数位数显示。

    - setiosflags(ios::scientific): 设置浮点数以科学计数法(即指数形式)显示。

    - setiosflags(ios::left): 输出数据左对齐。

    - setiosflags(ios::right): 输出数据右对齐。

    - setiosflags(ios::shipws): 忽略前导的空格。

    - setiosflags(ios::uppercase): 在以科学计数法输出E和十六进制输出字母X时，以大写表示。

    - setiosflags(ios::showpos): 输出正数时，给出“+”号。

- resetiosflags: 终止已设置的输出格式状态，在括号中应指定内容。

## Examples:
- 以小数形式，保留三位小数输出：
```
cout<<setprecision(3)<<setiosflags(ios::fixed)<<3.1415926<<endl;

cout << std::fixed << std::setprecision(3) << 3.1415926 << endl;
```

```
cout<<setw(8)<<"C++"<<setw(6)<<"101"<<endl;
```

- showpoint （与setprecision（n）一起使用）, showpoint 强制小数后面以0显示。
````
cout<<setprecision(6);
cout<<showpoint<<1.23<<endl; ----显示 1.23000
cout<<showpoint<<123.0<<endl; ----显示 123.000
```

