# Sanitize

- https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind#tldr

## Sample case: A buffer overrun

- overrun.c
AddressSanitizer needs a recompilation of the code

```
gcc -o overrun overrun.c -Wall -g -fsanitize=address

==35214==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000028 at pc 0x55ac75f0f22d bp 0x7ffe67dd44a0 sp 0x7ffe67dd4490
WRITE of size 1 at 0x602000000028 thread T0
    #0 0x55ac75f0f22c in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/overrun.c:6
    #1 0x7f9599f66d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #2 0x7f9599f66e3f in __libc_start_main_impl ../csu/libc-start.c:392
    #3 0x55ac75f0f104 in _start (/home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/overrun+0x1104)

0x602000000028 is located 8 bytes to the left of 16-byte region [0x602000000030,0x602000000040)
allocated by thread T0 here:
    #0 0x7f959a219867 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
    #1 0x55ac75f0f1ec in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/overrun.c:5
    #2 0x7f9599f66d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/overrun.c:6 in main

```

## ASAN: Out-of-bounds access in stack variables
Because Valgrind does not require recompiling the program, it cannot detect some invalid memory accesses. One such bug is accessing memory out of the range of automatic (local) variables and global variables.

Because Valgrind gets involved only at runtime, it does fence and track memory from malloc allocations. Unfortunately, allocation of variables on the stack is inherent to the already compiled program without calling any external functions such as malloc, so Valgrind cannot find out whether the access to stack memory is valid or is just an accidental runaway from a different stack object. Sanitizers, on the other hand, instruments all the code at compile time, when the compiler still knows which specific variable on the stack the program is trying to access and what the variable's correct stack boundaries are:

- stack.c

```
$ gcc -o stack stack.c -Wall -g -fsanitize=address

$ ./stack 
=================================================================
==35504==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffc2629dc84 at pc 0x563581bec2c3 bp 0x7ffc2629daa0 sp 0x7ffc2629da90
READ of size 4 at 0x7ffc2629dc84 thread T0
    #0 0x563581bec2c2 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/stack.c:3
    #1 0x7fe4034b0d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #2 0x7fe4034b0e3f in __libc_start_main_impl ../csu/libc-start.c:392
    #3 0x563581bec104 in _start (/home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/stack+0x1104)

Address 0x7ffc2629dc84 is located in stack of thread T0 at offset 452 in frame
    #0 0x563581bec1d8 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/stack.c:1

$ gcc -o stack stack.c -Wall -g 
stack.c: In function ‘main’:
stack.c:3:13: warning: ‘a’ is used uninitialized [-Wuninitialized]
    3 |     return a[argc + 100];
      |            ~^~~~~~~~~~~~
stack.c:2:9: note: ‘a’ declared here
    2 |     int a[100];
      |         ^
$ valgrind ./stack 
==35569== Memcheck, a memory error detector
==35569== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==35569== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==35569== Command: ./stack
==35569== 
==35569== Syscall param exit_group(status) contains uninitialised byte(s)
==35569==    at 0x4962CA1: _Exit (_exit.c:30)
==35569==    by 0x48BD551: __run_exit_handlers (exit.c:136)
==35569==    by 0x48BD60F: exit (exit.c:143)
==35569==    by 0x48A1D96: (below main) (libc_start_call_main.h:74)
==35569== 
==35569== 
==35569== HEAP SUMMARY:
==35569==     in use at exit: 0 bytes in 0 blocks
==35569==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==35569== 
==35569== All heap blocks were freed -- no leaks are possible
==35569== 
==35569== Use --track-origins=yes to see where uninitialised values come from
==35569== For lists of detected and suppressed errors, rerun with: -s
==35569== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

## ASAN: Out-of-bounds access in global variables
As with variables on the stack, Valgrind cannot detect a global variable overrun because it does not recompile the program.

I described earlier why Valgrind cannot catch such errors. Here are the outputs from AddressSanitizer and Valgrind:

- global.c

```
$ clang -o global global.c -Wall -g -fsanitize=address

