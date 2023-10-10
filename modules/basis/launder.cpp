#include <gtest/gtest.h>

// https://en.cppreference.com/w/cpp/utility/launder
// std::launder has no effect on its argument. Its return value must be used to access the object. Thus, it's always an
// error to discard the return value.
//
#include <new>  // std::launder

struct X {
    const int n;
};

union U {
    X x;
    float f;
};

TEST(new_api, paper_example) {
    U u{{1}};

    EXPECT_EQ(1, u.x.n);

    // Because X is trivial, we need not destroy the old object before creating a new one in its place, so this is
    // perfectly legal code. The new object will have its n member be 2.
    X* p = new (&u.x) X{2};
    // It's wrong, because the compiler is allowed to assume that a truly const variable (not merely a const&, but an
    //  object variable declared const) will never change. But we just changed it.
    EXPECT_EQ(2, u.x.n);
    EXPECT_EQ(2, p->n);
}

TEST(new_api, paper_example_fix) {
    U u{{1}};

    EXPECT_EQ(1, u.x.n);

    // error: invalid conversion from 'const int*' to 'int*'
    // int* p = std::launder(&u.x.n);

    //  error: invalid conversion from 'const void*' to 'void*'
    // int* p = new (std::launder(&u.x.n)) int{2};

    EXPECT_EQ(1, *std::launder(&u.x.n));
}
