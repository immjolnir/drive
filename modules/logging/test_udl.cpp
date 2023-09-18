#include "udl.h"

#include <gtest/gtest.h>

using namespace std;

TEST(udl, user_defined_logging) {
    auto res = log("a=%%, b=%%, c=%%", 1, true, 0.1f);
    EXPECT_EQ("a=1, b=True, c=0.1f", res);
}