$ ./global 
=================================================================
==35691==ERROR: AddressSanitizer: global-buffer-overflow on address 0x55b3b7e2a674 at pc 0x55b3b74a0f06 bp 0x7fff3b9d99f0 sp 0x7fff3b9d99e8
READ of size 4 at 0x55b3b7e2a674 thread T0
    #0 0x55b3b74a0f05 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/global.c:3:42
    #1 0x7f5507a5fd8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #2 0x7f5507a5fe3f in __libc_start_main csu/../csu/libc-start.c:392:3
    #3 0x55b3b73e3304 in _start (/home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/global+0x1e304) (BuildId: e0cd00ec6d1b5cded7662426024ef33569adc8d5)

0x55b3b7e2a674 is located 4 bytes to the right of global variable 'a' defined in 'global.c:1:5' (0x55b3b7e2a4e0) of size 400

$ clang -o global global.c -Wall -g
$ valgrind ./global 
==35758== Memcheck, a memory error detector
==35758== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==35758== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==35758== Command: ./global
==35758== 
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x1b
get_Form_szB: unhandled 27 (DW_FORM_addrx)
--35758-- WARNING: Serious error when reading debug info
--35758-- When reading debug info from /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/global:
--35758-- get_Form_contents: unhandled DW_FORM
==35758== 
==35758== HEAP SUMMARY:
==35758==     in use at exit: 0 bytes in 0 blocks
==35758==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==35758== 
==35758== All heap blocks were freed -- no leaks are possible
==35758== 
==35758== For lists of detected and suppressed errors, rerun with: -s
==35758== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## MSAN: Uninitialized memory reads


AddressSanitizer does not detect reads of uninitialized memory. MemorySanitizer was developed for that. It needs a separate compilation and run. (See the MemorySanitizer documentation.) Why AddressSanitizer was not designed to include the functionality of MemorySanitizer is unclear to me, and I'm not the only one.

A run of MemorySanitizer follows:

- unint.c

```
$ clang -o uninit uninit.c -Wall -g -fsanitize=address -fsanitize=memory
clang: error: invalid argument '-fsanitize=address' not allowed with '-fsanitize=memory'
$ clang -o uninit uninit.c -Wall -g -fsanitize=memory

$ ./uninit 
==35909==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x562c51c51466 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/uninit.c:3:9
    #1 0x7f163f544d8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #2 0x7f163f544e3f in __libc_start_main csu/../csu/libc-start.c:392:3
    #3 0x562c51bcb2a4 in _start (/home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/uninit+0x1e2a4) (BuildId: 3c84f71b2527046cacfd1f461869fcd83d29be63)

SUMMARY: MemorySanitizer: use-of-uninitialized-value /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/uninit.c:3:9 in main
Exiting

```

## ASAN: Stack use after return

AddressSanitizer requires enabling ASAN_OPTIONS=detect_stack_use_after_return=1 at runtime, because this feature imposes extra runtime overhead. (See the AddressSanitizer Use After Return documentation.) The following is a sample program that runs without error by itself or with Valgrind, but reveals the error when run with AddressSanitizer:

- uar.cpp

```
$ clang++ -o uar uar.cpp -Wall -g -fsanitize=address
$ ./uar
(nothing found by default)
$ ASAN_OPTIONS=detect_stack_use_after_return=1 ./uar
=================================================================
==164341==ERROR: AddressSanitizer: stack-use-after-return on address 0x7fb71a561020 at pc 0x0000004f78e1 bp 0x7ffc299184c0 sp 0x7ffc299184b8
READ of size 4 at 0x7fb71a561020 thread T0
    #0 0x4f78e0 in g(int*) /home/lace/src/uar.cpp:7:10
    #1 0x4f790b in main /home/lace/src/uar.cpp:10:10
    #2 0x7fb71dbde1e1 in __libc_start_main (/lib64/libc.so.6+0x281e1)
    #3 0x41c41d in _start (/home/lace/src/uar+0x41c41d)

Address 0x7fb71a561020 is located in stack of thread T0 at offset 32 in frame
    #0 0x4f771f in f() /home/lace/src/uar.cpp:1

  This frame has 1 object(s):
    [32, 36) 'i' (line 2) <== Memory access at offset 32 is inside this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-use-after-return /home/lace/src/uar.cpp:7:10 in g(int*)
...
$ clang++ -o uar uar.cpp -Wall -g
$ valgrind ./uar
...
(nothing found by Valgrind)
```

