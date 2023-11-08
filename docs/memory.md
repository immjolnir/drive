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


## When they say "stack grows down, heap grows up", what exactly does the "growing" of heap mean?
- A1: 
When we say that the _heap_ grows up, we mean that the memory addresses allocated to the _heap_ increase as more memory is allocated. This is in contrast to the _stack_, which grows down in memory address space as more function calls are made and local variables are allocated. The _heap_ is typically used for dynamic memory allocation, where the size of the memory needed is not known at compile time, and can be allocated and deallocated at runtime.

- A2:
It is a memory model where the stack starts at the highest memory address and as it grows, it goes downward in addresses. The heap starts using from the lowest available and goes upward.

Stack allocation occurs for variables defined in methods and a way for the system to keep track of where to return to when a method/function finishes.

Heap memory is used for dynamic allocation, usually the keyword ‘new’ is involved (though it may be called by the library and not directly by the user).

The idea is to allow flexible allocation between the two types without an artificial limit. Consider if both grew downward with the heap starting halfway between the highest and lowest. This creates an artificial limit: you could run out of one type while still having available memory. With up/down approach you run out of stack space only when you run out available memory. Ditto for heap.


- A3:
For Heap, growing just means more heap memory is allocated (used). Its usage is not as linear or predictable as stack. This means there can be ‘holes’ where say address 1000 and 1200 are used, but 1100 is not. But that hole is still usable in the future. Growth means more is used. There are techniques called compaction that moves the heap around to remove holes.

It may sound stupid, but it is unavoidable. For example, with a linked list as data is inserted into the list, it uses more heap memory. If items are removed, then the amount of memory used decreases. The list may last a long time (relatively) and the stack will be growing and shrinking as methods are called without change to the heap. It can’t be done without fragmentation because that would require either equal amounts allocated every time (which is what paging does) or knowing in advance what would be needed.

There are a few allocation techniques that help manage the fragmentation issues so that they are not usually a real problem, and compaction would fix it.

- Memory Organization of a typical program in MIPS
```
                .-------------------------.
High Address    |                         | command-line arguments and environment variables
                .-------------------------.
                |          Stack          |
                .------------|------------.
                |            v            |
                |                         |
                |                         |
                |                         |
                |                         |
                |                         |
                |                         |
                |                         |
                |                         |
                |            ^            |
                .------------|------------.
                |           Heap          |
                .-------------------------.
                |    Uninitialized Data   | bss, initialized to zero by `exec`
                .-------------------------.  
                |    Initialized data     | read from program file by `exec` 
                .-------------------------.
Low Address     |         text            | read from program file by `exec`
                '-------------------------'
```

## [Endianness(字节顺序)](https://en.wikipedia.org/wiki/Endianness)

In computing, __endianness__ is the order or sequence of `byte`s of a word of digital data in computer memory or data communication which is identified by describing the impact of the "first" `byte`s, meaning at the smallest address or sent first. __Endianness__ is primarily expressed as __big-endian__ (BE) or __little-endian__ (LE). A __big-endian__ system stores the most significant `byte` of a word at the smallest memory address and the least significant byte at the largest. A __little-endian__ system, in contrast, stores the least-significant `byte` at the smallest address. __Bi-endianness__ is a feature supported by numerous computer architectures that feature switchable endianness in data fetches and stores or for instruction fetches. Other orderings are generically called __middle-endian__ or __mixed-endian__.

Endianness may also be used to describe the order in which the bits are transmitted over a communication channel, e.g., big-endian in a communications channel transmits the most significant bits first. Bit-endianness is seldom used in other contexts.

Endianness refers to the order in which the bytes of a multi-byte word are stored in memory.

### Overview
Computers store information in various-sized groups of binary bits. Each group is assigned a number, called its address, that the computer uses to access that data. On most modern computers, the smallest data group with an address is eight bits long and is called a byte. Larger groups comprise two or more bytes, for example, a 32-bit word contains four bytes. There are two possible ways a computer could number the individual bytes in a larger group, starting at either end. Both types of endianness are in widespread use in digital electronic engineering. The initial choice of endianness of a new design is often arbitrary, but later technology revisions and updates perpetuate the existing endianness to maintain backward compatibility.

Internally, any given computer will work equally well regardless of what endianness it uses since its hardware will consistently use the same endianness to both store and load its data. For this reason, programmers and computer users normally ignore the endianness of the computer they are working with. However, endianness can become an issue when moving data external to the computer – as when transmitting data between different computers, or a programmer investigating internal computer bytes of data from a memory dump – and the endianness used differs from expectation. In these cases, the endianness of the data must be understood and accounted for.

