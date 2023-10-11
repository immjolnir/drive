# [std::string](https://en.cppreference.com/w/cpp/string/basic_string/basic_string)

examples:
- string_size_length_capacity.cpp

## Contruct from iterator
```c++
basic_string( InputIt first, InputIt last, const Allocator& alloc = Allocator());
```

Constructs the string with the contents of the range [`first`, `last`).

- Implementation
```c++
basic_string(_InputIterator __beg, _InputIterator __end, const _Alloc& __a = _Alloc())
  : _M_dataplus(_M_local_data(), __a) {
    _M_construct(__beg, __end, std::__iterator_category(__beg));
}

// libstdc++-v3/include/bits/basic_string.tcc
// NB: This is the special case for Input Iterators, used in
// istreambuf_iterators, etc.
// Input Iterators have a cost structure very different from
// Input Iterators have a cost structure very different from
// pointers, calling for a different coding style.
template <typename _CharT, typename _Traits, typename _Alloc>
template <typename _InIterator>
void basic_string<_CharT, _Traits, _Alloc>::_M_construct(_InIterator __beg, _InIterator __end,
                                                         std::input_iterator_tag) {
    size_type __len = 0;
    size_type __capacity = size_type(_S_local_capacity);

    while (__beg != __end && __len < __capacity) {
        _M_data()[__len++] = *__beg;
        ++__beg;
    }

    __try {
        while (__beg != __end) {
            if (__len == __capacity) {
                // Allocate more space.
                __capacity = __len + 1;
                pointer __another = _M_create(__capacity, __len);
                this->_S_copy(__another, _M_data(), __len);
                _M_dispose();
                _M_data(__another);
                _M_capacity(__capacity);
            }
            _M_data()[__len++] = *__beg;
            ++__beg;
        }
    }
    __catch(...) {
        _M_dispose();
        __throw_exception_again;
    }

    _M_set_length(__len);
}
```
### _M_create

```c++
template <typename _CharT, typename _Traits, typename _Alloc>
typename basic_string<_CharT, _Traits, _Alloc>::pointer basic_string<_CharT, _Traits, _Alloc>::_M_create(
  size_type& __capacity, size_type __old_capacity) {
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 83.  String::npos vs. string::max_size()
    if (__capacity > max_size()) std::__throw_length_error(__N("basic_string::_M_create"));

    // The below implements an exponential growth policy, necessary to
    // meet amortized linear time requirements of the library: see
    // http://gcc.gnu.org/ml/libstdc++/2001-07/msg00085.html.
    if (__capacity > __old_capacity && __capacity < 2 * __old_capacity) {
        __capacity = 2 * __old_capacity;
        // Never allocate a string bigger than max_size.
        if (__capacity > max_size()) __capacity = max_size();
    }

    // NB: Need an array of char_type[__capacity], plus a terminating
    // null char_type() element.
    return _Alloc_traits::allocate(_M_get_allocator(), __capacity + 1);
}

```

## construct from initializer_list

```
basic_string(std::initializer_list<CharT> ilist, const Allocator& alloc = Allocator());
```
- Possible implementation
```c++
/**
 *  @brief  Construct string from an initializer %list.
 *  @param  __l  std::initializer_list of characters.
 *  @param  __a  Allocator to use (default is default allocator).
 */
basic_string(initializer_list<_CharT> __l, const _Alloc& __a = _Alloc()) : _M_dataplus(_M_local_data(), __a) {
    _M_construct(__l.begin(), __l.end());
}
```
