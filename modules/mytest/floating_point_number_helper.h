#pragma once

#include <ctype.h>
#include <cfloat>   // limits of floating-point types
#include <iomanip>  // for setprecision
#include <limits>   // for numeric_limits
#include <memory>
#include <ostream>
#include <sstream>
#include <type_traits>

namespace mytesting
{

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
        typedef void UInt;
    };

    // The specialization for size 4.
    template <>
    class TypeWithSize<4> {
      public:
        // unsigned int has size 4 in both gcc and MSVC.
        //
        // As base/basictypes.h doesn't compile on Windows, we cannot use
        // uint32, uint64, and etc here.
        typedef int Int;
        typedef unsigned int UInt;
    };

    // The specialization for size 8.
    template <>
    class TypeWithSize<8> {
      public:
#if GTEST_OS_WINDOWS
        typedef __int64 Int;
        typedef unsigned __int64 UInt;
#else
        typedef long long Int;            // NOLINT
        typedef unsigned long long UInt;  // NOLINT
#endif  // GTEST_OS_WINDOWS
    };

    // Integer types of known sizes.
    typedef TypeWithSize<4>::Int Int32;
    typedef TypeWithSize<4>::UInt UInt32;
    typedef TypeWithSize<8>::Int Int64;
    typedef TypeWithSize<8>::UInt UInt64;
    typedef TypeWithSize<8>::Int TimeInMillis;  // Represents time in milliseconds.

    // Source from gtest-internal.h
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
        static const size_t kBitCount = 8 * sizeof(RawType);

        // # of fraction bits in a number.
        static const size_t kFractionBitCount = std::numeric_limits<RawType>::digits - 1;

        // # of exponent bits in a number.
        static const size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

        // The mask for the sign bit.
        static const Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

        // The mask for the fraction bits.
        static const Bits kFractionBitMask = ~static_cast<Bits>(0) >> (kExponentBitCount + 1);

        // The mask for the exponent bits.
        static const Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

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
        static const uint32_t kMaxUlps = 4;

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

            return DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_) <= kMaxUlps;
        }

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

    // The Message class works like an ostream repeater.
    //
    // Typical usage:
    //
    //   1. You stream a bunch of values to a Message object.
    //      It will remember the text in a stringstream.
    //   2. Then you stream the Message object to an ostream.
    //      This causes the text in the Message to be streamed
    //      to the ostream.
    //
    // For example;
    //
    //   testing::Message foo;
    //   foo << 1 << " != " << 2;
    //   std::cout << foo;
    //
    // will print "1 != 2".
    //
    // Message is not intended to be inherited from.  In particular, its
    // destructor is not virtual.
    //
    // Note that stringstream behaves differently in gcc and in MSVC.  You
    // can stream a NULL char pointer to it in the former, but not in the
    // latter (it causes an access violation if you do).  The Message
    // class hides this difference by treating a NULL char pointer as
    // "(null)".
    class Message {
      private:
        // The type of basic IO manipulators (endl, ends, and flush) for
        // narrow streams.
        typedef std::ostream& (*BasicNarrowIoManip)(std::ostream&);

      public:
        // Constructs an empty Message.
        Message();

        // Copy constructor.
        Message(const Message& msg) : ss_(new ::std::stringstream) {  // NOLINT
            *ss_ << msg.GetString();
        }

        // Constructs a Message from a C-string.
        explicit Message(const char* str) : ss_(new ::std::stringstream) { *ss_ << str; }

        // Streams a non-pointer value to this object.
        template <typename T>
        inline Message& operator<<(const T& val) {
            // Some libraries overload << for STL containers.  These
            // overloads are defined in the global namespace instead of ::std.
            //
            // C++'s symbol lookup rule (i.e. Koenig lookup) says that these
            // overloads are visible in either the std namespace or the global
            // namespace, but not other namespaces, including the testing
            // namespace which Google Test's Message class is in.
            //
            // To allow STL containers (and other types that has a << operator
            // defined in the global namespace) to be used in Google Test
            // assertions, testing::Message must access the custom << operator
            // from the global namespace.  With this using declaration,
            // overloads of << defined in the global namespace and those
            // visible via Koenig lookup are both exposed in this function.
            using std::operator<<;
            *ss_ << val;
            return *this;
        }

        // Streams a pointer value to this object.
        //
        // This function is an overload of the previous one.  When you
        // stream a pointer to a Message, this definition will be used as it
        // is more specialized.  (The C++ Standard, section
        // [temp.func.order].)  If you stream a non-pointer, then the
        // previous definition will be used.
        //
        // The reason for this overload is that streaming a NULL pointer to
        // ostream is undefined behavior.  Depending on the compiler, you
        // may get "0", "(nil)", "(null)", or an access violation.  To
        // ensure consistent result across compilers, we always treat NULL
        // as "(null)".
        template <typename T>
        inline Message& operator<<(T* const& pointer) {  // NOLINT
            if (pointer == nullptr) {
                *ss_ << "(null)";
            } else {
                *ss_ << pointer;
            }
            return *this;
        }

        // Since the basic IO manipulators are overloaded for both narrow
        // and wide streams, we have to provide this specialized definition
        // of operator <<, even though its body is the same as the
        // templatized version above.  Without this definition, streaming
        // endl or other basic IO manipulators to Message will confuse the
        // compiler.
        Message& operator<<(BasicNarrowIoManip val) {
            *ss_ << val;
            return *this;
        }

        // Instead of 1/0, we want to see true/false for bool values.
        Message& operator<<(bool b) { return *this << (b ? "true" : "false"); }

        // These two overloads allow streaming a wide C string to a Message
        // using the UTF-8 encoding.
        Message& operator<<(const wchar_t* wide_c_str);
        Message& operator<<(wchar_t* wide_c_str);

#if GTEST_HAS_STD_WSTRING
        // Converts the given wide string to a narrow string using the UTF-8
        // encoding, and streams the result to this Message object.
        Message& operator<<(const ::std::wstring& wstr);
#endif  // GTEST_HAS_STD_WSTRING

        // Gets the text streamed to this object so far as an std::string.
        // Each '\0' character in the buffer is replaced with "\\0".
        //
        // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
        std::string GetString() const;

      private:
        // We'll hold the text streamed to this object here.
        const std::unique_ptr< ::std::stringstream> ss_;

        // We declare (but don't implement) this to prevent the compiler
        // from implementing the assignment operator.
        void operator=(const Message&);
    };

    // Streams a Message to an ostream.
    inline std::ostream& operator<<(std::ostream& os, const Message& sb) { return os << sb.GetString(); }

    // A class for indicating whether an assertion was successful.  When
    // the assertion wasn't successful, the AssertionResult object
    // remembers a non-empty message that describes how it failed.
    //
    // To create an instance of this class, use one of the factory functions
    // (AssertionSuccess() and AssertionFailure()).
    //
    // This class is useful for two purposes:
    //   1. Defining predicate functions to be used with Boolean test assertions
    //      EXPECT_TRUE/EXPECT_FALSE and their ASSERT_ counterparts
    //   2. Defining predicate-format functions to be
    //      used with predicate assertions (ASSERT_PRED_FORMAT*, etc).
    //
    // For example, if you define IsEven predicate:
    //
    //   testing::AssertionResult IsEven(int n) {
    //     if ((n % 2) == 0)
    //       return testing::AssertionSuccess();
    //     else
    //       return testing::AssertionFailure() << n << " is odd";
    //   }
    //
    // Then the failed expectation EXPECT_TRUE(IsEven(Fib(5)))
    // will print the message
    //
    //   Value of: IsEven(Fib(5))
    //     Actual: false (5 is odd)
    //   Expected: true
    //
    // instead of a more opaque
    //
    //   Value of: IsEven(Fib(5))
    //     Actual: false
    //   Expected: true
    //
    // in case IsEven is a simple Boolean predicate.
    //
    // If you expect your predicate to be reused and want to support informative
    // messages in EXPECT_FALSE and ASSERT_FALSE (negative assertions show up
    // about half as often as positive ones in our tests), supply messages for
    // both success and failure cases:
    //
    //   testing::AssertionResult IsEven(int n) {
    //     if ((n % 2) == 0)
    //       return testing::AssertionSuccess() << n << " is even";
    //     else
    //       return testing::AssertionFailure() << n << " is odd";
    //   }
    //
    // Then a statement EXPECT_FALSE(IsEven(Fib(6))) will print
    //
    //   Value of: IsEven(Fib(6))
    //     Actual: true (8 is even)
    //   Expected: false
    //
    // NB: Predicates that support negative Boolean assertions have reduced
    // performance in positive ones so be careful not to use them in tests
    // that have lots (tens of thousands) of positive Boolean assertions.
    //
    // To use this class with EXPECT_PRED_FORMAT assertions such as:
    //
    //   // Verifies that Foo() returns an even number.
    //   EXPECT_PRED_FORMAT1(IsEven, Foo());
    //
    // you need to define:
    //
    //   testing::AssertionResult IsEven(const char* expr, int n) {
    //     if ((n % 2) == 0)
    //       return testing::AssertionSuccess();
    //     else
    //       return testing::AssertionFailure()
    //         << "Expected: " << expr << " is even\n  Actual: it's " << n;
    //   }
    //
    // If Foo() returns 5, you will see the following message:
    //
    //   Expected: Foo() is even
    //     Actual: it's 5
    //
    class AssertionResult {
      public:
        // Copy constructor.
        // Used in EXPECT_TRUE/FALSE(assertion_result).
        AssertionResult(const AssertionResult& other);

// C4800 is a level 3 warning in Visual Studio 2015 and earlier.
// This warning is not emitted in Visual Studio 2017.
// This warning is off by default starting in Visual Studio 2019 but can be
// enabled with command-line options.
#if defined(_MSC_VER) && (_MSC_VER < 1910 || _MSC_VER >= 1920)
        GTEST_DISABLE_MSC_WARNINGS_PUSH_(4800 /* forcing value to bool */)
#endif

        // Used in the EXPECT_TRUE/FALSE(bool_expression).
        //
        // T must be contextually convertible to bool.
        //
        // The second parameter prevents this overload from being considered if
        // the argument is implicitly convertible to AssertionResult. In that case
        // we want AssertionResult's copy constructor to be used.
        template <typename T>
        explicit AssertionResult(const T& success,
                                 typename std::enable_if<!std::is_convertible<T, AssertionResult>::value>::type*
                                 /*enabler*/
                                 = nullptr)
          : success_(success) {}

#if defined(_MSC_VER) && (_MSC_VER < 1910 || _MSC_VER >= 1920)
        GTEST_DISABLE_MSC_WARNINGS_POP_()
#endif

        // Assignment operator.
        AssertionResult& operator=(AssertionResult other) {
            swap(other);
            return *this;
        }

        // Returns true if and only if the assertion succeeded.
        operator bool() const { return success_; }  // NOLINT

        // Returns the assertion's negation. Used with EXPECT/ASSERT_FALSE.
        AssertionResult operator!() const;

        // Returns the text streamed into this AssertionResult. Test assertions
        // use it when they fail (i.e., the predicate's outcome doesn't match the
        // assertion's expectation). When nothing has been streamed into the
        // object, returns an empty string.
        const char* message() const { return message_.get() != nullptr ? message_->c_str() : ""; }

        // Deprecated; please use message() instead.
        const char* failure_message() const { return message(); }

        // Streams a custom failure message into this object.
        template <typename T>
        AssertionResult& operator<<(const T& value) {
            AppendMessage(Message() << value);
            return *this;
        }

        // Allows streaming basic output manipulators such as endl or flush into
        // this object.
        AssertionResult& operator<<(::std::ostream& (*basic_manipulator)(::std::ostream& stream)) {
            AppendMessage(Message() << basic_manipulator);
            return *this;
        }

      private:
        // Appends the contents of message to message_.
        void AppendMessage(const Message& a_message) {
            if (message_.get() == nullptr) message_.reset(new ::std::string);
            message_->append(a_message.GetString().c_str());
        }

        // Swap the contents of this AssertionResult with other.
        void swap(AssertionResult& other);

        // Stores result of the assertion predicate.
        bool success_;
        // Stores the message describing the condition in case the expectation
        // construct is not satisfied with the predicate's outcome.
        // Referenced via a pointer to avoid taking too much stack frame space
        // with test assertions.
        std::unique_ptr< ::std::string> message_;
    };

    // Makes a successful assertion result.
    AssertionResult AssertionSuccess();

    // Makes a failed assertion result.
    AssertionResult AssertionFailure();

    // Makes a failed assertion result with the given failure message.
    // Deprecated; use AssertionFailure() << msg.
    AssertionResult AssertionFailure(const Message& msg);

    // Gets the content of the stringstream's buffer as an std::string.  Each '\0'
    // character in the buffer is replaced with "\\0".
    std::string StringStreamToString(::std::stringstream* stream);

    // Constructs and returns the message for an equality assertion
    // (e.g. ASSERT_EQ, EXPECT_STREQ, etc) failure.
    //
    // The first four parameters are the expressions used in the assertion
    // and their values, as strings.  For example, for ASSERT_EQ(foo, bar)
    // where foo is 5 and bar is 6, we have:
    //
    //   expected_expression: "foo"
    //   actual_expression:   "bar"
    //   expected_value:      "5"
    //   actual_value:        "6"
    //
    // The ignoring_case parameter is true if and only if the assertion is a
    // *_STRCASEEQ*.  When it's true, the string " (ignoring case)" will
    // be inserted into the message.
    AssertionResult EqFailure(const char* expected_expression, const char* actual_expression,
                              const std::string& expected_value, const std::string& actual_value, bool ignoring_case);

    // Helper template function for comparing floating-points.
    //
    // Template parameter:
    //
    //   RawType: the raw floating-point type (either float or double)
    //
    // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
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

}  // namespace mytesting

/*
#elif __GNUC__ >= 4 || defined(__clang__)
# define GTEST_API_ __attribute__((visibility ("default")))
#endif  // _MSC_VER
*/