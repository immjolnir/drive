# Alignment

- [word](https://en.wikipedia.org/wiki/Word_(computer_architecture))

In computing, a __word__ is the natural unit of data used by a particular processor design. A word is a fixed-sized datum handled as a unit by the instruction set or the hardware of the processor. The number of bits or digits in a word (the __word size__, __word width__, or __word length__) is an important characteristic of any specific processor design or computer architecture.

The size of a word is reflected in many aspects of a computer's structure and operation; the majority of the registers in a processor are usually word-sized and the largest datum that can be transferred to and from the working memory in a single operation is a word in many (not all) architectures. The largest possible address size, used to designate a location in memory, is typically a hardware word (here, "hardware word" means the full-sized natural word of the processor, as opposed to any other definition used).

- Uses of word
Depending on how a computer is organized, word-size units may be used for:
  - Fixed Point Number
  
  - Addresses: Holders for memory addresses must be of a size capable of expressing the needed range of values but not be excessively large, so often the size used is the word though it can also be a multiple or fraction of the word size.

  - Registers: Processor registers are designed with a size appropriate for the type of data they hold, e.g. integers, floating-point numbers, or addresses. Many computer architectures use general-purpose registers that are capable of storing data in multiple representations.

- Linux Find If Processor (CPU) is 64 bit / 32 bit
  - getconf LONG_BIT
  - lscpu
```
$ lscpu
Architecture:           x86_64
  CPU op-mode(s):       32-bit, 64-bit
  Address sizes:        39 bits physical, 48 bits virtual
  Byte Order:           Little Endian
CPU(s):                 16
  On-line CPU(s) list:  0-15
Vendor ID:              GenuineIntel
  Model name:           Intel(R) Core(TM) i9-9900K CPU @ 3.60GHz
    CPU family:         6
    Model:              158
    Thread(s) per core: 2
    Core(s) per socket: 8
    Socket(s):          1
    Stepping:           12
    CPU max MHz:        5000.0000
    CPU min MHz:        800.0000
    BogoMIPS:           7200.00
    Flags:              fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon 
                        pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt 
                        tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single ssbd ibrs ibpb stibp fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx
                         rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d arch_capabilities
Caches (sum of all):    
  L1d:                  256 KiB (8 instances)
  L1i:                  256 KiB (8 instances)
  L2:                   2 MiB (8 instances)
  L3:                   16 MiB (1 instance)
NUMA:                   
  NUMA node(s):         1
  NUMA node0 CPU(s):    0-15
Vulnerabilities:        
  Itlb multihit:        KVM: Mitigation: VMX unsupported
  L1tf:                 Not affected
  Mds:                  Mitigation; Clear CPU buffers; SMT vulnerable
  Meltdown:             Not affected
  Mmio stale data:      Mitigation; Clear CPU buffers; SMT vulnerable
  Retbleed:             Mitigation; IBRS
  Spec store bypass:    Mitigation; Speculative Store Bypass disabled via prctl and seccomp
  Spectre v1:           Mitigation; usercopy/swapgs barriers and __user pointer sanitization
  Spectre v2:           Mitigation; IBRS, IBPB conditional, RSB filling, PBRSB-eIBRS Not affected
  Srbds:                Mitigation; Microcode
  Tsx async abort:      Mitigation; TSX disabled

```
## [Purpose of memory alignment](https://stackoverflow.com/questions/381244/purpose-of-memory-alignment/381368#381368)

The memory subsystem on a modern processor is restricted to accessing memory at the granularity and alignment of its word size; this is the case for a number of reasons.

- Speed

Modern processors have multiple levels of cache memory that data must be pulled through; supporting single-byte reads would make the memory subsystem throughput tightly bound to the execution unit throughput (aka cpu-bound); this is all reminiscent of how PIO mode was surpassed by DMA for many of the same reasons in hard drives.

The CPU always reads at its word size (4 bytes on a 32-bit processor), so when you do an unaligned address access — on a processor that supports it — the processor is going to read multiple words. The CPU will read each word of memory that your requested address straddles. This causes an amplification of up to 2X the number of memory transactions required to access the requested data.

Because of this, it can very easily be slower to read two bytes than four. For example, say you have a struct in memory that looks like this:
```
struct mystruct {
    char c;  // one byte
    int i;   // four bytes
    short s; // two bytes
}
```

The processor can read each of these members in one transaction.

- Range
For any given address space, if the architecture can assume that the 2 LSBs are always 0 (e.g., 32-bit machines) then it can access 4 times more memory (the 2 saved bits can represent 4 distinct states), or the same amount of memory with 2 bits for something like flags. Taking the 2 LSBs off of an address would give you a 4-byte alignment; also referred to as a stride of 4 bytes. Each time an address is incremented it is effectively incrementing bit 2, not bit 0, i.e., the last 2 bits will always continue to be 00.

This can even affect the physical design of the system. If the address bus needs 2 fewer bits, there can be 2 fewer pins on the CPU, and 2 fewer traces on the circuit board.

- Atomicity

The CPU can operate on an aligned word of memory atomically, meaning that no other instruction can interrupt that operation. This is critical to the correct operation of many lock-free data structures and other concurrency paradigms.

- Conclusion
The memory system of a processor is quite a bit more complex and involved than described here; a discussion on how an x86 processor actually addresses memory can help (many processors work similarly).

There are many more benefits to adhering to memory alignment that you can read at this IBM article.

A computer's primary use is to transform data. Modern memory architectures and technologies have been optimized over decades to facilitate getting more data, in, out, and between more and faster execution units–in a highly reliable way.

- Bonus: Caches
Another alignment-for-performance that I alluded to previously is alignment on cache lines which are (for example, on some CPUs) 64B.

For more info on how much performance can be gained by leveraging caches, take a look at [Gallery of Processor Cache Effects](http://igoro.com/archive/gallery-of-processor-cache-effects/); from this question on cache-line sizes

> Understanding of cache lines can be important for certain types of program optimizations. For example, the alignment of data may determine whether an operation touches one or two cache lines. As we saw in the example above, this can easily mean that in the misaligned case, the operation will be twice slower.

- [Lock-free Data Structures. Basics: Atomicity and Atomic Primitives](https://kukuruku.co/hub/cpp/lock-free-data-structures-basics-atomicity-and-atomic-primitives)

## Cache Lines

Caches consist of lines, each holding multiple adjacent words.

- On Core i7, cache lines hold 64 bytes.
  - 64-byte lines common for Intel/AMD processors.
  - 64 bytes = 16 32-bit values, 8 64-bit values, etc.
    - E.g., 16 32-bit array elements.

- Main memory read/written in terms of cache lines.
  - Read byte not in cache ⇒ read full cache line from main memory.
  - Write byte ⇒ write full cache line to main memory (eventually).

```
           .------------------ ... ------------------------.
Cache Line |   0   |   1   |   ...                |   63   |
(64 Bytes) '------------------ ... ------------------------'
```



-----------------------------
## Word Size
- Q: What does it mean by word size in computer? Is it the length of the data bus, address bus?

- A: "Word size" refers to the number of bits processed by a computer's CPU in one go (these days, typically 32 bits or 64 bits). Data bus size, instruction size, address size are usually multiples of the word size.

Consider the following:

CPU (processor), RAM (main memory), I/O devices (mouse, keyboard, printer), Bus (data transfer component).

How would you like these computer parts to communicate and transfer data? you definitely need a fixed size of bits to be considered a single unit of data.

For that, Computer scientists agreed to standardize this unit to be 32 bits or 64 bits (depending on the manufacturer choice).

They gave this unit a name and called it a Word.

So a Word is nothing but a unit of data (bunch of bits (signal charges of zeros and ones)) that moves around from a computer component to another.

> The word size is often the same as the pointer size.



```
$ cat /proc/cpuinfo


processor	: 15
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i9-9900K CPU @ 3.60GHz
stepping	: 12
microcode	: 0xea
cpu MHz		: 3600.000
cache size	: 16384 KB
physical id	: 0
siblings	: 16
core id		: 7
cpu cores	: 8
apicid		: 15
initial apicid	: 15
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single ssbd ibrs ibpb stibp fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass mds swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 7200.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

```


## [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment)

Every object type has the property called alignment requirement, which is a nonnegative integer value (of type std::size_t, and always a power of two) representing the number of bytes between successive addresses at which objects of this type can be allocated.

The alignment requirement of a type can be queried with alignof or std::alignment_of. The pointer alignment function std::align can be used to obtain a suitably-aligned pointer within some buffer, and std::aligned_storage can be used to obtain suitably-aligned storage.

(since C++11)
Each object type imposes its alignment requirement on every object of that type; stricter alignment (with larger alignment requirement) can be requested using alignas (since C++11).

In order to satisfy alignment requirements of all non-static members of a class, padding bits may be inserted after some of its members.

## [alignof operator](https://en.cppreference.com/w/cpp/language/alignof) in C++

In C++11 the alignof operator used to returns the alignment, in bytes of the specified type.

- Syntax:
```
alignof(type)
```

- Syntax Explanation:
  - alignof: operator returns the alignment in byte, required for instances of type, which type is either complete type, array type or a reference type.

  - array type: alignment requirement of the element type is returned.

  - reference type: the operator returns the alignment of referenced type.

- Return Value: The alignof operator typically used to returns a value of type `std::size_t`.

## [alignas](https://en.cppreference.com/w/cpp/language/alignas)

Specifies the alignment requirement of a type or an object.

```
#include <iostream>
 
// every object of type struct_float will be aligned to alignof(float) boundary
// (usually 4):
struct alignas(float) struct_float
{
    // your definition here
};
 
// every object of type sse_t will be aligned to 32-byte boundary:
struct alignas(32) sse_t
{
    float sse_data[4];
};
 
// the array "cacheline" will be aligned to 64-byte boundary:
alignas(64) char cacheline[64];
 
int main()
{
    struct default_aligned
    {
        float data[4];
    } a, b, c;
    sse_t x, y, z;
 
    std::cout
        << "alignof(struct_float) = " << alignof(struct_float) << '\n'
        << "sizeof(sse_t) = " << sizeof(sse_t) << '\n'
        << "alignof(sse_t) = " << alignof(sse_t) << '\n'
        << "alignof(cacheline) = " << alignof(alignas(64) char[64]) << '\n'
        << std::hex << std::showbase
        << "&a: " << &a << '\n'
        << "&b: " << &b << '\n'
        << "&c: " << &c << '\n'
        << "&x: " << &x << '\n'
        << "&y: " << &y << '\n'
        << "&z: " << &z << '\n';
}
```

## [aligned_storage](https://en.cppreference.com/w/cpp/types/aligned_storage)

- Possible implementation
```
template <std::size_t Len, std::size_t Align>
struct aligned_storage {
    struct type {
        alignas(Align) unsigned char data[Len];
    };
};
```

The type defined by `std::aligned_storage<>::type` can be used to create uninitialized memory blocks suitable to hold the objects of given type, optionally aligned stricter than their natural alignment requirement, for example on a cache or page boundary.

As with any other uninitialized storage, the objects are created using [placement new](https://en.cppreference.com/w/cpp/language/new) and destroyed with explicit destructor calls.


