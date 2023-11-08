# C++ Code Style


## Position of the input and output

The method signature of the C as below:
- strcpy
```c
       char *strcpy(char *dest, const char *src);

       char *strncpy(char *dest, const char *src, size_t n);
```
- memcpy
```c
       void *memcpy(void *dest, const void *src, size_t n);
```

A simple implementation of strncpy() might be:
```c
char * strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

    return dest;
}
```

Output parameters are positioned before inputs.