## UBSAN: Undefined behavior

UndefinedBehaviorSanitizer protects code from computations that are forbidden by the language standard. (See the UndefinedBehaviorSanitizer documentation.) For performance reasons, some undefined computations might not be trapped at runtime, but nobody guarantees anything about the program if they are included. Most commonly, such numeric expressions just compute an unexpected result. UndefinedBehaviorSanitizer can detect and report such operations.

UndefinedBehaviorSanitizer can be used together with the most common Sanitizer, the AddressSanitizer:

- undefined.cpp

```
$ clang++ -o undefined undefined.cpp -Wall -g -fsanitize=undefined
zhishan@006IT00075:~/iwork/drive/modules/basis/memory_debugger/sanitize [GIT:main]
$ ./undefined 
undefined.cpp:1:53: runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'

$ clang++ -o undefined undefined.cpp -Wall -g 
zhishan@006IT00075:~/iwork/drive/modules/basis/memory_debugger/sanitize [GIT:main]
$ valgrind ./undefined 
==36279== Memcheck, a memory error detector
==36279== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==36279== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==36279== Command: ./undefined
==36279== 
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x1b
get_Form_szB: unhandled 27 (DW_FORM_addrx)
--36279-- WARNING: Serious error when reading debug info
--36279-- When reading debug info from /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/undefined:
--36279-- get_Form_contents: unhandled DW_FORM
==36279== 
==36279== HEAP SUMMARY:
==36279==     in use at exit: 0 bytes in 0 blocks
==36279==   total heap usage: 1 allocs, 1 frees, 72,704 bytes allocated
==36279== 
==36279== All heap blocks were freed -- no leaks are possible
==36279== 
==36279== For lists of detected and suppressed errors, rerun with: -s
==36279== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

My personal preference is to abort the program at the first such occurrence, because otherwise it is difficult to find the bug. I therefore use -fno-sanitize-recover=all. I also prefer to extend the UndefinedBehaviorSanitizer coverage a bit by including: -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow.

## LSAN: Memory leaks

LeakSanitizer reports allocated memory that has not been freed before the program finished. (See the LeakSanitizer documentation.) Such behavior is not necessarily a bug. But freeing all allocated memory makes it easier, for example, to catch real, unexpected memory leaks:

- leak.cpp
```
$ clang++ -o leak leak.cpp -Wall -g -fsanitize=address
$ ./leak 

=================================================================
==36372==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 10 byte(s) in 1 object(s) allocated from:
    #0 0x561a48a5b14e in malloc (/home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/leak+0xa114e) (BuildId: b0bca94630dd48c9aa40b04fa79364403500ac99)
    #1 0x561a48a98528 in main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/leak.cpp:4:15
    #2 0x7f7b51befd8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16

SUMMARY: AddressSanitizer: 10 byte(s) leaked in 1 allocation(s).

$ clang++ -o leak leak.cpp -Wall -g 

$ valgrind --leak-check=full ./leak 
==36406== Memcheck, a memory error detector
==36406== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==36406== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==36406== Command: ./leak
==36406== 
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x1b
get_Form_szB: unhandled 27 (DW_FORM_addrx)
--36406-- WARNING: Serious error when reading debug info
--36406-- When reading debug info from /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/leak:
--36406-- get_Form_contents: unhandled DW_FORM
==36406== 
==36406== HEAP SUMMARY:
==36406==     in use at exit: 10 bytes in 1 blocks
==36406==   total heap usage: 2 allocs, 1 frees, 72,714 bytes allocated
==36406== 
==36406== 10 bytes in 1 blocks are definitely lost in loss record 1 of 1
==36406==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==36406==    by 0x109158: main (leak.cpp:4)
==36406== 
==36406== LEAK SUMMARY:
==36406==    definitely lost: 10 bytes in 1 blocks
==36406==    indirectly lost: 0 bytes in 0 blocks
==36406==      possibly lost: 0 bytes in 0 blocks
==36406==    still reachable: 0 bytes in 0 blocks
==36406==         suppressed: 0 bytes in 0 blocks
==36406== 
==36406== For lists of detected and suppressed errors, rerun with: -s
==36406== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

