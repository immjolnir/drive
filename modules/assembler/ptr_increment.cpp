
int main() {
    char buffer[5] = {'a', 'b', 'c', 'd', 'e'};
    const char* ptr = buffer;

    ++ptr;
    /*
        movq    -16(%rbp), %rax
        addq    $1, %rax
        movq    %rax, -16(%rbp)
    */

    char b = *(ptr++);  // get 'b'.
    /*
        movq    -16(%rbp), %rax  # 把 ptr地址保存到 rax 中
        movq    %rax, %rcx
        addq    $1, %rcx
        movq    %rcx, -16(%rbp)
        movb    (%rax), %al     # 按照rax地址从内存中取出值，写入到 al 中，这就是为什么变量b的值是 ptr加1前的值。
        movb    %al, -17(%rbp)
    */

    char c = *ptr;  // get 'c'
    /*
        movq    -16(%rbp), %rax
        movb    (%rax), %al
        movb    %al, -18(%rbp)
    */
    ptr++;
}

// AT&T ASM

/*
* Direction of Operands.
The direction of the operands in Intel syntax is opposite from that of AT&T syntax.
In Intel syntax the first operand is the destination, and the second operand is the source whereas
in AT&T syntax the first operand is the source and the second operand is the destination.
The advantage of AT&T syntax in this situation is obvious.

We read from left to right, we write from left to right, so this way is only natural.

main:                                   # @main
        pushq   %rbp
        movq    %rsp, %rbp
        movl    .L__const.main.buffer(%rip), %eax
        movl    %eax, -5(%rbp)
        movb    .L__const.main.buffer+4(%rip), %al
        movb    %al, -1(%rbp)
        leaq    -5(%rbp), %rax
        movq    %rax, -16(%rbp)
        movq    -16(%rbp), %rax
        addq    $1, %rax
        movq    %rax, -16(%rbp)
        movq    -16(%rbp), %rax
        movq    %rax, %rcx
        addq    $1, %rcx
        movq    %rcx, -16(%rbp)
        movb    (%rax), %al
        movb    %al, -17(%rbp)
        movq    -16(%rbp), %rax
        movb    (%rax), %al
        movb    %al, -18(%rbp)
        movq    -16(%rbp), %rax
        addq    $1, %rax
        movq    %rax, -16(%rbp)
        xorl    %eax, %eax
        popq    %rbp
        retq
.L__const.main.buffer:
        .ascii  "abcde"
*/