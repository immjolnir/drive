# Type Casting

- static_cast
- const_cast
- reinterpret_cast
- dynamic_cast

## reinterpret_cast

`reinterpret_cast` only guarantees that if you cast a __pointer__ to a different type, and then `reinterpret_cast` it back to the original type, you get the original value.

 


### reinterpret_cast vs static_cast
- https://stackoverflow.com/questions/573294/when-to-use-reinterpret-cast

The C++ standard guarantees the following:

- `static_casting` a pointer to and from `void*` preserves the address. That is, in the following, a, b and c all point to __the same address__:
```c++
int* a = new int();
void* b = static_cast<void*>(a);
int* c = static_cast<int*>(b);
```

- `reinterpret_cast` only guarantees that if you cast a pointer to a different type, and then `reinterpret_cast` it back to the original type, you get __the original value__. So in the following:
```c++
int* a = new int();
void* b = reinterpret_cast<void*>(a);
int* c = reinterpret_cast<int*>(b);
```
a and c contain the same value, but the value of b is unspecified. (in practice it will typically contain the same address as a and c, but that's not specified in the standard, and it may not be true on machines with more complex memory systems.)

For casting to and from void*, static_cast should be preferred.

### use case of reinterpret_cast
It is true that `reinterpret_cast` is __not portable__ because of one reason - __byte order (endianness)__.

But this is often surprisingly the best reason to use it.

Let's imagine the example: you have to read binary 32bit number from file, and you know it is big endian.

Your code has to be generic and has to work properly on big endian (e.g. some ARM) and little endian (e.g. x86) systems. So you have to check the byte order. 
You can write a function to achieve this:

```c++
 /*constexpr*/ bool is_little_endian() {
  std::uint16_t x=0x0001;
  auto p = reinterpret_cast<std::uint8_t*>(&x);
  return *p != 0;
}
```

- Explanation: 
The binary representation of x in memory could be `0000'0000'0000'0001` (big) or `0000'0001'0000'0000` (little endian). After `reinterpret-casting` the byte under p pointer could be respectively `0000'0000` or `0000'0001`.
If you use `static-casting`, it will always be `0000'0001`, no matter what endianness is being used.

- Practice: modules/basis/type_casting.cpp

## const_cast

### reinterpret_cast vs const_cast

reinterpret_cast can convert between unrelated pointer types, but can't remove const or volatile qualifiers. You need const_cast for that.
```c++

```