## LSAN: Memory leaks with specific libraries (glib2)

Some frameworks have custom memory allocators that prevent LeakSanitizer from doing its job. The following example uses such a framework, glib2 (not glibc). Other libraries may have other runtime or compile-time options. Output from LeakSanitizer and Valgrind follows:

- gc.c

```
$ clang -o gc gc.c -Wall -g $(pkg-config --cflags --libs glib-2.0) -fsanitize=address
$ ./gc
=================================================================
==233215==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 100 byte(s) in 1 object(s) allocated from:
    #0 0x4bfd2f in malloc (/tmp/gc+0x4bfd2f)
    #1 0x7f1fcf12b908 in g_malloc (/lib64/libglib-2.0.so.0+0x5b908)
    #2 0x7f1fced961e1 in __libc_start_main (/lib64/libc.so.6+0x281e1)

SUMMARY: AddressSanitizer: 100 byte(s) leaked in 1 allocation(s).
$ clang -o gc gc.c -Wall -g $(pkg-config --cflags --libs glib-2.0)
$ valgrind --leak-check=full ./gc
...
==233250== 100 bytes in 1 blocks are definitely lost in loss record 8 of 11
==233250==    at 0x4839809: malloc (vg_replace_malloc.c:307)
==233250==    by 0x48DF908: g_malloc (in /usr/lib64/libglib-2.0.so.0.6600.3)
==233250==    by 0x4011C5: main (gc.c:6)
==233250==
==233250== 256 (96 direct, 160 indirect) bytes in 1 blocks are definitely lost in loss record 9 of 11
==233250==    at 0x4839809: malloc (vg_replace_malloc.c:307)
==233250==    by 0x48DF908: g_malloc (in /usr/lib64/libglib-2.0.so.0.6600.3)
==233250==    by 0x48F71C1: g_slice_alloc (in /usr/lib64/libglib-2.0.so.0.6600.3)
==233250==    by 0x48C5A51: g_hash_table_new_full (in /usr/lib64/libglib-2.0.so.0.6600.3)
==233250==    by 0x401197: main (gc.c:3)
```

## TSAN: Data races
ThreadSanitizer reports data races where multiple threads access data without thread-race protection. (See the ThreadSanitizer documentation.) An example follows:

- data_race.cpp

