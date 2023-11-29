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