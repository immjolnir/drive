#include "IEEE_754.h"
#include "math_constants.h"

#include <gtest/gtest.h>
#include <type_traits>

namespace gtest_internal {
// Macros for comparing floating-point numbers.
//
//    * {ASSERT|EXPECT}_FLOAT_EQ(val1, val2):
//         Tests that two float values are almost equal.
//    * {ASSERT|EXPECT}_DOUBLE_EQ(val1, val2):
//         Tests that two double values are almost equal.
//    * {ASSERT|EXPECT}_NEAR(v1, v2, abs_error):
//         Tests that v1 and v2 are within the given distance to each other.
//
// Google Test uses ULP-based comparison to automatically pick a default
// error bound that is appropriate for the operands.  See the
// FloatingPoint template class in gtest-internal.h if you are
// interested in the implementation details.

#define EXPECT_FLOAT_EQ(val1, val2) \
    EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<float>, val1, val2)

#define EXPECT_DOUBLE_EQ(val1, val2) \
    EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<double>, val1, val2)

// Helper template function for comparing floating-points.
//
// Template parameter:
//
//   RawType: the raw floating-point type (either float or double)
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
/*
template <typename RawType>
AssertionResult CmpHelperFloatingPointEQ(const char* lhs_expression, const char* rhs_expression, RawType lhs_value,
                                         RawType rhs_value) {
    const FloatingPoint<RawType> lhs(lhs_value), rhs(rhs_value);

    if (lhs.AlmostEquals(rhs)) {
        return AssertionSuccess();
    }

    ::std::stringstream lhs_ss;
    lhs_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2) << lhs_value;

    ::std::stringstream rhs_ss;
    rhs_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2) << rhs_value;

    return EqFailure(lhs_expression, rhs_expression, StringStreamToString(&lhs_ss), StringStreamToString(&rhs_ss),
                     false);
}
*/
}  // namespace gtest_internal

namespace ieee_754 {

TEST(ieee_754, Float) {
    /*
    The most-significant bit being the leftmost, an IEEE floating-point looks like:
    .------.----------.----------.
    | sign | exponent | fraction |
    '------'----------'----------'
    |  1   |     8    |    23    |
    `------'----------'----------`
    */
    // Float f(kPiFloat);
    bool same_class = std::is_same_v<Float::Bits, std::uint32_t>;
    EXPECT_TRUE(same_class);

    EXPECT_EQ(32, Float::kBitCount);
    EXPECT_EQ(23, Float::kFractionBitCount);
    EXPECT_EQ(8, Float::kExponentBitCount);

    EXPECT_EQ(0x00'00'00'01, static_cast<std::uint32_t>(1));
    // static_cast<Bits>(1) << (kBitCount - 1);
    EXPECT_EQ(0x80'00'00'00, Float::kSignBitMask);

    EXPECT_EQ(0xFF'FF'FF'FF, ~static_cast<std::uint32_t>(0));
    // ~static_cast<std::uint32_t>(0) >> 24)
    EXPECT_EQ(0x00'7f'ff'ff, Float::kFractionBitMask);

    EXPECT_EQ(0x80'7f'ff'ff, Float::kSignBitMask | Float::kFractionBitMask);

    // ~(kSignBitMask | kFractionBitMask)
    EXPECT_EQ(0x7f'80'00'00, Float::kExponentBitMask);
}

TEST(ieee_754, Double) {
    /*
    The most-significant bit being the leftmost, an IEEE floating-point looks like:
    .------.----------.----------.
    | sign | exponent | fraction |
    '------'----------'----------'
    |  1   |     11    |    52   |
    '------'----------'----------'
    */
    // Float f(kPiFloat);
    bool same_class = std::is_same_v<Double::Bits, std::uint64_t>;
    EXPECT_TRUE(same_class);

    EXPECT_EQ(64, Double::kBitCount);
    EXPECT_EQ(11, Double::kExponentBitCount);
    EXPECT_EQ(52, Double::kFractionBitCount);

    EXPECT_EQ(0x00'00'00'00'00'00'00'01, static_cast<std::uint64_t>(1));
    // static_cast<Bits>(1) << (kBitCount - 1);
    EXPECT_EQ(0x80'00'00'00'00'00'00'00, Double::kSignBitMask);

    EXPECT_EQ(0xFF'FF'FF'FF'FF'FF'FF'FF, ~static_cast<std::uint64_t>(0));
    // ~static_cast<std::uint32_t>(0) >> 24)
    EXPECT_EQ(0x00'0f'ff'ff'ff'ff'ff'ff, Double::kFractionBitMask);

    EXPECT_EQ(0x80'0f'ff'ff'ff'ff'ff'ff, Double::kSignBitMask | Double::kFractionBitMask);
    // ~(kSignBitMask | kFractionBitMask)
    EXPECT_EQ(0x7f'f0'00'00'00'00'00'00, Double::kExponentBitMask);
}

TEST(ieee_754, floating_point_number_comparision) {
    Float fa(kPiFloat);
    EXPECT_EQ(1078530011, fa.bits());
    EXPECT_EQ(0, fa.sign_bit());
    EXPECT_EQ(1073741824, fa.exponent_bits());
    EXPECT_EQ(4788187, fa.fraction_bits());

    Float fb(-1 * kPiFloat);
    EXPECT_EQ(3226013659, fb.bits());
    EXPECT_EQ(0x80'00'00'00, fb.sign_bit());
    EXPECT_EQ(1073741824, fb.exponent_bits());
    EXPECT_EQ(4788187, fb.fraction_bits());
}

TEST(ieee_754, floating_point_number_comparision2) {
    float a = kPiFloat, b = kPiFloat;

    // EXPECT_FLOAT_EQ(a, b);

    Float fa(a), fb(b);
    std::cout << "Float::Bits=" << sizeof(Float::Bits) << std::endl;

    fa.PrintRawType();
    fa.PrintBitsType();

    EXPECT_TRUE(fa.AlmostEquals(fb));
};
}  // namespace ieee_754
