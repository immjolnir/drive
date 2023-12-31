#pragma once

#include <cstddef>  // size_t
#include <cstdint>  // std::int32_t
#include <numeric>

// https://en.cppreference.com/w/cpp/types/climits
#include <cfloat>  // FLT_MAX, DBL_MAX
#include <iostream>

namespace ieee_754 {

/**
 * https://en.wikipedia.org/wiki/IEEE_754
 *
 * Impl from /usr/include/gtest/internal/gtest-internal.h
 */

// This template class serves as a compile-time function from size to
// type.  It maps a size in bytes to a primitive type with that
// size. e.g.
//
//   TypeWithSize<4>::UInt
//
// is typedef-ed to be unsigned int (unsigned integer made up of 4
// bytes).
//
// Such functionality should belong to STL, but I cannot find it
// there.
//
// Google Test uses this class in the implementation of floating-point
// comparison.
//
// For now it only handles UInt (unsigned int) as that's all Google Test
// needs.  Other types can be easily added in the future if need
// arises.
template <size_t size>
class TypeWithSize {
  public:
    // This prevents the user from using TypeWithSize<N> with incorrect
    // values of N.
    using UInt = void;
};

// The specialization for size 4.
template <>
class TypeWithSize<4> {
  public:
    using Int = std::int32_t;
    using UInt = std::uint32_t;
};

// The specialization for size 8.
template <>
class TypeWithSize<8> {
  public:
    using Int = std::int64_t;
    using UInt = std::uint64_t;
};

// This template class represents an IEEE floating-point number
// (either single-precision or double-precision, depending on the
// template parameters).
//
// The purpose of this class is to do more sophisticated number
// comparison.  (Due to round-off error, etc, it's very unlikely that
// two floating-points will be equal exactly.  Hence a naive
// comparison by the == operation often doesn't work.)
//
// Format of IEEE floating-point:
//
//   The most-significant bit being the leftmost, an IEEE
//   floating-point looks like
//
//     sign_bit exponent_bits fraction_bits
//
//   Here, sign_bit is a single bit that designates the sign of the
//   number.
//
//   For float, there are 8 exponent bits and 23 fraction bits.
//
//   For double, there are 11 exponent bits and 52 fraction bits.
//
//   More details can be found at
//   http://en.wikipedia.org/wiki/IEEE_floating-point_standard.
//
// Template parameter:
//
//   RawType: the raw floating-point type (either float or double)
template <typename RawType>
class FloatingPoint {
  public:
    // Defines the unsigned integer type that has the same size as the
    // floating point number.
    typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

    // Constants.

    // # of bits in a number.
    static constexpr size_t kBitCount = 8 * sizeof(RawType);

    // # of fraction bits in a number.
    static constexpr size_t kFractionBitCount = std::numeric_limits<RawType>::digits - 1;

    // # of exponent bits in a number.
    static constexpr size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

    // The mask for the sign bit.
    static constexpr Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

    // The mask for the fraction bits.
    static constexpr Bits kFractionBitMask = ~static_cast<Bits>(0) >> (kExponentBitCount + 1);

    // The mask for the exponent bits.
    static constexpr Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

    // How many ULP's (Units in the Last Place) we want to tolerate when
    // comparing two numbers.  The larger the value, the more error we
    // allow.  A 0 value means that two numbers must be exactly the same
    // to be considered equal.
    //
    // The maximum error of a single floating-point operation is 0.5
    // units in the last place.  On Intel CPU's, all floating-point
    // calculations are done with 80-bit precision, while double has 64
    // bits.  Therefore, 4 should be enough for ordinary use.
    //
    // See the following article for more details on ULP:
    // http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    static constexpr uint32_t kMaxUlps = 4;

    // Constructs a FloatingPoint from a raw floating-point number.
    //
    // On an Intel CPU, passing a non-normalized NAN (Not a Number)
    // around may change its bits, although the new value is guaranteed
    // to be also a NAN.  Therefore, don't expect this constructor to
    // preserve the bits in x when x is a NAN.
    explicit FloatingPoint(const RawType& x) { u_.value_ = x; }

