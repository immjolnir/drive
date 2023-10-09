#include "global_variables.h"

#include <gtest/gtest.h>
#include <iostream>

TEST(variables, general) {
    EXPECT_EQ(2, i);
    EXPECT_EQ(0, j);  // why not 2? Variables defined outside any function body are initialized to zero.
    EXPECT_EQ(1, k);  // why is 1?  `extern int k = 1;` is also a definition.
}

/*
$ nm -g test_global_variables

0000000000059040 B i
0000000000059178 B j
0000000000059010 D k
*/