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

