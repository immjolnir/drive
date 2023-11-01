# [C++11](https://en.wikipedia.org/wiki/C++11)

C++11 is a version of the ISO/IEC 14882 standard for the C++ programming language. C++11 replaced the prior version of the C++ standard, called C++03,[1] and was later replaced by C++14. The name follows the tradition of naming language versions by the publication year of the specification, though it was formerly named C++0x because it was expected to be published before 2010

## Core language runtime performance enhancements
These language features primarily exist to provide some kind of performance benefit, either of memory or of computational speed.
- Rvalue references and move constructors
In C++11, a move constructor of `std::vector<T>` that takes an rvalue reference to an `std::vector<T>` can copy the pointer to the internal C-style array out of the rvalue into the new `std::vector<T>`, then set the pointer inside the rvalue to null. Since the temporary will never again be used, no code will try to access the null pointer, and because the pointer is null, its memory is not deleted when it goes out of scope. Hence, the operation not only forgoes the expense of a deep copy, but is safe and invisible.

- constexpr – Generalized constant expressions
C++ has always had the concept of constant expressions. These are expressions such as 3+4 that will always yield the same results, at compile time and at run time. Constant expressions are optimization opportunities for compilers, and compilers frequently execute them at compile time and hardcode the results in the program. Also, in several places, the C++ specification requires using constant expressions. Defining an array requires a constant expression, and enumerator values must be constant expressions.

C++11 introduced the keyword constexpr, which allows the user to guarantee that a function or object constructor is a compile-time constant.

`constexpr` differs from `consteval`, introduced in C++20, in that the latter must always produce a compile time constant, while `constexpr` does not have this restriction.

- Modification to the definition of plain old data

C++11 relaxed several of the POD rules, by dividing the POD concept into two separate concepts: trivial and standard-layout.

A type that is trivial can be statically initialized. It also means that it is valid to copy data around via memcpy, rather than having to use a copy constructor. The lifetime of a trivial type begins when its storage is defined, not when a constructor completes.

A trivial class or struct is defined as one that:

Has a trivial default constructor. This may use the default constructor syntax (SomeConstructor() = default;).
Has trivial copy and move constructors, which may use the default syntax.
Has trivial copy and move assignment operators, which may use the default syntax.
Has a trivial destructor, which must not be virtual.
Constructors are trivial only if there are no virtual member functions of the class and no virtual base classes. Copy/move operations also require all non-static data members to be trivial.

A type that is standard-layout means that it orders and packs its members in a way that is compatible with C. A class or struct is standard-layout, by definition, provided:

It has no virtual functions
It has no virtual base classes
All its non-static data members have the same access control (public, private, protected)
All its non-static data members, including any in its base classes, are in the same one class in the hierarchy
The above rules also apply to all the base classes and to all non-static data members in the class hierarchy
It has no base classes of the same type as the first defined non-static data member
A class/struct/union is considered POD if it is trivial, standard-layout, and all of its non-static data members and base classes are PODs.

By separating these concepts, it becomes possible to give up one without losing the other. A class with complex move and copy constructors may not be trivial, but it could be standard-layout and thus interoperate with C. Similarly, a class with public and private non-static data members would not be standard-layout, but it could be trivial and thus memcpy-able.


## Core language build-time performance enhancements
- Extern template

In C++03, the compiler must instantiate a template whenever a fully specified template is encountered in a translation unit. If the template is instantiated with the same types in many translation units, this can dramatically increase compile times. There is no way to prevent this in C++03, so C++11 introduced `extern template` declarations, analogous to extern data declarations.

C++03 has this syntax to oblige the compiler to instantiate a template:
```c++
template class std::vector<MyClass>;
```

C++11 now provides this syntax:
```c++
extern template class std::vector<MyClass>;
```
which tells the compiler not to instantiate the template in this translation unit.

- For example: `modules/basis/template/extern_template.cpp`
You should only use `extern template` to force the compiler to not instantiate a template when you know that it will be instantiated somewhere else. It is used to reduce compile time and object file size.
  - https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/


