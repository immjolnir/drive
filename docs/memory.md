# Memory

- instrumentation

In the context of computer programming, __instrumentation__ refers to the measure of a product's performance, in order to diagnose errors and to write trace information.

__instrumentation__ can be of two types: source instrumentation and binary instrumentation.


## [Sanitizers](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind)

This article compares two tools, __Sanitizers__ and __Valgrind__, that find memory bugs in programs written in memory-unsafe languages.


- Performance benefits of Sanitizers

`Valgrind` uses dynamic instrumentation instead of static instrumentation at compile time, which leads to the high performance overhead that can be impractical for CPU-intensive applications. `Sanitizers` uses static instrumentation and allows for similar checks with a lower overhead.

If you are accustomed to Valgrind, give AddressSanitizer a try—just add the `-fsanitize=address` compilation plus linking parameter (that is, to all of the CFLAGS, CXXFLAGS, and LDFLAGS) as the first try.

```
-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
```

- Clang option to catch uninitialized memory reads: -fsanitize=memory. That option cannot be combined with -fsanitize=address.




## valgrind

`Valgrind` is an instrumentation framework for building dynamic analysis tools that check C and C++ programs for errors. Memcheck is the default tool Valgrind uses when you don't ask it for another tool. Valgrind Memcheck can detect various memory leaks and keep track of whether memory is accessible and defined.

But what if you have built your own memory manager? Memcheck keeps track of memory by observing the standard malloc/free, new/delete, and mmap/munmap system calls. 

- [Use Valgrind Memcheck with a custom memory manager](https://developers.redhat.com/articles/2022/03/23/use-valgrind-memcheck-custom-memory-manager)

- Replacing malloc

If you simply replace the whole GNU C malloc implementation by defining your own functions with the same names (for example, by using __tcmalloc__ or __jemalloc__), `Valgrind` will, since version 3.12.0, intercept all your replacement function (like malloc, free, and so on) unless you tell it not to. (See the `--soname-synonyms` option in the Valgrind manual for more on this.) After you give your functions these standard names, Memcheck can provide all normal memory-tracking functions, just as if you were using the system's malloc implementation.



## [jemalloc](http://jemalloc.net/)

## [tcmalloc](https://github.com/google/tcmalloc)

## [gperftools](https://github.com/gperftools/gperftools)
(originally Google Performance Tools)

The fastest malloc we’ve seen; works particularly well with threads
and STL. Also: thread-friendly heap-checker, heap-profiler, and
cpu-profiler.

gperftools is a collection of a high-performance multi-threaded
malloc() implementation, plus some pretty nifty performance analysis
tools.


