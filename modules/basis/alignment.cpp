#include <gtest/gtest.h>

#include <type_traits>

// https://en.cppreference.com/w/cpp/language/object#Alignment

// objects of type S can be allocated at any address
// because both S.a and S.b can be allocated at any address
struct S {
    char a;  // size: 1, alignment: 1
    char b;  // size: 1, alignment: 1
};           // size: 2, alignment: 1

// objects of type X must be allocated at 4-byte boundaries
// because X.n must be allocated at 4-byte boundaries
// because int's alignment requirement is (usually) 4
struct X {
    int n;   // size: 4, alignment: 4
    char c;  // size: 1, alignment: 1
    // three bytes of padding bits
};  // size: 8, alignment: 4

TEST(alignment, alignment) {
    EXPECT_EQ(1, alignof(S));
    EXPECT_EQ(2, sizeof(S));

    EXPECT_EQ(4, alignof(X));
    EXPECT_EQ(8, sizeof(X));
}

// https://www.geeksforgeeks.org/alignof-operator-in-c/
struct Geeks {
    int i;
    char c;
    short s;
    float f;
    /*
    .-------.
    | i     |
    .-------.
    | c | s |
    .-------.
    | f     |
    '-------'
    */
};

struct Empty {};

TEST(alignment, quick_example) {
    // The alignof value is the same as the value for sizeof for basic types
    EXPECT_EQ(1, alignof(char));
    EXPECT_EQ(4, alignof(float));
    EXPECT_EQ(8, alignof(double));
    EXPECT_EQ(16, alignof(long double));

    EXPECT_EQ(8, alignof(char*));
    EXPECT_EQ(8, alignof(int*));

    EXPECT_EQ(4, alignof(Geeks));
    EXPECT_EQ(1, alignof(Empty));
}

TEST(alignment, alignof_vs_sizeof) {
    // Above case, the alignof value is the alignment requirement of the largest element in the structure.
    EXPECT_EQ(4, alignof(Geeks));
    EXPECT_EQ(12, sizeof(Geeks));

    EXPECT_EQ(1, alignof(Empty));
    EXPECT_EQ(1, sizeof(Empty));
}

// https://en.cppreference.com/w/cpp/types/alignment_of
// Possible implementation
// template<class T>
// struct alignment_of : std::integral_constant<std::size_t, alignof(T)> {};
TEST(alignment, alignment_of) {
    EXPECT_EQ(8, alignof(double));

    EXPECT_EQ(8, std::alignment_of<double>());
    EXPECT_EQ(8, std::alignment_of<double>::value);
}