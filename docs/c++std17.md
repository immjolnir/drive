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

## C++17 中的 aligned_alloc 在开源代码中是如何被使用的？
在C++17中， aligned_alloc 函数是用于动态分配对齐内存的C标准库函数。它允许开发者按照指定的字节对齐方式来分配内存空间。

以下是一个aligned_alloc在开源代码中的使用示例：
```c++
#include <cstdlib>
int main() { 
  // 指定需要分配的字节数和对齐要求（例如，假设我们需要16字节对齐） 
  size_t nbytes = 1024; 
  size_t alignment = alignof(std::max_align_t); // 或者自定义如16、32等

  // 使用aligned_alloc分配内存
  void* ptr = aligned_alloc(alignment, nbytes);
  if (ptr == nullptr) {
      std::cerr << "Failed to allocate memory using aligned_alloc" << std::endl;
      return 1;
  }

// 使用分配到的内存...
// （在此处填充或操作 ptr 指向的内存区域）

  // 在使用完内存后，必须通过free函数释放
  free(ptr);
  return 0;
}
```

在这个例子中，
aligned_alloc被用来分配一块至少为nbytes大小并且对齐到alignment倍数地址的内存。
分配成功后，返回一个指向新分配内存块的指针；如果分配失败，则返回nullptr。

请注意，在使用完毕之后，必须调用free函数来释放由 aligned_alloc分配的内存，而不是 delete或 delete[]。这是因为aligned_alloc是C语言级别的内存管理函数，与C++的new/delete操作符不完全对应。

## Why did C++17 introduce std::aligned_alloc?
```c++
void* aligned_alloc( std::size_t alignment, std::size_t size );
```
- Return value
  - On success, returns the pointer to the beginning of newly allocated memory. To avoid a memory leak, the returned pointer must be deallocated with std::free or std::realloc.

- On failure, returns a null pointer.


- https://stackoverflow.com/questions/63871802/why-did-c17-introduce-stdaligned-alloc

- Q:
Aim: To understand the motivation why C++17 introduced std::aligned_alloc for dynamic memory management.

Problem: For the memory allocation in C++, using std::malloc is virtually always discouraged due to various reasons exemplified under In what cases do I use malloc and/or new?. Instead, using new expression is almost always encouraged in low-level code (e.g. see Notes).

Despite this discouragement, I wonder why C++17 introduced std::aligned_alloc which looks like a continuation of std::malloc.

Is there anything in C++17 (and beyond) that cannot be performed by new (or other equivalent utilities encouraged to use, if any)?

- Answer
C++17 bumped up its support for plain C from C99 to C11. It inherited aligned_alloc along with other stuff from C11.

paper: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0063r3.html

So the motivation is to make C features available in C++ for those who want them.

所以，要想使用aligned memory, 最好使用 std::aligned_storage 来实现。

## In what cases do I use malloc and/or new?
- https://stackoverflow.com/questions/184537/in-what-cases-do-i-use-malloc-and-or-new/7970036#7970036

The short answer is: don't use malloc for C++ without a really good reason for doing so. malloc has a number of deficiencies when used with C++, which new was defined to overcome.

Deficiencies fixed by new for C++ code
malloc is not typesafe in any meaningful way. In C++ you are required to cast the return from void*. This potentially introduces a lot of problems:

#include <stdlib.h>

struct foo {
  double d[5];
}; 

int main() {
  foo *f1 = malloc(1); // error, no cast
  foo *f2 = static_cast<foo*>(malloc(sizeof(foo)));
  foo *f3 = static_cast<foo*>(malloc(1)); // No error, bad
}
It's worse than that though. If the type in question is POD (plain old data) then you can semi-sensibly use malloc to allocate memory for it, as f2 does in the first example.

It's not so obvious though if a type is POD. The fact that it's possible for a given type to change from POD to non-POD with no resulting compiler error and potentially very hard to debug problems is a significant factor. For example if someone (possibly another programmer, during maintenance, much later on were to make a change that caused foo to no longer be POD then no obvious error would appear at compile time as you'd hope, e.g.:

struct foo {
  double d[5];
  virtual ~foo() { }
};
would make the malloc of f2 also become bad, without any obvious diagnostics. The example here is trivial, but it's possible to accidentally introduce non-PODness much further away (e.g. in a base class, by adding a non-POD member). If you have C++11/boost you can use is_pod to check that this assumption is correct and produce an error if it's not:

#include <type_traits>
#include <stdlib.h>

foo *safe_foo_malloc() {
  static_assert(std::is_pod<foo>::value, "foo must be POD");
  return static_cast<foo*>(malloc(sizeof(foo)));
}
Although boost is unable to determine if a type is POD without C++11 or some other compiler extensions.

malloc returns NULL if allocation fails. new will throw std::bad_alloc. The behaviour of later using a NULL pointer is undefined. An exception has clean semantics when it is thrown and it is thrown from the source of the error. Wrapping malloc with an appropriate test at every call seems tedious and error prone. (You only have to forget once to undo all that good work). An exception can be allowed to propagate to a level where a caller is able to sensibly process it, where as NULL is much harder to pass back meaningfully. We could extend our safe_foo_malloc function to throw an exception or exit the program or call some handler:

#include <type_traits>
#include <stdlib.h>

void my_malloc_failed_handler();

foo *safe_foo_malloc() {
  static_assert(std::is_pod<foo>::value, "foo must be POD");
  foo *mem = static_cast<foo*>(malloc(sizeof(foo)));
  if (!mem) {
     my_malloc_failed_handler();
     // or throw ...
  }
  return mem;
}
Fundamentally malloc is a C feature and new is a C++ feature. As a result malloc does not play nicely with constructors, it only looks at allocating a chunk of bytes. We could extend our safe_foo_malloc further to use placement new:

#include <stdlib.h>
#include <new>

void my_malloc_failed_handler();

foo *safe_foo_malloc() {
  void *mem = malloc(sizeof(foo));
  if (!mem) {
     my_malloc_failed_handler();
     // or throw ...
  }
  return new (mem)foo();
}
Our safe_foo_malloc function isn't very generic - ideally we'd want something that can handle any type, not just foo. We can achieve this with templates and variadic templates for non-default constructors:

#include <functional>
#include <new>
#include <stdlib.h>

void my_malloc_failed_handler();

template <typename T>
struct alloc {
  template <typename ...Args>
  static T *safe_malloc(Args&&... args) {
    void *mem = malloc(sizeof(T));
    if (!mem) {
       my_malloc_failed_handler();
       // or throw ...
    }
    return new (mem)T(std::forward(args)...);
  }
};
Now though in fixing all the issues we identified so far we've practically reinvented the default new operator. If you're going to use malloc and placement new then you might as well just use new to begin with!


