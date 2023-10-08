# Valgrind
Installation should be as simple as decompressing and untarring using bzip2 (XYZ is the version number in the below examples)

```
bzip2 -d valgrind-XYZ.tar.bz2
tar -xf valgrind-XYZ.tar
```

which will create a directory called valgrind-XYZ; change into that directory and run

```
./configure
make
make install
```
Now that you have Valgrind installed, let's look at how to use it.

## [Valgrind Memcheck](https://web.stanford.edu/class/archive/cs/cs107/cs107.1238/resources/valgrind.html)

Valgrind Memcheck is a tool that detects memory leaks and memory errors. Some of the most difficult C bugs come from mismanagement of memory: allocating the wrong size, using an uninitialized pointer, accessing memory after it was freed, overrunning a buffer, and so on. These types of errors are tricky, as they can provide little debugging information, and tracing the observed problem back to underlying root cause can be challenging. Valgrind is here to help!

- Memory Errors Vs. Memory Leaks

Valgrind reports two types of issues: memory errors and memory leaks. When a program dynamically allocates memory and forgets to later free it, it creates a leak. A memory leak generally won't cause a program to misbehave, crash, or give wrong answers, and is not an urgent situation. A memory error, on the other hand, is a red alert. Reading uninitialized memory, writing past the end of a piece of memory, accessing freed memory, and other memory errors can have significant consequences. Memory errors should never be treated casually or ignored. Although this guide describes about how to use Valgrind to find both, keep in mind that errors are by far the primary concern, and memory leaks can generally be resolved later.


- Finding Memory Errors

Memory errors can be truly evil. The more overt ones cause spectacular crashes, but even then it can be hard to pinpoint how and why the crash came about. More insidiously, a program with a memory error can still seem to work correctly because you manage to get "lucky" much of the time. After several "successful" outcomes, you might wishfully write off what appears to be a spurious catastrophic outcome as a figment of your imagination, but depending on luck to get the right answer is not a good strategy. Running under valgrind can help you track down the cause of visible memory errors as well as find lurking errors you don't even yet know about.

Each time valgrind detects an error, it prints information about what it observed. Each item is fairly terse-- the kind of error, the source line of the offending instruction, and a little info about the memory involved, but often it is enough information to direct your attention to the right place. Here is an example of valgrind running on a buggy program:

```
==4651== Invalid write of size 1
==4651==    at 0x80486A4: main (myprogram.c:58)
==4651==  Address 0x4449054 is not stack'd, malloc'd or (recently) free'd
==4651==
==4651== ERROR SUMMARY: 1 errors from 1 contexts
==4651== malloc/free: in use at exit: 0 bytes in 0 blocks.
==4651== malloc/free: 1 allocs, 1 frees, 10 bytes allocated.
==4651== For counts of detected errors, rerun with: -v
==4651== All heap blocks were freed -- no leaks are possible.
```
The ERROR SUMMARY says there is one error, an invalid write of size 1 (byte, that is). 

- Finding Memory Leaks
You can ask Valgrind to report on memory leaks in addition to errors. When you allocate heap memory, but don't free it, that is called a leak. For a small, short-lived program that runs and immediately exits, leaks are quite harmless, but for a project of larger size and/or longevity, a repeated small leak can eventually add up.


Valgrind categorizes leaks using these terms:

  - `definitely lost`: heap-allocated memory that was never `free`d to which the program no longer has a pointer. Valgrind knows that you once had the pointer, but have since lost track of it. This memory is definitely orphaned.

  - `indirectly lost`: heap-allocated memory that was never `free`d to which the only pointers to it also are lost. For example, if you orphan a linked list, the first node would be definitely lost, the subsequent nodes would be indirectly lost.

  - `possibly lost`: heap-allocated memory that was never `free`d to which valgrind cannot be sure whether there is a pointer or not.

  - `still reachable`: heap-allocated memory that was never `free`d to which the program still has a pointer at exit.

- https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

```
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./executable exampleParam1
```
The flags are, in short:

* `--leak-check=full`: "each individual leak will be shown in detail"

* `--show-leak-kinds=all`: Show all of "definite, indirect, possible, reachable" leak kinds in the "full" report.

* `--track-origins=yes`: Favor useful output over speed. This tracks the origins of uninitialized values, which could be very useful for memory errors. Consider turning off if Valgrind is unacceptably slow.

* `--verbose`: Can tell you about unusual behavior of your program. Repeat for more verbosity.

* `--log-file`: Write to a file. Useful when output exceeds terminal space.


## [Valgrind Callgrind](https://web.stanford.edu/class/archive/cs/cs107/cs107.1238/resources/callgrind.html)

`Valgrind Callgrind` is a program that can profile your code and report on its resources usage. It is another tool provided by Valgrind, which also helps detect memory issues. In fact, the Valgrind framework supports a variety of runtime analysis tools, including memcheck (detects memory errors/leaks), massif (reports on heap usage), helgrind (detects multithreaded race conditions), and callgrind/cachegrind (profiles CPU/cache performance). This tool guide will introduce you to the `Valgrind callgrind` profiling tool.

- Code profiling
A code profiler is a tool to analyze a program and report on its resource usage (where "resource" is memory, CPU cycles, network bandwidth, and so on). The first step in program optimization is to gather factual, quantitative data from representative program execution using a profiler. The profiling data will give insights into the patterns and peaks of resource consumption so you can determine if there is a concern at all, and if so, where the problem is concentrated, allowing you to focus your efforts on the passages that most need attention. You can also measure and re-measure with the profiler to verify your efforts are bearing fruit.

Most code profilers operate via a dynamic analysis--- which is to say that they observe an executing program and take live measurements---as opposed to static analysis which examines the source and predicts behavior. Dynamic profiler operate in a variety of ways: some by interjecting counting code into the program, other take samples of its activity at a high-frequency, and others run the program in a simulated environment with built-in monitoring.

The standard C/unix tool for profiling is gprof, the gnu profiler. This no-frills tool is a statistical sampler that tracks time spent at the function-level. It take regular snapshots of a running program and traces the function call stack (just like you did in crash reporter!) to observe activity. You can check out the online gprof manual if you are curious about its features and use.

The Valgrind profiling tools are cachegrind and callgrind. The cachegrind tool simulates the L1/L2 caches and counts cache misses/hits. The callgrind tool counts function calls and the CPU instructions executed within each call and builds a function callgraph. The callgrind tool includes a cache simulation feature adopted from cachegrind, so you can actually use callgrind for both CPU and cache profiling. The callgrind tool works by instrumenting your program with extra instructions that record activity and keep counters.


- Counting instructions with callgrind
To profile, you run your program under Valgrind and explicitly request the callgrind tool (if unspecified, the tool defaults to memcheck).
```
valgrind --tool=callgrind program-to-run program-arguments
```