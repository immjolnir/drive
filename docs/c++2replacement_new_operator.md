# Replacement New Operator dont complain about out of bounds
- my Q: https://stackoverflow.com/questions/77270777/how-to-check-out-of-range-when-using-placement-new-operator

- example: replacement_new_operator_dont_complain_about_out_of_bounds.cpp

```c++
#include <iostream>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

int main() {
    char slab[10] = {'\0'};

    // call to 'SimpleChar::SimpleChar(char)'
    SimpleChar* c0 = new (slab) SimpleChar('a');
    std::cout << c0->c << std::endl;

    SimpleChar* c1 = new (slab + 8) SimpleChar('b');
    std::cout << c1->c << std::endl;

    SimpleChar* c2 =
      new (std::launder(reinterpret_cast<char*>(slab + 80))) SimpleChar('d');  // But how to detect the wrong usage?
    std::cout << c2->c << std::endl;                                           // d

    SimpleChar* c3 = new (slab + 80) SimpleChar('e');  // But how to detect the wrong usage?
    std::cout << c3->c << std::endl;                   // e
}
```

## How to detect it?

- 1. `g++ -Wall -pedantic -O3 -fsanitize=address -g`

- 2. In that case `-fsanitize=undefined`, specifically `-fsanitize=alignment`, may complain because the offsets in your example are misaligned for the type.
```
  $ g++ -Wall -pedantic -O3 -fsanitize=undefined,alignment -g

  $ a.out
  replacement_new_operator_dont_complain_about_out_of_bounds.cpp:14:47: runtime error: constructor call on misaligned address 0x7fff98e065ae for type 'struct SimpleChar', which requires 8 byte alignment
0x7fff98e065ae: note: pointer points here
 0e 52 cf 7f 00 00  00 00 00 00 00 00 00 00  00 11 78 c3 dc d7 81 a9  88 ee 24 52 cf 7f 00 00  00 00
             ^ 
replacement_new_operator_dont_complain_about_out_of_bounds.cpp:4:25: runtime error: reference binding to misaligned address 0x7fff98e065ae for type 'struct __as_base ', which requires 8 byte alignment
0x7fff98e065ae: note: pointer points here
 0e 52 cf 7f 00 00  00 00 00 00 00 00 00 00  00 11 78 c3 dc d7 81 a9  88 ee 24 52 cf 7f 00 00  00 00
             ^ 
```

- 3. cppcheck: static code analysis
```
$ cppcheck --enable=all replacement_new_operator_dont_complain_about_out_of_bounds.cpp

Checking replacement_new_operator_dont_complain_about_out_of_bounds.cpp ...
replacement_new_operator_dont_complain_about_out_of_bounds.cpp:21:54: portability: Undefined behaviour, pointer arithmetic 'slab+80' is out of bounds. [pointerOutOfBounds]
      new (std::launder(reinterpret_cast<char*>(slab + 80))) SimpleChar('d');  // But how to detect the wrong usage?
                                                     ^
replacement_new_operator_dont_complain_about_out_of_bounds.cpp:24:32: portability: Undefined behaviour, pointer arithmetic 'slab+80' is out of bounds. [pointerOutOfBounds]
    SimpleChar* c3 = new (slab + 80) SimpleChar('e');  // But how to detect the wrong usage?
                               ^
replacement_new_operator_dont_complain_about_out_of_bounds.cpp:5:5: style: Struct 'SimpleChar' has a constructor with 1 argument that is not explicit. [noExplicitConstructor]
    SimpleChar(char c_) : c(c_) {}
    ^
```

- valgrind

## What's the correct type of the pre-allocated storage?

> (Also, technically, an array that is used to provide storage for other objects should have element type unsigned char or std::byte, not char.)

From apache/arrow/cpp/src/arrow/util/aligned_storage.h
```c++
        template <typename T>
        class AlignedStorage {
        private:
            typename std::aligned_storage<sizeof(T), alignof(T)>::type data_;
        };
```
- /usr/include/c++/12/type_traits
```c++
template <std::size_t _Len>
struct __aligned_storage_msa {
    union __type {
        unsigned char __data[_Len];

        struct __attribute__((__aligned__)) {
        } __align;
    };
};

/**
 *  @brief Alignment type.
 *
 *  The value of _Align is a default-alignment which shall be the
 *  most stringent alignment requirement for any C++ object type
 *  whose size is no greater than _Len (3.9). The member typedef
 *  type shall be a POD type suitable for use as uninitialized
 *  storage for any object whose size is at most _Len and whose
 *  alignment is a divisor of _Align.
 */
template <std::size_t _Len, std::size_t _Align = __alignof__(typename __aligned_storage_msa<_Len>::__type)>
struct aligned_storage {
    union type {
        unsigned char __data[_Len];

        struct __attribute__((__aligned__((_Align)))) {
        } __align;
    };
};
```
But per https://en.cppreference.com/w/cpp/types/aligned_storage, its Possible implementation
Except for default argument, aligned_storage is expressible in terms of alignas:
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


So, change the code to replacement_new_operator_dont_complain_about_out_of_bounds_with_fix.cpp
```c++
#include <iostream>
#include <new>
#include <type_traits>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

int main() {
    std::aligned_storage<sizeof(SimpleChar), alignof(SimpleChar)>::type slab;

    SimpleChar* p = reinterpret_cast<SimpleChar*>(&slab);

    SimpleChar* c0 = new (p) SimpleChar('a');
    std::cout << c0->c << std::endl;

    SimpleChar* c1 = new (p + 8) SimpleChar('b');
    std::cout << c1->c << std::endl;

    SimpleChar* c2 =
      new (std::launder(reinterpret_cast<char*>(p + 80))) SimpleChar('d');  // But how to detect the wrong usage?
    std::cout << c2->c << std::endl;                                        // d

    SimpleChar* c3 = new (p + 80) SimpleChar('e');  // But how to detect the wrong usage?
    std::cout << c3->c << std::endl;                // e
}
```

Now, the __cppcheck__ cannot work. But `-fsanitize=alignment` still work.

## What's the correct size of the pre-allocated storage?
倍数关系

> size is at most _Len and whose alignment is a divisor of _Align.

> it must be a multiple of SimpleChar's size. Here should be 8, 16, 24,...
