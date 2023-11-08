# C++17

## std::byte

# [uninitialized_copy](https://en.cppreference.com/w/cpp/memory/uninitialized_copy)

Possible implementation
```c++
template<class InputIt, class NoThrowForwardIt>
NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last, NoThrowForwardIt d_first)
{
    using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
    NoThrowForwardIt current = d_first;
    try
    {
        for (; first != last; ++first, (void) ++current)
            ::new (static_cast<void*>(std::addressof(*current))) T(*first);
        return current;
    }
    catch (...)
    {
        for (; d_first != current; ++d_first)
            d_first->~T();
        throw;
    }
}
```
