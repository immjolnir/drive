#include <stdlib.h>

int main() {
    char* p = malloc(16);
    char* p2 = malloc(16);
    p[24] = 1;  // buffer overrun, p has only 16 bytes
    free(p2);   // free(): invalid pointer
    free(p);
    return 0;
}