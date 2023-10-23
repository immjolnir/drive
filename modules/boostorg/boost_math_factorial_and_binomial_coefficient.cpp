#include <gtest/gtest.h>

#include <boost/math/special_functions.hpp>
#include <boost/math/special_functions/factorials.hpp>

// Factorials and Binomial Coefficients.
TEST(boost_math, factorial) {
    // Factorials
    /*
    EXPECT_EQ(6, boost::math::factorial<int>(3));    // 3!
    Not support int for /usr/include/boost/math/special_functions/factorials.hpp:32

    BOOST_STATIC_ASSERT(!boost::is_integral<T>::value);

   // factorial<unsigned int>(n) is not implemented
   // because it would overflow integral type T for too small n
   // to be useful. Use instead a floating-point type,
   // and convert to an unsigned type if essential, for example:
   // unsigned int nfac = static_cast<unsigned int>(factorial<double>(n));
   // See factorial documentation for more detail.
    */
    EXPECT_EQ(6, boost::math::factorial<double>(3));    // 3!
    EXPECT_EQ(24, boost::math::factorial<double>(4));   // 4!
    EXPECT_EQ(120, boost::math::factorial<double>(5));  // 5!
    // std::cout << "Factorial: " << boost::math::factorial<double>(n) << std::endl;

    // Caution: You must provide a return type template value, so this will not compile
    // unsigned int nfac = factorial(n); // could not deduce template argument for 'T'
    // You must provide an explicit floating-point (not integer) return type.
    // If you do provide an integer type, like this:
    // unsigned int uintfac = factorial<unsigned int>(n);
    // you will also get a compile error, for MSVC C2338.
    // If you really want an integer type, you can convert from double:
    unsigned int n = 3;
    unsigned int intfac = static_cast<unsigned int>(boost::math::factorial<double>(n));
    // this will be exact, until the result of the factorial overflows the integer type.
    EXPECT_EQ(6, intfac);  // 3!
}

// Rising and falling factorials
// https://mathworld.wolfram.com/RisingFactorial.html
// In mathematics, the falling factorial (sometimes called the descending factorial,[1] falling sequential product, or
// lower factorial) is defined as the polynomial: x_n = x(x-1)(x-2)...(x-n+1)
//
TEST(boost_math, falling_factorial) {
    int i = 2;
    double x = 8;

    EXPECT_EQ(56, boost::math::falling_factorial(x, i));  // 8*7
}

// The rising factorial (sometimes called the Pochhammer function, Pochhammer polynomial, ascending factorial,[1] rising
// sequential product, or upper factorial) is defined as x(x+1)(x+2)...(x+n-1)
//
TEST(boost_math, rising_factorial) {
    int i = 2;
    double x = 8;
    EXPECT_EQ(72, boost::math::rising_factorial(x, i));  // 8*9
}

/*
Binomial coefficients
https://www.statlect.com/glossary/binomial-coefficient

In mathematics, the binomial coefficients are the positive integers that occur as coefficients in the binomial theorem.
this coefficient can be computed by the multiplicative formula

|n|                n(n-1)(n-2)...2*1
|k| = -----------------------------------------
     [(n-k)(n-k-1)..2*1] * [k(k-1)(k-2)..*1]
which using factorial notation can be compactly expressed as
|n|        n!
|k| = --------------
        (n-k)! k!
*/
TEST(boost_math, binomial_coefficient) {
    unsigned n = 5;
    unsigned k = 2;
    /*
      5!              5x4x3x2x1               5x4x3x2x1        5 x 4
    ----- = ---------------------------- = --------------- = -------- = 10
      2!    (5-2)(5-2-1)(5-2-2) x (2x1)      3x2x1 x 2x1        2
    */
    // Cant support integral too.
    // /usr/include/boost/math/special_functions/binomial.hpp:23
    EXPECT_EQ(10, boost::math::binomial_coefficient<double>(n, k));
    // details

    /*
    A numerator is a dividend.
    A denominator is a divisor.
      a
    -----
      b

    a is the numerator (or dividend) and
    b is the denominator (or divisor).

    Since fractions are just shorthand for division, the denominator is the number by which the numerator gets divided.
    In a division problem, it is called the divisor .
    */
    auto numerator = boost::math::factorial<double>(n);
    // it equals to boost::math::rising_factorial<double>(1, 5 - 2)
    auto denominator = boost::math::factorial<double>(5 - 2) * boost::math::factorial<double>(k);
    EXPECT_EQ(120, numerator);
    EXPECT_EQ(12, denominator);

    auto fraction = numerator / denominator;
    EXPECT_EQ(10, fraction);
}

/* Implementation
template <class T, class Policy>
T binomial_coefficient(unsigned n, unsigned k, const Policy& pol) {
    BOOST_STATIC_ASSERT(!boost::is_integral<T>::value);
    BOOST_MATH_STD_USING
    static const char* function = "boost::math::binomial_coefficient<%1%>(unsigned, unsigned)";
    if (k > n)
        return policies::raise_domain_error<T>(
          function, "The binomial coefficient is undefined for k > n, but got k = %1%.", static_cast<T>(k), pol);
    T result;
    if ((k == 0) || (k == n)) return static_cast<T>(1);
    if ((k == 1) || (k == n - 1)) return static_cast<T>(n);

    if (n <= max_factorial<T>::value) {
        // Use fast table lookup:
        result = unchecked_factorial<T>(n);
        result /= unchecked_factorial<T>(n - k);
        result /= unchecked_factorial<T>(k);
    } else {
        // Use the beta function:
        if (k < n - k)
            result = k * beta(static_cast<T>(k), static_cast<T>(n - k + 1), pol);
        else
            result = (n - k) * beta(static_cast<T>(k + 1), static_cast<T>(n - k), pol);
        if (result == 0) return policies::raise_overflow_error<T>(function, 0, pol);
        result = 1 / result;
    }
    // convert to nearest integer:
    return ceil(result - 0.5f);
}
*/
