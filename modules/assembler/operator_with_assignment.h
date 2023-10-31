// Post them on https://godbolt.org/

int PreIncrementOperator() {
    // ++i
    int i = 1;
    int j = ++i;
    return j;
}

int PostIncrementOperator() {
    // i++
    int i = 1;
    int j = i++;
    return j;
}

int PreDecrementOperator() {
    // --i
    int i = 1;
    int j = --i;
    return j;
}

int PostDecrementOperator() {
    // i--
    int i = 1;
    int j = i--;
    return j;
}

/*
PreIncrementOperator():              # @PreIncrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)    # -4(%rbp) 是 变量i的地址。这是把立即数赋值给i
        movl    -4(%rbp), %eax  # 把 i 的值 copy 到 eax 中
        addl    $1, %eax        # eax加1，然后写入 eax中，完成加1
        movl    %eax, -4(%rbp)  # 把加1后的结果，写回 变量i
        movl    %eax, -8(%rbp)  # 把加1后的结果，写到 变量j; -8(%rbp)是变量j; 此时j是2.
        movl    -8(%rbp), %eax  # 把变量j copy 到 %eax寄存器中。 eax在函数调用中，经常用来存储函数的返回值.
        popq    %rbp
        retq
PostIncrementOperator():             # @PostIncrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)    # i=1
        movl    -4(%rbp), %eax  # copy 1 to eax
        movl    %eax, %ecx      # copy eax to ecx
        addl    $1, %ecx        # increase ecx by 1.(ecx = ecx + 1)
        movl    %ecx, -4(%rbp)  # 把加1后的结果赋给i
        movl    %eax, -8(%rbp)  # 把 eax 的值赋给j，这里的 eax 保存的是i之前的数；本例中j是1.
        movl    -8(%rbp), %eax  # 把 j 放到返回值寄存器 eax 里
        popq    %rbp
        retq
PreDecrementOperator():              # @PreDecrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        addl    $-1, %eax
        movl    %eax, -4(%rbp)  # eax是完成-1后的结果
        movl    %eax, -8(%rbp)  # j与i，一样，都是减1后的值，即0.
        movl    -8(%rbp), %eax
        popq    %rbp
        retq
PostDecrementOperator():             # @PostDecrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        movl    %eax, %ecx
        addl    $-1, %ecx
        movl    %ecx, -4(%rbp)  # ecx是完成-1后的结果，即i=0
        movl    %eax, -8(%rbp)  # eax是执行-1前的值，即j=1.
        movl    -8(%rbp), %eax
        popq    %rbp
        retq
*/