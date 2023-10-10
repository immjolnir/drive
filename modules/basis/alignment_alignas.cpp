#include <gtest/gtest.h>
#include <type_traits> // std::aligned_storage

struct Empty {};

// https://en.cppreference.com/w/cpp/language/alignof
struct alignas(64) Empty64 {};

// Note: `alignas(alignof(long double))` below can be simplified to simply
// `alignas(long double)` if desired.
struct alignas(alignof(long double)) Foo2 {
    // put your definition here
};

// alignas(alignof(void*)) struct Foo {
struct alignas(16) Bar {
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

TEST(alignment_alignas, alignof_vs_sizeof) {
    // Above case, the alignof value is the alignment requirement of the largest element in the structure.
    EXPECT_EQ(1, alignof(Empty));
    EXPECT_EQ(64, alignof(Empty64));
    EXPECT_EQ(16, alignof(Bar));
    EXPECT_EQ(16, alignof(Foo2));

    EXPECT_EQ(1, sizeof(Empty));
    EXPECT_EQ(64, sizeof(Empty64));  // why? auto padding?
    EXPECT_EQ(16, sizeof(Bar));
    EXPECT_EQ(16, sizeof(Foo2));
}