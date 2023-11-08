# CC23

## [from_range](https://en.cppreference.com/w/cpp/ranges/from_range)

std::from_range is a disambiguation tag that can be passed to the constructors of the suitable containers to indicate that the contained member is range constructed.

The corresponding type std::from_range_t can be used in the constructor's parameter list to match the intended tag.


## byteswap
```c++
template< class T >
constexpr T byteswap( T n ) noexcept;
```
Reverses the bytes in the given integer value n.

std::byteswap participates in overload resolution only if T satisfies integral, i.e., T is an integer type. The program is ill-formed if T has padding bits.

- Possible implementation
```c++
template<std::integral T>
constexpr T byteswap(T value) noexcept
{
    static_assert(std::has_unique_object_representations_v<T>, 
                  "T may not have padding bits");
    auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
    std::ranges::reverse(value_representation);
    return std::bit_cast<T>(value_representation);
}
```
