#include <gtest/gtest.h>

#include <cmath>  // std::log10
#include <numeric>

/**
 * Examples:
 * digits: modules/google_gtest/IEEE_754.h
 *
 * digits10:
     ::std::stringstream lhs_ss;
    lhs_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2) << lhs_value;
 *
*/
TEST(numeric, significant_decimal_digits_digits10) {
    /*
    https://en.cppreference.com/w/cpp/types/numeric_limits/digits10
    float	FLT_DIG (6 for IEEE float)
    double	DBL_DIG (15 for IEEE double)
    long double	LDBL_DIG (18 for 80-bit Intel long double; 33 for IEEE quadruple)
    */
    EXPECT_EQ(6, std::numeric_limits<float>::digits10);
    EXPECT_EQ(15, std::numeric_limits<double>::digits10);
    EXPECT_EQ(18, std::numeric_limits<long double>::digits10);
}

TEST(numeric, numeric_digits_vs_digits10_vs_max_digits10_int) {
    EXPECT_EQ(31, std::numeric_limits<int>::digits);
    EXPECT_EQ(9, std::numeric_limits<int>::digits10);

    std::cout << "log10(2)=" << std::log10(2) << std::endl;

    EXPECT_EQ(std::numeric_limits<int>::digits10, int(std::numeric_limits<int>::digits * log10(2)));
    EXPECT_EQ(0, std::numeric_limits<int>::max_digits10);  // why 0?
}

TEST(numeric, numeric_digits_vs_digits10_vs_max_digits10_float) {
    EXPECT_EQ(24, std::numeric_limits<float>::digits);
    EXPECT_EQ(6, std::numeric_limits<float>::digits10);
    EXPECT_EQ(9, std::numeric_limits<float>::max_digits10);
}

TEST(numeric, numeric_digits_vs_digits10_vs_max_digits10_double) {
    EXPECT_EQ(53, std::numeric_limits<double>::digits);
    EXPECT_EQ(15, std::numeric_limits<double>::digits10);
    EXPECT_EQ(17, std::numeric_limits<double>::max_digits10);
}