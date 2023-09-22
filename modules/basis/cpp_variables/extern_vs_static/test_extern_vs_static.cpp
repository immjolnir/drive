#include <gtest/gtest.h>

#include "chararray.h"
#include "chararray_ptr.h"
#include "global.h"

TEST(CharArray, size) {
    EXPECT_EQ(10, sizeof(CharArray));
    EXPECT_EQ(8, sizeof(CharArrayPtr));
    EXPECT_EQ(8, sizeof(void*));
}

TEST(extern_vs_static, whats_the_order) {
    EXPECT_EQ(1, i);
    EXPECT_EQ(0, j);
    EXPECT_EQ(1, k);
}