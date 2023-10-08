
## free(): double free detected in tcache 2

- Dr.Memory
```
$ g++ -g -gdwarf-4 -fno-inline -fno-omit-frame-pointer leak_example.cpp -o leak_example

$ ./leak_example 
Destructed 2
size=3
Destructed 1446418487
free(): double free detected in tcache 2
Aborted (core dumped)

$ /usr/local/DrMemory-Linux-2.6.0/bin/drmemory -show_reachable -- leak_example
...
~~Dr.M~~ ERRORS FOUND:
~~Dr.M~~       1 unique,     1 total unaddressable access(es)
~~Dr.M~~       0 unique,     0 total uninitialized access(es)
~~Dr.M~~       1 unique,     1 total invalid heap argument(s)
~~Dr.M~~       0 unique,     0 total warning(s)
~~Dr.M~~       1 unique,     1 total,      8 byte(s) of leak(s)
~~Dr.M~~       0 unique,     0 total,      0 byte(s) of possible leak(s)
~~Dr.M~~       2 unique,     2 total,  73728 byte(s) of still-reachable allocation(s)
```

- valgrind
```
$   valgrind --leak-check=yes ./leak_example 
...
==27965== HEAP SUMMARY:
==27965==     in use at exit: 8 bytes in 1 blocks
==27965==   total heap usage: 4 allocs, 4 frees, 73,748 bytes allocated
==27965== 
==27965== 8 bytes in 1 blocks are definitely lost in loss record 1 of 1
==27965==    at 0x484A2F3: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==27965==    by 0x109401: my_array::my_array(int, int) (leak_example.cpp:11)
==27965==    by 0x10929A: main (leak_example.cpp:28)
==27965== 
==27965== LEAK SUMMARY:
==27965==    definitely lost: 8 bytes in 1 blocks
==27965==    indirectly lost: 0 bytes in 0 blocks
==27965==      possibly lost: 0 bytes in 0 blocks
==27965==    still reachable: 0 bytes in 0 blocks
==27965==         suppressed: 0 bytes in 0 blocks
```
> ==27965==   total heap usage: 4 allocs, 4 frees, 73,748 bytes allocated
所以，这里是多 free 了一次。
