# [aligned_storage](https://en.cppreference.com/w/cpp/types/aligned_storage)

The type defined by `std::aligned_storage<>::type` can be used to create uninitialized memory blocks suitable to hold the objects of given type, optionally aligned stricter than their natural alignment requirement, for example on a cache or page boundary.

As with any other uninitialized storage, the objects are created using `placement new` and destroyed with `explicit destructor` calls.

- Possible implementation

Except for default argument, __aligned_storage__ is expressible in terms of alignas:

```c++
template<std::size_t Len, std::size_t Align = /* default alignment not implemented */>
struct aligned_storage
{
    struct type
    {
        alignas(Align) unsigned char data[Len];
    };
};
```

## [What's the purpose of std::aligned_storage?](https://stackoverflow.com/questions/50271304/what-is-the-purpose-of-stdaligned-storage)

You can use `std::aligned_storage` whenever you wish to decouple memory allocation from object creation.

The cppreference provides a legitimate use case:
```c++
template<class T, std::size_t N>
class static_vector
{
    // properly aligned uninitialized storage for N T's
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
    std::size_t m_size = 0;
...
```

The idea here is that once the `static_vector` is constructed, memory is immediately allocated for `N` objects of type `T`, but no objects of type `T` are created yet.

A common way of using `aligned_storage` is as a chunk of memory where other objects of any type can be manually created and destroyed. It or something like it can be used to implement things like `std::optional` and `std::variant`.

To show the idea behind this, here's the start of writing a class similar to `std::optional`:
```c++
#include <type_traits>
#include <memory>

template <typename T>
class my_optional
{
private:
    std::aligned_storage_t<sizeof(T), alignof(T)> m_storage;
    bool m_valid;
public:
    constexpr my_optional() noexcept : m_valid(false) {}
    constexpr my_optional(const T& obj)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
        : m_valid(false)
    {
        new(static_cast<void*>(&m_storage)) T(obj);
        m_valid = true;
    }
    constexpr const T& operator*() const
    {
        return *static_cast<const T*>(static_cast<const void*>(&m_storage));
    }
    constexpr T& operator*()
    {
        return *static_cast<T*>(static_cast<void*>(&m_storage));
    }
    ~my_optional()
    {
        if (m_valid)
            operator*().~T();
    }
    // Much more, to obey the Rule Of Five and add more functionality...
 };
 ```
`std::aligned_storage` manages aligned storage. Whether you place POD or non-POD objects in the storage is irrelevant.

The purpose of `std::aligned_storage `is that it provides a standardized higher-level utility for managing aligned storage, so that you can write cleaner code with less hassles.



## [Why is std::aligned_storage to be deprecated in C++23 and what to use instead?](https://stackoverflow.com/questions/71828288/why-is-stdaligned-storage-to-be-deprecated-in-c23-and-what-to-use-instead)

I just saw that C++23 plans to deprecate both std::aligned_storage and std::aligned_storage_t as well as std::aligned_union and std::aligned_union_t.


Just a note to future readers that std::aligned_storage and friends are deprecated in C++23:
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1413r3.pdf

Suggested replacement:

- Simple way
```c++
template <typename T>
using unaligned_storage = std::byte[sizeof(T)];
```

- https://github.com/abseil/abseil-cpp/blob/b1dd425423380126f6441ce4fbb6f8f6c75b793a/absl/meta/type_traits.h#L539-L541
```c++
template <size_t Len, size_t Align = type_traits_internal::
                          default_alignment_of_aligned_storage<Len>::value>
using aligned_storage_t = typename std::aligned_storage<Len, Align>::type;
```

- https://github.com/facebook/folly/blob/61a8ac33097d8e2af97e38c139895a1bd7e92a6a/folly/Traits.h#L304-L306
```c++
template <typename T>
using aligned_storage_for_t =
    typename std::aligned_storage<sizeof(T), alignof(T)>::type;
```

