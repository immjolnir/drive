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

TEST(FloatingPointNumberHelper, API_StringStreamToString) {
    std::stringstream ss;
    ss << "abc" << '\0' << "def";

    std::string str = ss.str();
    EXPECT_EQ(7, str.length());

    const char* const start = str.c_str();
    const char* const end = start + str.length();
    const size_t size = static_cast<size_t>(end - start);
    EXPECT_EQ(7, size);

    std::string result;
    result.reserve(2 * size);
    EXPECT_EQ(0, result.length());
    EXPECT_EQ(2 * size + 1, result.capacity());  // Why add 1??

    bool found_null = false;
    for (const char* ch = start; ch != end; ++ch) {
        if (*ch == '\0') {
            found_null = true;
            std::cout << "found the NUL(\\0)" << std::endl;
            result += "\\0";  // Replaces NUL with "\\0";
        } else {
            result += *ch;
        }
    }
    EXPECT_TRUE(found_null);
    EXPECT_EQ(8, result.length());
    EXPECT_EQ(15, result.capacity());
}