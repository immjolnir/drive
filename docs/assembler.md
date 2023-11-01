# Assembly 

- https://zhuanlan.zhihu.com/p/469950256
- https://agner.org/optimize/

- Intel asm 与  AT&T asm 比较
  - https://blog.csdn.net/liutianshx2012/article/details/50974844?locationnum=8


- qword(quad word), dword(double word), word, tyte 分别对应8, 4, 2, 1个字节.


- eax: 是32位寄存器, eax在函数调用中，经常用来存储函数的返回值
  - `ax`是`eax`的低16位。所以可以说eax与ax不是独立的。
    - ah: `ax`的高8位
    - al: `ax`的低8位

例 `eax` 为12345678, 那么 `ax`为 5678, `ah`为56; `al`为78. 如果此时`ax`变为9876，那么`eax`就是12349876，对`ax`的赋值可以影响到`eax`。

- ptr

PTR 和 WORD 这两个一般是连用的 。代表字属性的有 `word ptr`； 代表字节属性的有 `byte ptr`，PTR（指针）缩写就是全称POINTER.

规定的字（保留字）在汇编里面是PTR，`MOV AX,bx`在汇编里面是把bx寄存器“里”赋予AX 两个都是WORD型。

`MOV AX WORD PTR [BX]`，“意思是把内存地址等于bx寄存器的值”的地方所存放的数据，赋予AX, 是BYTE 还是word ，需要用WORD明确出来。

指针（Pointer）是编程语言中的一个对象，利用地址，它的值直接指向（points to）存在电脑存储器中另一个地方的值。
在高级语言中，指针有效地取代了在低级语言，如汇编语言与机器码，直接使用通用暂存器的地方，但它可能只适用于合法地址之中。
指针参考了存储器中某个地址，通过被称为反参考指针的动作，可以取出在那个地址中存储的值。

```c++
    const char buffer[5] = {'a', 'b', 'c', 'd', 'e'};
    const char* ptr = buffer;
    ++ptr;
    char b = *(ptr++);
    /*
        mov     rax, qword ptr [rbp - 16]
        mov     rcx, rax
        add     rcx, 1
        mov     qword ptr [rbp - 16], rcx
        mov     al, byte ptr [rax]
        mov     byte ptr [rbp - 17], al
    */

    char c = *ptr;
    /*
        mov     rax, qword ptr [rbp - 16]
        mov     al, byte ptr [rax]
        mov     byte ptr [rbp - 18], al
    */
    ptr++;
    /*
        mov     rax, qword ptr [rbp - 16]
        add     rax, 1
        mov     qword ptr [rbp - 16], rax
    */
```

