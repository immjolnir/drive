#include <stdio.h>
#include <stdlib.h>

typedef struct foo {
    int flag1;
    int flag2;
    int size;
    int** buf;
} foo;

void print_buf(struct foo* s) {
    printf("buf[0][0]: %d\n", s->buf[0][0]);

    /* fix
    for (int i = 0; i < 20; i++) {
        free(s->buf[i]);
    }
    */
    free(s->buf);
}

void setup_foo(struct foo* s) {
    s->flag2 = 2;
    /* fix
    s->buf = malloc(20 * sizeof(void*));
    */
    s->buf = malloc(20 * sizeof(int));
    for (int i = 0; i < 20; i++) s->buf[i] = malloc(20 * sizeof(int));
}

int main(void) {
    printf("sizeof(int)=%lu, sizeof(void*)=%lu\n", sizeof(int), sizeof(void*));

    struct foo s;

    setup_foo(&s);
    print_buf(&s);

    if (s.flag1 || s.flag2) printf("hey\n");

    return 0;
}
