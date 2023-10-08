# Example: free(): double free detected in tcache 2

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

- sanitize
```
$ g++ -g -O0 -fsanitize=address leak_example.cpp -o leak_example

$ ./leak_example 
Destructed 2
size=3
=================================================================
==34145==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000030 at pc 0x564780c3d9aa bp 0x7ffd5d6fca10 sp 0x7ffd5d6fca00
READ of size 4 at 0x602000000030 thread T0
    #0 0x564780c3d9a9 in my_array::~my_array() /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:18
    #1 0x564780c3d669 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:37
    #2 0x7faad468ad8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7faad468ae3f in __libc_start_main_impl ../csu/libc-start.c:392
    #4 0x564780c3d364 in _start (/home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example+0x2364)

0x602000000030 is located 0 bytes inside of 12-byte region [0x602000000030,0x60200000003c)
freed by thread T0 here:
    #0 0x7faad4c70e37 in operator delete[](void*) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:163
    #1 0x564780c3da0c in my_array::~my_array() /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:20
    #2 0x564780c3d5d5 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:33
    #3 0x7faad468ad8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

previously allocated by thread T0 here:
    #0 0x7faad4c70337 in operator new[](unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:102
    #1 0x564780c3d85c in my_array::my_array(int, int) /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:11
    #2 0x564780c3d4f3 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:31
    #3 0x7faad468ad8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

SUMMARY: AddressSanitizer: heap-use-after-free /home/zhishan/iwork/drive/modules/basis/memory_debugger/leak_example.cpp:18 in my_array::~my_array()
Shadow bytes around the buggy address:
  0x0c047fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c047fff8000: fa fa 00 fa fa fa[fd]fd fa fa fa fa fa fa fa fa
  0x0c047fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==34145==ABORTING
```

- Detecting The Use Of Uninitialized Variables

Valgrind is even smart enough to know that if a variable is assigned the value of an uninitialized variable, that that variable is still in an "uninitialized" state.

```
$ g++ -g -O0 uninitialized_variable.cpp -o unapp

$ valgrind --tool=memcheck --leak-check=yes ./unapp 
==30876== Memcheck, a memory error detector
==30876== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==30876== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==30876== Command: ./unapp
==30876== 
==30876== Conditional jump or move depends on uninitialised value(s)
==30876==    at 0x1091B9: main (uninitialized_variable.cpp:6)
==30876== 
X is zero
```