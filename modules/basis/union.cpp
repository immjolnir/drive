#include <gtest/gtest.h>

/*
A union can have member functions (including constructors and destructors), but not virtual functions.

A union cannot have base classes and cannot be used as a base class.

A union cannot have non-static data members of reference types.

Just like in struct declaration, the default member access in a union is public.
*/

// https://www.geeksforgeeks.org/structures-unions-and-enumerations-in-cpp/
//
// The size of the union is the size of its largest field (or larger if alignment so requires, for example on a SPARC
// machine a union contains a double and a char [17] so its size is likely to be 24 because it needs 64-bit alignment).
// Unions cannot have a destructor.
//

union SPARC {
    double d;
    char arr[17];
};

static_assert(sizeof(SPARC) == 24, "Its size is 24B");

union Foo {
    double d;
    char c;
};

union alignas(64) Bar {
    double d;
    char c;
};

TEST(union, size) {
    EXPECT_EQ(24, sizeof(SPARC));

    // The size of the union is the size of its largest field.
    EXPECT_TRUE(sizeof(Foo) == sizeof(double));
    EXPECT_EQ(8, sizeof(double));
    EXPECT_EQ(8, sizeof(Foo));

    // The size of the union is larger than its largest field for alignment.
    EXPECT_EQ(64, sizeof(Bar));
}

// https://en.cppreference.com/w/cpp/language/union
// The union is at least as big as necessary to hold its largest data member, but is usually not larger.
// The other data members are intended to be allocated in the same bytes as part of that largest member.
// The details of that allocation are implementation-defined,
// but all non-static data members will have the same address (since C++14).
// It is undefined behavior to read from the member of the union that wasn't most recently written.
TEST(union, which_active_field) {
    Foo foo;
    foo.d = 1.0;  // d: is now the active member
    EXPECT_EQ(1.0, foo.d);

    foo.c = 'a';  // c is now the active member
    EXPECT_EQ('a', foo.c);
    // UB(undefined behavior)
    // It shouldnot work any more.
    // EXPECT_EQ(1.0, foo.d);
}