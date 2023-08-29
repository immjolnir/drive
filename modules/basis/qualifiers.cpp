// https://stackoverflow.com/questions/29883327/is-it-safe-to-remove-const-via-const-cast-and-invoke-a-non-const-function-that-d
#include <gtest/gtest.h>
#include <iostream>

/*
https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers

Specifier and qualifier are defined in the C++ standard.
Qualifier is just an integral part of a specifier. For example, type specifier in a declaration can include
cv-qualifiers.

1. static_cast:
- https://stackoverflow.com/questions/2253168/dynamic-cast-and-static-cast-in-c
  - cast compile time
  - Only perform the cast if the types are related

2. dynamic_cast conversion
- https://en.cppreference.com/w/cpp/language/dynamic_cast

  Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.

  If the cast is successful, dynamic_cast returns a value of type target-type. If the cast fails and target-type is a
pointer type, it returns a null pointer of that type.

  > The dynamic_cast keyword casts a datum from one pointer or reference type to another, performing a runtime check to
ensure the validity of the cast

  - cast is executed at runtime, not compile time

*/

TEST(qualifiers, remove_const_is_unsafe) {
    // We know that both string::c_str or string::data functions returns const char*. To get a non-const version, we can
    // use the const_cast operator, which removes the const attribute from a class. This works in constant time as no
    // copying is involved.

    const std::string line = "Removing the const qualifier is not safe";
    char* s = const_cast<char*>(line.c_str());
    s[0] = '0';
    s[2] = '0';
    EXPECT_EQ('0', line[0]);
    EXPECT_EQ('e', line[1]);
    EXPECT_EQ('0', line[2]);
}

struct Base {
    virtual ~Base() {}
};

struct Derived : Base {
    virtual void name() {}
};

class X {};

TEST(qualifiers, static_cast_show) {
    Base* b1 = new Base;
    EXPECT_TRUE(static_cast<Derived*>(b1) != nullptr);

    Base* b2 = new Derived;
    EXPECT_TRUE(static_cast<Derived*>(b2) != nullptr);

    Derived* d3 = static_cast<Derived*>(b2);
    // It's safe to convert to Base type.
    EXPECT_TRUE(static_cast<Base*>(d3) != nullptr);

    // irrelate-type, failed the compilation
    // error: invalid 'static_cast' from type 'Derived*' to type 'X*'
    // EXPECT_TRUE(static_cast<X*>(d3) != nullptr);

    delete b1;
    delete b2;
}

TEST(qualifiers, dynamic_cast_show) {
    Base* b1 = new Base;
    EXPECT_FALSE(dynamic_cast<Derived*>(b1) != nullptr);

    Base* b2 = new Derived;
    EXPECT_TRUE(dynamic_cast<Derived*>(b2) != nullptr);
    if (Derived* d = dynamic_cast<Derived*>(b2); d != nullptr) {
        std::cout << "downcast from b2 to d successful\n";
        d->name();  // safe to call
    }

    Derived* d3 = dynamic_cast<Derived*>(b2);
    // It's safe to convert to Base type.
    EXPECT_TRUE(dynamic_cast<Base*>(d3) != nullptr);

    // irrelate-type but success to compile
    EXPECT_FALSE(dynamic_cast<X*>(d3) != nullptr);

    delete b1;
    delete b2;
    // delete d3;  // Segmentation fault (core dumped)
}