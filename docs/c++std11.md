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

