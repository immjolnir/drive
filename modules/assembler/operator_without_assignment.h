// Post them on https://godbolt.org/

void PreIncrementOperator() {
    // ++i
    int i = 1;
    ++i;
}

void PostIncrementOperator() {
    // i++
    int i = 1;
    i++;
}

void PreDecrementOperator() {
    // --i
    int i = 1;
    --i;
}

void PostDecrementOperator() {
    // i--
    int i = 1;
    i--;
}

/* Get the result from the Compiler Explorer

PreIncrementOperator():              # @PreIncrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        addl    $1, %eax
        movl    %eax, -4(%rbp)
        popq    %rbp
        retq
PostIncrementOperator():             # @PostIncrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        addl    $1, %eax
        movl    %eax, -4(%rbp)
        popq    %rbp
        retq
PreDecrementOperator():              # @PreDecrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        addl    $-1, %eax
        movl    %eax, -4(%rbp)
        popq    %rbp
        retq
PostDecrementOperator():             # @PostDecrementOperator()
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        addl    $-1, %eax
        movl    %eax, -4(%rbp)
        popq    %rbp
        retq
*/

// In the above cases, we can know:
// ++i and i++ are generating the same AT&T ASM
// --i and i-- are generating the same AT&T ASM