![](./32bit-Endianess.svg)

These two diagrams show how two computers using different endianness store a 32-bit (four byte) integer with the value of 0x0A0B0C0D. In both cases, the integer is broken into four bytes, 0x0A, 0x0B, 0x0C, and 0x0D, and the bytes are stored in four sequential byte locations in memory, starting with the memory location with address a, then a + 1, a + 2, and a + 3. The difference between big- and little-endian is the order of the four bytes of the integer being stored.

The first diagram shows a computer using little-endian. This starts the storing of the integer with the least-significant byte, 0x0D, at address a, and ends with the most-significant byte, 0x0A, at address a + 3.

The second diagram shows a computer using big-endian. This starts the storing of the integer with the most-significant byte, 0x0A, at address a, and ends with the least-significant byte, 0x0D, at address a + 3.

Since each computer uses its same endianness to both store and retrieve the integer, the results will be the same for both computers. Issues may arise when memory is addressed by bytes instead of integers, or when memory contents are transmitted between computers with different endianness.

Big-endianness is the dominant ordering in __networking protocols__, such as in the Internet protocol suite, where it is referred to as network order, transmitting the most significant byte first. Conversely, little-endianness is the dominant ordering for processor architectures (x86, most ARM implementations, base RISC-V implementations) and their associated memory. File formats can use either ordering; some formats use a mixture of both or contain an indicator of which ordering is used throughout the file.

The styles of big- and little-endian may also be used more generally to characterize the ordering of any representation, e.g. the digits in a numeral system or the sections of a date. Numbers in positional notation are generally written with their digits in left-to-right big-endian order, even in right-to-left scripts. Similarly, programming languages use big-endian digit ordering for numeric literals.

### Basics
Computer memory consists of a sequence of storage cells (smallest addressable units); in machines that support byte addressing, those units are called bytes. Each byte is identified and accessed in hardware and software by its memory address. If the total number of bytes in memory is n, then addresses are enumerated from 0 to n − 1.

Computer programs often use data structures or fields that may consist of more data than can be stored in one byte. In the context of this article where its type cannot be arbitrarily complicated, a "field" consists of a consecutive sequence of bytes and represents a "simple data value" which – at least potentially – can be manipulated by one single hardware instruction. On most systems, the address of a multi-byte simple data value is the address of its first byte (the byte with the lowest address).[note 1]

Another important attribute of a byte being part of a "field" is its "significance". These attributes of the parts of a field play an important role in the sequence the bytes are accessed by the computer hardware, more precisely: by the low-level algorithms contributing to the results of a computer instruction.

### Bit order
__Bit order__ is a concept similar to endianness, but on a level of bits, not bytes. In serial communication, bits in a byte may be transmitted __least-significant bit (lsb)__ or __most significant bit (msb)__ first. RS-232, HDLC, Ethernet, and USB transmit lsb first. Teletext, I2C, SMBus, PMBus, and SONET and SDH transmit msb first. ARINC 429 uses one ordering for its label field and the other ordering for the remainder of the frame.

__Bit order__ is not a critical consideration in computer architectures which typically use parallel structures and it is rare for each individual bit to have a unique address. Individual bits or bit fields are accessed via their bit numbering or, in high-level programming languages, assigned names, the effects of which, however, may be machine dependent or lack software portability.

### Byte swapping
Byte-swapping consists of rearranging bytes to change endianness. Many compilers provide built-ins that are likely to be compiled into native processor instructions (bswap/movbe), such as `__builtin_bswap32`. Software interfaces for swapping include:
- Standard network endianness functions (from/to BE, up to 32-bit). Windows has a 64-bit extension in winsock2.h.
- BSD and Glibc endian.h functions (from/to BE and LE, up to 64-bit).
- macOS OSByteOrder.h macros (from/to BE and LE, up to 64-bit).
- The std::byteswap function in C++23.

### Questions
- Why does a UTF-8 have no byte order?
UTF-8 is a variable-width character encoding that can represent any character in the Unicode standard. Unlike other character encodings such as UTF-16 or UTF-32, UTF-8 does not have a byte order because it uses a specific byte pattern to represent each character, regardless of the endianness of the system. This allows UTF-8 to be used on any platform without requiring any byte order conversion. Therefore, unlike UTF-16, UTF-8 has no endianness issues.
