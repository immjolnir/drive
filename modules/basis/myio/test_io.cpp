#include "io.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace my;

TEST(myio, example) {
    std::cout << "tt" << std::endl;
    if (mout._buf != nullptr) {
        std::cout << "not null" << std::endl;
    }

    std::cout << __ioinit._S_refcount << std::endl;
    EXPECT_EQ(2, __ioinit._S_refcount);

    mout.add("Hello IO!");

    std::cout << mout.to_string() << std::endl;
}