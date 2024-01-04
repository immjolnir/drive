# C11

## iota
```c++
template< class ForwardIt, class T >
void iota( ForwardIt first, ForwardIt last, T value );
```
Fills the range `[first, last)` with sequentially increasing values, starting with value and repetitively evaluating `++value`.

Equivalent operation:
```
*(first)     = value;
*(first + 1) = ++value;
*(first + 2) = ++value;
*(first + 3) = ++value;
...
```


##  C++11 initializer list syntax


## std::forward_list

`std::forward_list` is a container that supports fast insertion and removal of elements from anywhere in the container. Fast random access is not supported. It is implemented as a singly-linked list. Compared to `std::list` this container provides more space efficient storage when bidirectional iteration is not needed.

- Example: modules/basis/iterator/iterator_category.cpp


## `nullptr` definition
- https://en.cppreference.com/w/cpp/types/nullptr_t

`std::nullptr_t` is the type of the null pointer literal, `nullptr`.
It is a distinct type that is not itself a pointer type or a pointer to member type.
Its values are null pointer constants (see NULL), and may be implicitly converted to any pointer and pointer to member type.

- https://en.cppreference.com/w/cpp/language/nullptr
The keyword `nullptr` denotes the pointer literal. It is a prvalue of type `std::nullptr_t`.
There exist implicit conversions from `nullptr` to null pointer value of any pointer type and any pointer to member type.
Similar conversions exist for any null pointer constant, which includes values of type `std::nullptr_t` as well as the macro `NULL`.


- libstdc++-v3/include/bits/c++config

```
namespace std
{
  typedef __SIZE_TYPE__     size_t;
  typedef __PTRDIFF_TYPE__  ptrdiff_t;

#if __cplusplus >= 201103L
  typedef decltype(nullptr) nullptr_t;
#endif
}

```

## std::atomic_store的原理是什么？
std::atomic_store 是C++中的一个原子操作函数，用于将给定的值存储到原子对象中。其原理主要涉及以下几个方面：

- 原子性：
  - std::atomic_store保证在多线程环境下的操作原子性，这意味着该操作不会被其他线程中断，会作为一个不可分割的单位执行。这通过硬件级别的支持来实现，例如现代处理器提供的锁前缀指令（如x86架构的 lock指令）或者专门的原子指令（如ARM架构的LDREX和STREX指令）。

- 内存序：
  - std::atomic_store还允许指定内存顺序（memory order）。内存顺序控制了原子操作与其他内存操作之间的可见性和排序关系。这可以确保在多线程环境中的同步行为符合程序员的预期。例如，使用std::memory_order_release可以在store操作后使所有先前的写操作对其他线程变得可见。

- 同步机制：
  - 在不支持硬件级别原子操作的平台上，std::atomic_store可能会退化为使用锁或者其他同步机制来模拟原子操作的效果。这通常涉及到更复杂的实现，可能会引入额外的性能开销。

简而言之，std::atomic_store的原理是利用硬件或软件同步机制来确保在多线程环境中的存储操作是原子的，并且可以根据指定的内存顺序控制操作的可见性和顺序关系，从而提供一种安全的方式来修改共享变量的值。


## c++在申请堆空间时，可以按页或段来申请吗？请举例说明

在C++中，标准库并没有直接提供按页或段来申请堆空间的接口。
系统级别的内存分配通常是由操作系统内核管理的，并且以页为基本单位进行分配。

尽管如此，C++中的 new 和malloc 函数并不直接暴露这样的细粒度控制，它们主要用来分配连续的一块内存区域，大小由程序员指定。
然而，在某些操作系统或环境下，可以通过调用特定的系统调用来实现按照页或段来申请内存。

例如，在POSIX兼容的系统（如Unix、Linux）中，可以使用 mmap函数来映射虚拟内存区域，这样就能以页为单位进行内存分配：
```c
#include <sys/mman.h>

int main() {
    // 分配一页内存（假设一页大小为4096字节）
    size_t page_size = sysconf(_SC_PAGESIZE);
    void* ptr = mmap(nullptr, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // 使用这块内存...

    // 当不再需要时，解除映射
    if (munmap(ptr, page_size) == -1) {
        perror("munmap failed");
        return 1;
    }

    return 0;
}
```

上述代码示例展示了如何通过 mmap函数请求操作系统分配一页大小的内存，这里的“页”是操作系统的最小内存分配单位，大小可通过sysconf查询。请注意，
mmap是一个低层次的操作，使用它进行内存管理通常更为复杂，而且需要开发者自行处理__内存边界对齐__和其他与页相关的细节问题。

对于常规程序开发，除非有特殊需求（如实现内存池或共享内存等），否则更常见的是使用 new 和 delete或者 malloc和 free来动态地申请和释放内存。

## 开发者如何处理内存边界对齐？
在C++中处理内存边界对齐主要是为了确保数据结构在内存中的布局满足特定平台硬件对于读取效率和正确性的要求。

以下是一些开发者可以用来控制内存对齐的方法：
- 编译器默认对齐规则：
  - 编译器会根据目标平台的架构自动为基本数据类型（如int、double等）以及用户自定义结构体进行内存对齐。
  - 编译器设置的对齐规则通常基于数据类型的大小，将对象或结构体成员放在适当倍数的地址上。

- 结构体内的填充（Padding）：
• 为了满足结构体内每个成员的对齐要求，编译器可能在结构体的成员之间插入额外的字节作为填充，使得每个成员都能在其自然对齐边界上开始。
预处理器指令 #pragma pack：
• 开发者可以使用
#pragma pack(n)• 来显式指定结构体的对齐方式，其中n是一个数值，表示新的对齐边界值（按字节计算）。例如，
#pragma pack(4)• 意味着所有后续声明的结构体成员将以4字节边界对齐。不过，这可能会牺牲一定的性能或者导致不兼容某些硬件平台的要求。

attribute((aligned(N)))：
• 在GCC和其他支持此特性的编译器中，可以使用
__attribute__((aligned(N)))• 来指定变量或结构体的对齐方式。这对于需要手动调整对齐以优化性能或兼容特殊硬件接口的情况非常有用。

专用API或系统调用：
• 在某些情况下，特别是对于大块内存分配时，可以使用操作系统提供的API（如POSIX的 memalign 或 posix_memalign• 函数），它们允许程序员按照特定的对齐粒度来分配内存。

C++17 中的 aligned_alloc：
• C++17标准引入了 std::aligned_alloc• 函数，它允许你按照指定的对齐方式进行动态内存分配。

综上所述，处理内存边界对齐时需要考虑性能需求、跨平台兼容性和硬件限制等因素，并适当利用语言特性或平台API来进行有效管理。