    // Static methods

    // Reinterprets a bit pattern as a floating-point number.
    //
    // This function is needed to test the AlmostEquals() method.
    static RawType ReinterpretBits(const Bits bits) {
        FloatingPoint fp(0);
        fp.u_.bits_ = bits;
        return fp.u_.value_;
    }

    // Returns the floating-point number that represent positive infinity.
    static RawType Infinity() { return ReinterpretBits(kExponentBitMask); }

    // Returns the maximum representable finite floating-point number.
    static RawType Max();

    // Non-static methods

    // Returns the bits that represents this number.
    const Bits& bits() const { return u_.bits_; }

    // Returns the exponent bits of this number.
    Bits exponent_bits() const { return kExponentBitMask & u_.bits_; }

    // Returns the fraction bits of this number.
    Bits fraction_bits() const { return kFractionBitMask & u_.bits_; }

    // Returns the sign bit of this number.
    Bits sign_bit() const { return kSignBitMask & u_.bits_; }

    // Returns true if and only if this is NAN (not a number).
    bool is_nan() const {
        // It's a NAN if the exponent bits are all ones and the fraction
        // bits are not entirely zeros.
        return (exponent_bits() == kExponentBitMask) && (fraction_bits() != 0);
    }

    // Returns true if and only if this number is at most kMaxUlps ULP's away
    // from rhs.  In particular, this function:
    //
    //   - returns false if either number is (or both are) NAN.
    //   - treats really large numbers as almost equal to infinity.
    //   - thinks +0.0 and -0.0 are 0 DLP's apart.
    bool AlmostEquals(const FloatingPoint& rhs) const {
        // The IEEE standard says that any comparison operation involving
        // a NAN must return false.
        if (is_nan() || rhs.is_nan()) return false;

        // Don't it need init bits_ before using it?
        return DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_) <= kMaxUlps;
    }

    void PrintRawType() { std::cout << "RawType value: " << u_.value_ << std::endl; }

    void PrintBitsType() { std::cout << "Bits _bits: " << u_.bits_ << std::endl; }

  private:
    // The data type used to store the actual floating-point number.
    union FloatingPointUnion {
        RawType value_;  // The raw floating-point number.
        Bits bits_;      // The bits that represent the number.
    };

    // Converts an integer from the sign-and-magnitude representation to
    // the biased representation.  More precisely, let N be 2 to the
    // power of (kBitCount - 1), an integer x is represented by the
    // unsigned number x + N.
    //
    // For instance,
    //
    //   -N + 1 (the most negative number representable using
    //          sign-and-magnitude) is represented by 1;
    //   0      is represented by N; and
    //   N - 1  (the biggest number representable using
    //          sign-and-magnitude) is represented by 2N - 1.
    //
    // Read http://en.wikipedia.org/wiki/Signed_number_representations
    // for more details on signed number representations.
    static Bits SignAndMagnitudeToBiased(const Bits& sam) {
        if (kSignBitMask & sam) {
            // sam represents a negative number.
            return ~sam + 1;
        } else {
            // sam represents a positive number.
            return kSignBitMask | sam;
        }
    }

    // Given two numbers in the sign-and-magnitude representation,
    // returns the distance between them as an unsigned number.
    static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits& sam1, const Bits& sam2) {
        const Bits biased1 = SignAndMagnitudeToBiased(sam1);
        const Bits biased2 = SignAndMagnitudeToBiased(sam2);
        return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
    }

    FloatingPointUnion u_;
};

// We cannot use std::numeric_limits<T>::max() as it clashes with the max()
// macro defined by <windows.h>.
template <>
inline float FloatingPoint<float>::Max() {
    return FLT_MAX;
}

template <>
inline double FloatingPoint<double>::Max() {
    return DBL_MAX;
}

// Typedefs the instances of the FloatingPoint template class that we
// care to use.
typedef FloatingPoint<float> Float;
typedef FloatingPoint<double> Double;
}  // namespace ieee_754
