#include "floating_point_number_helper.h"

#include <gtest/gtest.h>

// Internal macro for implementing {EXPECT|ASSERT}_PRED_FORMAT2.
// Don't use this in your code.
#define PRED_FORMAT2_(pred_format, v1, v2) pred_format(#v1, #v2, v1, v2)

// Binary predicate assertion macros.
#define PRED_FORMAT2(pred_format, v1, v2) PRED_FORMAT2_(pred_format, v1, v2)

#define DOUBLE_EQ(val1, val2) PRED_FORMAT2(mytesting::CmpHelperFloatingPointEQ<double>, val1, val2)

TEST(FloatingPointNumberHelper, equal) {
    auto res = DOUBLE_EQ(6.2160608428610731e-05, 6.2160608428610731e-05);
    if (const mytesting::AssertionResult gtest_ar = (res))
        std::cout << "Success" << std::endl;
    else
        std::cout << "Failure message=" << gtest_ar.failure_message() << std::endl;
}

TEST(FloatingPointNumberHelper, not_equal) {
    double expect = 1.0f;
    double actual = 2.0f;
    auto res = DOUBLE_EQ(expect, actual);
    if (const mytesting::AssertionResult gtest_ar = (res))
        std::cout << "Success" << std::endl;
    else
        std::cout << "Failure message=" << gtest_ar.failure_message() << std::endl;
}