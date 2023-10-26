# Trival Type

See
- aligned_storage_test.cc

From [Trivial, standard-layout, POD, and literal types](https://learn.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170) and [C++20: Aggregate, POD, trivial type, standard layout class, what is what](https://andreasfertig.blog/2021/01/cpp20-aggregate-pod-trivial-type-standard-layout-class-what-is-what/)
I like to give you some details about the new definition in C++20 of a:

- POD
- Trivial type
- Standard layout class
- Aggregate

The term layout refers to how the members of an object of class, struct or union type are arranged in memory. In some cases, the layout is well-defined by the language specification. But when a class or struct contains certain C++ language features such as virtual base classes, virtual functions, members with different access control, then the compiler is free to choose a layout. That layout may vary depending on what optimizations are being performed and in many cases the object might not even occupy a contiguous area of memory. For example, if a class has virtual functions, all the instances of that class might share a single virtual function table. Such types are very useful, but they also have limitations. Because the layout is undefined they cannot be passed to programs written in other languages, such as C, and because they might be non-contiguous they cannot be reliably copied with fast low-level functions such as memcopy, or serialized over a network.

To enable compilers as well as C++ programs and metaprograms to reason about the suitability of any given type for operations that depend on a particular memory layout, C++14 introduced three categories of simple classes and structs: trivial, standard-layout, and POD or Plain Old Data. The Standard Library has the function templates `is_trivial<T>`, `is_standard_layout<T>` and `is_pod<T>` that determine whether a given type belongs to a given category.



## Trivial types
When a class or struct in C++ has compiler-provided or explicitly defaulted special member functions, then it is a trivial type. __It occupies a contiguous memory area__. It can have members with different access specifiers. In C++, the compiler is free to choose how to order members in this situation. Therefore, you can __memcopy__ such objects but you cannot reliably consume them from a C program. A trivial type T can be copied into an array of char or unsigned char, and safely copied back into a T variable. Note that because of alignment requirements, there might be padding bytes between type members.

In short, a trivial type is a class or struct for which the compiler provides all the special members, either implicitly or because they are explicitly defaulted by us. Once we provide our own default constructor for a class, such a type is no longer a trivial type. Another property of a trivial type is that such a type occupies a contiguous memory area, which makes it memcopy-able, and it supports static initialization.

Trivial types have a trivial default constructor, trivial copy constructor, trivial copy assignment operator and trivial destructor. In each case, trivial means the constructor/operator/destructor is not user-provided and belongs to a class that has

- no virtual functions or virtual base classes,

- no base classes with a corresponding non-trivial constructor/operator/destructor

- no data members of class type with a corresponding non-trivial constructor/operator/destructor

The following examples show trivial types. In Trivial2, the presence of the Trivial2(int a, int b) constructor requires that you provide a default constructor. For the type to qualify as trivial, you must explicitly default that constructor.

```c++
struct Trivial
{
   int i;
private:
   int j;
};

struct Trivial2
{
   int i;
   Trivial2(int a, int b) : i(a), j(b) {}
   Trivial2() = default;
private:
   int j;   // Different access control
};
```

Alignment and also the size of the type may be different from char due to alignment and padding rules.

## Standard layout types
When a class or struct does not contain certain C++ language features such as virtual functions which are not found in the C language, and all members have the same access control, it is a standard-layout type. It is memcopy-able and the layout is sufficiently defined that it can be consumed by C programs. Standard-layout types can have user-defined special member functions. In addition, standard layout types have these characteristics:

The term layout refers to the arrangement of members of classes, structs, or unions in memory. A standard-layout class defines a type, which does not use certain specific C++ features that are not available in C. Such a type is memcopy-able, and its layout is defined in a way that the same type can be used in a C program. So in more general speak, a standard-layout class (or type) is compatible with C and can be exchanged over a C-API.


- no virtual functions or virtual base classes

- all non-static data members have the same access control

- all non-static members of class type are standard-layout

- any base classes are standard-layout

- has no base classes of the same type as the first non-static data member.

- meets one of these conditions:

  - no non-static data member in the most-derived class and no more than one base class with non-static data members, or

  - has no base classes with non-static data members

The following code shows one example of a standard-layout type:
```c++
struct SL
{
   // All members have same access:
   int i;
   int j;
   SL(int a, int b) : i(a), j(b) {} // User-defined constructor OK
};
```

The last two requirements can perhaps be better illustrated with code. In the next example, even though Base is standard-layout, Derived is not standard layout because both it (the most derived class) and Base have non-static data members:
```c++
struct Base
{
   int i;
   int j;
};

// std::is_standard_layout<Derived> == false!
struct Derived : public Base
{
   int x;
   int y;
};
```

In this example Derived is standard-layout because Base has no non-static data members:

```c++
struct Base
{
   void Foo() {}
};

// std::is_standard_layout<Derived> == true
struct Derived : public Base
{
   int x;
   int y;
};
```
Derived would also be standard-layout if Base had the data members and Derived had only member functions.

Please note that at this point, we talk about the layout in the memory and the interoperability with C. What you do not see in the definition above is that a standard-layout class could have special members. They do not change the memory layout. Special members only help to initialize an object. Despite that C has no special members, we can have them in C++ in a standard-layout type because it is just about the layout, nothing else.

## POD types

When a class or struct is both trivial and standard-layout, it is a POD (Plain Old Data) type. The memory layout of POD types is therefore contiguous and each member has a higher address than the member that was declared before it, so that byte for byte copies and binary I/O can be performed on these types. Scalar types such as int are also POD types. POD types that are classes can have only POD types as non-static data members.

The following example shows the distinctions between trivial, standard-layout, and POD types:

```c++
#include <type_traits>
#include <iostream>

using namespace std;

struct B
{
protected:
   virtual void Foo() {}
};

// Neither trivial nor standard-layout
struct A : B
{
   int a;
   int b;
   void Foo() override {} // Virtual function
};

// Trivial but not standard-layout
struct C
{
   int a;
private:
   int b;   // Different access control
};

// Standard-layout but not trivial
struct D
{
   int a;
   int b;
   D() {} //User-defined constructor
};

struct POD
{
   int a;
   int b;
};

int main()
{
   cout << boolalpha;
   cout << "A is trivial is " << is_trivial<A>() << endl; // false
   cout << "A is standard-layout is " << is_standard_layout<A>() << endl;  // false

   cout << "C is trivial is " << is_trivial<C>() << endl; // true
   cout << "C is standard-layout is " << is_standard_layout<C>() << endl;  // false

   cout << "D is trivial is " << is_trivial<D>() << endl;  // false
   cout << "D is standard-layout is " << is_standard_layout<D>() << endl; // true

   cout << "POD is trivial is " << is_trivial<POD>() << endl; // true
   cout << "POD is standard-layout is " << is_standard_layout<POD>() << endl; // true

   return 0;
}
```

Before C++20, we had the definition of a POD type. The specification was that a POD is a type that is trivial and standard-layout. With C++20, the definition of POD, as well as the type-trait std::is_pod, is gone. No worries, your favorite STL vendor will for sure provide the type-trait for some time before it actually gets removed.

The idea of a POD was that it supports two distinct properties:

- we can compile a POD in C++ and still use it in a C program, as it has the same memory layout in both languages (meet by standard-layout);
- a POD supports static initialization (meet by trivial type).

While a standard-layout type has a C compatible memory layout, it can have a user-defined default constructor. This is something C doesn't have. Hence we need the second property, a trivial type. As we learned above, such a type is default constructible the same way as a C struct.

As far as the C++20 standard is concerned, the term POD no longer exists. POD is replaced by standard-layout and trivial type. As a consequence, the type-trait std::is_pod is deprecated in C++20, and you are encouraged to use the two type-traits `std::is_trivial` and `std::is_standard_layout`.

- https://stackoverflow.com/questions/48225673/why-is-stdis-pod-deprecated-in-c20


## [Aggregate](https://en.cppreference.com/w/cpp/types/is_aggregate)
Since c++17

An aggregate can be seen as a composition of other types. All data members of an aggregate must be public. The interesting thing is that since C++17 aggregates can have public base classes as long as they are not virtual. These base classes do not need to be aggregates themself. If they are not, they are list-initialized.

- [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
Initializes an aggregate from an initializer list. It is a form of list-initialization. (since C++11)

  - For non-aggregate type, use () syntax.
  - For aggregate type, use {} syntax.

## Literal types
A literal type is one whose layout can be determined at compile time. The following are the literal types:

- void
- scalar types
- references
- Arrays of void, scalar types or references
- A class that has a trivial destructor, and one or more constexpr constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and not volatile.

## Summary 

Properties Overview
| Type  | memcopy-able| C compatible memory layout|
| ------|-----------|----- |
| Trivial type	|Yes|No|
| Standard-layout|Yes|Yes|
| Aggregate	|Yes|maybe|


## reference
- https://stackoverflow.com/questions/61329240/what-is-the-difference-between-trivial-and-non-trivial-objects

The point of triviality is that the type can be treated exactly like a fundamental type, in that objects of the type can be copied and moved with memcpy and constructed destructed without doing anything. Hence, triviality requires a type be essentially made only of fundamental types. This is what makes the copy, move, construction, and destruction operations relevant to the definition of trivial types. Other member functions don't play a role in triviality just as you can write void fn( int*, OtherArgs... ) without affecting whether or not an int is trivial, because you can think of member functions of T as essentially being free functions with the signature ReturnType member_function( T*, OtherArgs... ) that the compiler let's you call with the syntax a.member_function( other_args... ).

As for alignment, it simply isn't all that relevant because it's all taken care of for you. The compiler knows the alignment of the types it's working with, thanks to the strong static type system.



## Others

> Note that for `memcpy` to be legal to copy an object, the struct has to be trivially copyable. The important fact now is that the compiler knows that trivially copyable types can be trivially copied and will emit optimal code.

- New founding
If it is a trivial type, the assignment operator `=` and `memcpy` will generate the same assembly code.

Test it on https://godbolt.org/

```c++
#include <cstring>

struct A
{
    long long a0;
    long long a1;
    long long a2;
    long long a3;
    long long a4;
    // virtual void f() {}
};

void f( A& x, const A& y)
{
    x = y;
}

void g( A& x, const A& y)
{
    memcpy(&x, &y, sizeof(A));
}
```

## Codes
- modules/basis/serde
- modules/boostorg/boost_serialization.cpp

Save many memory.

