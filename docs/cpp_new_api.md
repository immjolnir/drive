# New API

## launder
- [What's the purpose of the std::launder?](https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder)

std::launder is aptly named, though only if you know what it's for. It performs memory laundering.

Consider the example in the paper:
```c++
struct X { const int n; };
union U { X x; float f; };
...

U u = {{ 1 }};
```
That statement performs aggregate initialization, initializing the first member of U with {1}.

Because n is a const variable, the compiler is free to assume that u.x.n shall always be 1.

So what happens if we do this:
```c++
X *p = new (&u.x) X {2};
```

Because X is trivial, we need not destroy the old object before creating a new one in its place, so this is perfectly legal code. The new object will have its n member be 2.

So tell me... what will u.x.n return?

The obvious answer will be 2. But that's wrong, because the compiler is allowed to assume that a truly const variable (not merely a const&, but an object variable declared const) will never change. But we just changed it.

[basic.life]/8 spells out the circumstances when it is OK to access the newly created object through variables/pointers/references to the old one. And having a const member is one of the disqualifying factors.

So... how can we talk about u.x.n properly?

We have to launder our memory:

assert(*std::launder(&u.x.n) == 2); //Will be true.
Money laundering is used to prevent people from tracing where you got your money from. Memory laundering is used to prevent the compiler from tracing where you got your object from, thus forcing it to avoid any optimizations that may no longer apply.

Another of the disqualifying factors is if you change the type of the object. std::launder can help here too:
```c++
alignas(int) char data[sizeof(int)];
new(&data) int;
int *p = std::launder(reinterpret_cast<int*>(&data));
```

[basic.life]/8 tells us that, if you allocate a new object in the storage of the old one, you cannot access the new object through pointers to the old. launder allows us to side-step that.

> This function performs the opposite of laundering.

- Some instances
  - arrow: aligned_storage.h

## [destory](https://en.cppreference.com/w/cpp/memory/destroy)

`std::destroy_at()` is not meant to deallocate memory according to [specialized.destroy]p1 it is meant to call the destructor of the object:

```
template<class T>
void destroy_at(T* location);
```

Effects: Equivalent to:
```
location->~T();
```

https://en.cppreference.com/w/cpp/memory/destroy gives an example of why this might be needed. In this case we have used placement new into a buffer and there is no need to free the memory.

The proposal that brought this in [Extending memory management tools](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0040r2.html) says:

> It is common for both the standard library and user libraries to manage memory without the use of standard-compliant allocators. They may use internal buffers (like optional) or use an allocator model that does not manage object lifetime [bde] [sgi] [eastl] [bitsquid]. Such alternative allocator models are common in efficiency-critical applications.

Above from https://stackoverflow.com/questions/52509559/usage-of-stddestroy-at-in-c17


## [construct_at](https://en.cppreference.com/w/cpp/memory/construct_at)

Creates a T object initialized with arguments args... at given address p. Specialization of this function template participates in overload resolution only if `::new(std::declval<void*>()) T(std::declval<Args>()...)` is well-formed in an unevaluated context.

Equivalent to
```c++
return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
```

except that __construct_at__ may be used in evaluation of constant expressions.

When __construct_at__ is called in the evaluation of some constant expression `e`, the argument `p` must point to either storage obtained by `std::allocator<T>::allocate` or an object whose lifetime began within the evaluation of `e`.

```c++
// Since c++20
#include <iostream>
#include <memory>
 
struct S
{
    int x;
    float y;
    double z;
 
    S(int x, float y, double z) : x{x}, y{y}, z{z} { std::cout << "S::S();\n"; }
 
    ~S() { std::cout << "S::~S();\n"; }
 
    void print() const
    {
        std::cout << "S { x=" << x << "; y=" << y << "; z=" << z << "; };\n";
    }
};
 
int main()
{
    alignas(S) unsigned char storage[sizeof(S)];
 
    S* ptr = std::construct_at(reinterpret_cast<S*>(storage), 42, 2.71828f, 3.1415);
    ptr->print();
 
    std::destroy_at(ptr);
}
```