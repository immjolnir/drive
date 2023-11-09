# iterator

- std::istreambuf_iterator

- Answer to the example: `modules/basis/iterator/read_whole_file.cpp`
  - istreambuf_iterator
    ```
    // 24.5.3 Template class istreambuf_iterator
    /// Provides input iterator semantics for streambufs.
    template <typename _CharT, typename _Traits>
    class istreambuf_iterator {
    public:
        ///  Construct start of input stream iterator.
        istreambuf_iterator(istream_type& __s) _GLIBCXX_USE_NOEXCEPT : _M_sbuf(__s.rdbuf()), _M_c(traits_type::eof()) {}

        /// Advance the iterator.  Calls streambuf.sbumpc().
        istreambuf_iterator& operator++() {
            __glibcxx_requires_cond(_M_sbuf && (!_S_is_eof(_M_c) || !_S_is_eof(_M_sbuf->sgetc())),
                                    _M_message(__gnu_debug::__msg_inc_istreambuf)._M_iterator(*this));

            _M_sbuf->sbumpc();
            _M_c = traits_type::eof();
            return *this;
        }

    private:
        // 24.5.3 istreambuf_iterator
        // p 1
        // If the end of stream is reached (streambuf_type::sgetc()
        // returns traits_type::eof()), the iterator becomes equal to
        // the "end of stream" iterator value.
        // NB: This implementation assumes the "end of stream" value
        // is EOF, or -1.
        mutable streambuf_type* _M_sbuf;
        int_type _M_c;
    };
    ```

  - /usr/include/c++/12/streambuf
    ```
    /**
    *  @brief  Getting the next character.
    *  @return  The next character, or eof.
    *
    *  If the input read position is available, returns that character
    *  and increments the read pointer, otherwise calls and returns
    *  @c uflow().
    */
    int_type sbumpc() {
        int_type __ret;
        if (__builtin_expect(this->gptr() < this->egptr(), true)) {
            __ret = traits_type::to_int_type(*this->gptr());
            this->gbump(1);
        } else
            __ret = this->uflow();
        return __ret;
    }
    ```

## [std::iter_swap](https://en.cppreference.com/w/cpp/algorithm/iter_swap)
Swaps the values of the elements the given iterators are pointing to.

```c++
template< class ForwardIt1, class ForwardIt2 >
void iter_swap( ForwardIt1 a, ForwardIt2 b );
```

Possible implementation
```c++
template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) // constexpr since C++20
{
    using std::swap;
    swap(*a, *b);
}
```

See `iter_swap.cpp`

## [iterator_tags](https://en.cppreference.com/w/cpp/iterator/iterator_tags)
- Definition
```c++
//  * <iterator>

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};
```

- Inherit Graph
```
input_iterator_tag
   |_ forward_iterator_tag
          |_ bidirectional_iterator_tag
               |_ random_access_iterator_tag
                     |_ contiguous_iterator_tag

output_iterator_tag
```

- Iterator_category

__Iterator_category__ is an iterator tag function: it is used to determine the category to which an iterator belongs.
Specifically, every iterator must belong to a type that is a model of the concept

  - Output Iterator
  - Input Iterator
  - Forward Iterator
  - Bidirectional Iterator
    - Increment and decrement are inverses of each other:
       - If a is incrementable and bool(a == b), then bool(--(++a) == b).
       - If a is decrementable and bool(a == b), then bool(++(--a) == b).
  - Random Access Iterator

- Examples
  - modules/basis/iterator/iterator_category_part1.cpp
  - modules/basis/iterator/iterator_category_part2.cpp
