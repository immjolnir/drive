#include <gtest/gtest.h>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

TEST(new_operator, quick_example) {
    // call to 'SimpleChar::SimpleChar(char)'
    SimpleChar* c0 = new SimpleChar('a');
    SimpleChar* c1 = new SimpleChar('b');
    EXPECT_EQ('a', c0->c);
    EXPECT_EQ('b', c1->c);
    delete c0;
    delete c1;
}

TEST(replacement_new_operator, quick_example) {
    char slab[10] = {'\0'};

    // call to 'SimpleChar::SimpleChar(char)' too
    SimpleChar* c0 = new (slab) SimpleChar('a');
    // clang-format off
    /*
    warning: placement new constructing an object of type 'SimpleChar' and size '8' in a region of type 'char [10]' and size '2' [-Wplacement-new=]
    22 |     SimpleChar* c1 = new (slab + 8) SimpleChar('b');
      |                           ~~~~~^~~
    10: note: at offset 8 from 'slab' declared here
   18 |     char slab[10] = {'\0'};
      |          ^~~~
    */
    // clang-format on
    SimpleChar* c1 = new (slab + 8) SimpleChar('b');
    EXPECT_EQ('a', c0->c);
    EXPECT_EQ('b', c1->c);

    // Unsafe but still safe
    // clang-format off
    /*
    warning: placement new constructing an object of type 'SimpleChar' and size '8' in a region of type 'char [10]' and size '0' [-Wplacement-new=]
    38 |     SimpleChar* c2 = new (slab + 80) SimpleChar('d');
    */
    // clang-format on
    SimpleChar* c2 = new (slab + 80) SimpleChar('d');  // But how to detect it?
    EXPECT_EQ('d', c2->c);
}