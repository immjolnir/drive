#include <gtest/gtest.h>

#include "global.h"

/*
// Expand it as shown below

extern int i;
static int j;

static int k = i;

// Is the `static int k = i;` safe?
// The `static int k=i;` is resolved at the runtime, while the the `extern int i;` is resolved by the linker at the
linking stage. So yes, it's safe.
*/

TEST(extern_vs_static, whats_the_order) {
    EXPECT_EQ(1, i);
    EXPECT_EQ(0, j);
    EXPECT_EQ(1, k);
}