```
$ clang++ -o data_race data_race.cpp -Wall -g -pthread -fsanitize=thread
zhishan@006IT00075:~/iwork/drive/modules/basis/memory_debugger/sanitize [GIT:main]
$ ./data_race 
==================
WARNING: ThreadSanitizer: data race (pid=36699)
  Write of size 1 at 0x55f554af76a8 by main thread:
    #0 main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/data_race.cpp:13:11 (data_race+0xd03c5) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)

  Previous read of size 1 at 0x55f554af76a8 by thread T1:
    #0 main::$_0::operator()() const /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/data_race.cpp:8:17 (data_race+0xd09c8) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #1 void std::__invoke_impl<void, main::$_0>(std::__invoke_other, main::$_0&&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/invoke.h:61:14 (data_race+0xd098d) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #2 std::__invoke_result<main::$_0>::type std::__invoke<main::$_0>(main::$_0&&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/invoke.h:96:14 (data_race+0xd08dd) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #3 void std::thread::_Invoker<std::tuple<main::$_0> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/std_thread.h:252:13 (data_race+0xd0885) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #4 std::thread::_Invoker<std::tuple<main::$_0> >::operator()() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/std_thread.h:259:11 (data_race+0xd0825) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #5 std::thread::_State_impl<std::thread::_Invoker<std::tuple<main::$_0> > >::_M_run() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/std_thread.h:210:13 (data_race+0xd0739) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #6 <null> <null> (libstdc++.so.6+0xdc2b2) (BuildId: f57e02bfadacc0c923c82457d5e18e1830b5faea)

  Location is global 'flip1' of size 1 at 0x55f554af76a8 (data_race+0x14f96a8)

  Thread T1 (tid=36701, running) created by main thread at:
    #0 pthread_create <null> (data_race+0x4f40d) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)
    #1 std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) <null> (libstdc++.so.6+0xdc388) (BuildId: f57e02bfadacc0c923c82457d5e18e1830b5faea)
    #2 main /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/data_race.cpp:7:17 (data_race+0xd03b4) (BuildId: 08af9cdfbf88a9feee5c0b7508c1c4f88438d829)

SUMMARY: ThreadSanitizer: data race /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/data_race.cpp:13:11 in main
==================
ThreadSanitizer: reported 1 warnings

$ clang++ -o data_race data_race.cpp -Wall -g -pthread
$ ./data_race 

$ valgrind --tool=helgrind ./data_race 
==36766== Helgrind, a thread error detector
==36766== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==36766== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==36766== Command: ./data_race
==36766== 
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x25
### unhandled dwarf2 abbrev form code 0x23
get_Form_szB: unhandled 35 (DW_FORM_rnglistx)
--36766-- WARNING: Serious error when reading debug info
--36766-- When reading debug info from /home/zhishan/iwork/drive/modules/basis/memory_debugger/sanitize/data_race:
--36766-- get_Form_contents: unhandled DW_FORM
==36766== ---Thread-Announcement------------------------------------------
==36766== 
==36766== Thread #1 is the program's root thread
==36766== 
==36766== ---Thread-Announcement------------------------------------------
==36766== 
==36766== Thread #2 was created
==36766==    at 0x4CD5BA3: clone (clone.S:76)
==36766==    by 0x4CD6A9E: __clone_internal (clone-internal.c:83)
==36766==    by 0x4C44758: create_thread (pthread_create.c:295)
==36766==    by 0x4C4527F: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
==36766==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==36766==    by 0x495B388: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==36766==    by 0x1092A9: std::thread::thread<main::$_0, , void>(main::$_0&&) (std_thread.h:142)
==36766==    by 0x1091CB: main (data_race.cpp:7)
==36766== 
==36766== ----------------------------------------------------------------
==36766== 
==36766== Possible data race during write of size 1 at 0x10C071 by thread #1
==36766== Locks held: none
==36766==    at 0x1091CC: main (data_race.cpp:13)
==36766== 
==36766== This conflicts with a previous read of size 1 by thread #2
==36766== Locks held: none
==36766==    at 0x109578: main::$_0::operator()() const (data_race.cpp:8)
==36766==    by 0x10955C: void std::__invoke_impl<void, main::$_0>(std::__invoke_other, main::$_0&&) (invoke.h:61)
==36766==    by 0x1094EC: std::__invoke_result<main::$_0>::type std::__invoke<main::$_0>(main::$_0&&) (invoke.h:96)
==36766==    by 0x1094C4: void std::thread::_Invoker<std::tuple<main::$_0> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:252)
==36766==    by 0x109494: std::thread::_Invoker<std::tuple<main::$_0> >::operator()() (std_thread.h:259)
==36766==    by 0x109418: std::thread::_State_impl<std::thread::_Invoker<std::tuple<main::$_0> > >::_M_run() (std_thread.h:210)
==36766==    by 0x495B2B2: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==36766==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==36766==  Address 0x10c071 is 0 bytes inside data symbol "_ZL5flip1"
==36766== 
==36766== ----------------------------------------------------------------
==36766== 
==36766== Possible data race during read of size 1 at 0x10C071 by thread #2
==36766== Locks held: none
==36766==    at 0x109578: main::$_0::operator()() const (data_race.cpp:8)
==36766==    by 0x10955C: void std::__invoke_impl<void, main::$_0>(std::__invoke_other, main::$_0&&) (invoke.h:61)
==36766==    by 0x1094EC: std::__invoke_result<main::$_0>::type std::__invoke<main::$_0>(main::$_0&&) (invoke.h:96)
==36766==    by 0x1094C4: void std::thread::_Invoker<std::tuple<main::$_0> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:252)
==36766==    by 0x109494: std::thread::_Invoker<std::tuple<main::$_0> >::operator()() (std_thread.h:259)
==36766==    by 0x109418: std::thread::_State_impl<std::thread::_Invoker<std::tuple<main::$_0> > >::_M_run() (std_thread.h:210)
==36766==    by 0x495B2B2: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==36766==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==36766==    by 0x4C44B42: start_thread (pthread_create.c:442)
==36766==    by 0x4CD5BB3: clone (clone.S:100)
==36766== 
==36766== This conflicts with a previous write of size 1 by thread #1
==36766== Locks held: none
==36766==    at 0x1091CC: main (data_race.cpp:13)
==36766==  Address 0x10c071 is 0 bytes inside data symbol "_ZL5flip1"
==36766== 
==36766== ----------------------------------------------------------------
==36766== 
==36766== Possible data race during write of size 1 at 0x10C072 by thread #2
==36766== Locks held: none
==36766==    at 0x109592: main::$_0::operator()() const (data_race.cpp:10)
==36766==    by 0x10955C: void std::__invoke_impl<void, main::$_0>(std::__invoke_other, main::$_0&&) (invoke.h:61)
==36766==    by 0x1094EC: std::__invoke_result<main::$_0>::type std::__invoke<main::$_0>(main::$_0&&) (invoke.h:96)
==36766==    by 0x1094C4: void std::thread::_Invoker<std::tuple<main::$_0> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:252)
==36766==    by 0x109494: std::thread::_Invoker<std::tuple<main::$_0> >::operator()() (std_thread.h:259)
==36766==    by 0x109418: std::thread::_State_impl<std::thread::_Invoker<std::tuple<main::$_0> > >::_M_run() (std_thread.h:210)
==36766==    by 0x495B2B2: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==36766==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==36766==    by 0x4C44B42: start_thread (pthread_create.c:442)
==36766==    by 0x4CD5BB3: clone (clone.S:100)
==36766== 
==36766== This conflicts with a previous read of size 1 by thread #1
==36766== Locks held: none
==36766==    at 0x1091D3: main (data_race.cpp:14)
==36766==  Address 0x10c072 is 0 bytes inside data symbol "_ZL5flip2"
==36766== 
==36766== ----------------------------------------------------------------
==36766== 
==36766== Possible data race during read of size 1 at 0x10C072 by thread #1
==36766== Locks held: none
==36766==    at 0x1091D3: main (data_race.cpp:14)
==36766== 
==36766== This conflicts with a previous write of size 1 by thread #2
==36766== Locks held: none
==36766==    at 0x109592: main::$_0::operator()() const (data_race.cpp:10)
==36766==    by 0x10955C: void std::__invoke_impl<void, main::$_0>(std::__invoke_other, main::$_0&&) (invoke.h:61)
==36766==    by 0x1094EC: std::__invoke_result<main::$_0>::type std::__invoke<main::$_0>(main::$_0&&) (invoke.h:96)
==36766==    by 0x1094C4: void std::thread::_Invoker<std::tuple<main::$_0> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:252)
==36766==    by 0x109494: std::thread::_Invoker<std::tuple<main::$_0> >::operator()() (std_thread.h:259)
==36766==    by 0x109418: std::thread::_State_impl<std::thread::_Invoker<std::tuple<main::$_0> > >::_M_run() (std_thread.h:210)
==36766==    by 0x495B2B2: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==36766==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==36766==  Address 0x10c072 is 0 bytes inside data symbol "_ZL5flip2"
==36766== 
==36766== 
==36766== Use --history-level=approx or =none to gain increased speed, at
==36766== the cost of reduced accuracy of conflicting-access information
==36766== For lists of detected and suppressed errors, rerun with: -s
==36766== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 0 from 0)
```

## Recompiling libraries

AddressSanitizer automatically processes all calls to glibc. This is not the case for other system or user libraries. For AddressSanitizer to function best, one should also recompile such libraries with -fsanitize=address. This is not required with Valgrind.

