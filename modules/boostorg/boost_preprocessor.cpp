#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>             // BOOST_PP_CAT
#include <boost/preprocessor/config/config.hpp>   // BOOST_PP_CONFIG_FLAGS
#include <boost/preprocessor/repeat_from_to.hpp>  // BOOST_PP_REPEAT_FROM_TO
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include <string>

#include <gtest/gtest.h>

/*
 * Source
 *
# define BOOST_PP_CONFIG_STRICT() 0x0001
# define BOOST_PP_CONFIG_MSVC() 0x0004
# define BOOST_PP_CONFIG_MWCC() 0x0008
...
# ifndef BOOST_PP_CONFIG_FLAGS
#    if defined(__GCCXML__) || defined(__WAVE__) || defined(__MWERKS__) && __MWERKS__ >= 0x3200
#        define BOOST_PP_CONFIG_FLAGS() (BOOST_PP_CONFIG_STRICT())
...
#    else
#        define BOOST_PP_CONFIG_FLAGS() (BOOST_PP_CONFIG_STRICT())
#    endif
# endif
...
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_STRICT()
# define BOOST_PP_IS_STANDARD() 1
# else
# define BOOST_PP_IS_STANDARD() 0
# endif
*
 * */
TEST(preprocessor, config) {
    EXPECT_EQ(0x0001, BOOST_PP_CONFIG_FLAGS());
    EXPECT_EQ(0x0004, BOOST_PP_CONFIG_MSVC());
    EXPECT_EQ(0x0004, ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC());

    EXPECT_EQ(0x0008, BOOST_PP_CONFIG_MWCC());
    EXPECT_EQ(0x0008, ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC());
}

/*
Usage: BOOST_PP_CAT(a, b)
Arguments
* a: The left operand of the concatenation.
* b: The right operand of the concatenation.
*
Remarks: The preprocessor token-pasting operator (##) prevents arguments from expanding.  This macro allows its
arguments to expand before concatenation. Concatenation must not result in an invocation of a macro that uses
BOOST_PP_CAT.  If that happens, BOOST_PP_CAT will not expand the second time.

Source code:
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)
# else
#    define BOOST_PP_CAT(a, b) BOOST_PP_CAT_OO((a, b))
#    define BOOST_PP_CAT_OO(par) BOOST_PP_CAT_I ## par
# endif
#
# if (~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1700)
#    define BOOST_PP_CAT_I(a, b) a ## b
# else
#    define BOOST_PP_CAT_I(a, b) BOOST_PP_CAT_II(~, a ## b)
#    define BOOST_PP_CAT_II(p, res) res
# endif

 * */

#define CAT(a, b) a##b

TEST(preprocessor, concatenates) {
#pragma GCC diagnostic ignored "-Wunused-variable"
    int x = 10;
    unsigned y = 20;
    unsigned xy = 1020;
    float z = 30;
    unsigned xyz = 102030;

    EXPECT_EQ(1020, CAT(x, y));
    // error: 'xCAT' was not declared in this scope; did you mean 'CAT'?
    // EXPECT_EQ(1020,  CAT(x, CAT(y, z)));
    //
    EXPECT_EQ(1020, BOOST_PP_CAT(x, y));
    EXPECT_EQ(102030, BOOST_PP_CAT(x, BOOST_PP_CAT(y, z)));  // expands to xyz
}

// How it implemented?

#define CAT_With_EXPANDING(a, b) CAT_I(a, b)
#define CAT_I(a, b) a##b

TEST(preprocessor, concatenates_impl) {
#pragma GCC diagnostic ignored "-Wunused-variable"
    int x = 10;
    unsigned y = 20;
    unsigned xy = 1020;
    float z = 30;
    unsigned xyz = 102030;

    EXPECT_EQ(1020, CAT(x, y));
    // error: 'xCAT' was not declared in this scope; did you mean 'CAT'?
    // EXPECT_EQ(1020,  CAT(x, CAT(y, z)));
    EXPECT_EQ(102030, CAT_With_EXPANDING(x, CAT_With_EXPANDING(y, z)));
}

// How it implemented? 两次展开即可!

#ifndef BOOST_BASE_FROM_MEMBER_MAX_ARITY
#define BOOST_BASE_FROM_MEMBER_MAX_ARITY 10
#endif
//  An iteration of a constructor template for base_from_member  -------------//

// A macro that should expand to:
//     template < typename T1, ..., typename Tn >
//     base_from_member( T1 x1, ..., Tn xn )
//         : member( x1, ..., xn )
//         {}
// This macro should only persist within this file.
#define BOOST_PRIVATE_CTR_DEF(z, n, data)          \
    template <BOOST_PP_ENUM_PARAMS(n, typename T)> \
    explicit base_from_member2(BOOST_PP_ENUM_BINARY_PARAMS(n, T, x)) : member(BOOST_PP_ENUM_PARAMS(n, x)) {}

//  Base-from-member class template  -----------------------------------------//

// Helper to initialize a base object so a derived class can use this
// object in the initialization of another base class.  Used by
// Dietmar Kuehl from ideas by Ron Klatcho to solve the problem of a
// base class needing to be initialized by a member.

// Contributed by Daryle Walker
template <typename MemberType, int UniqueID = 0>
class base_from_member2 {
  protected:
    MemberType member;

    base_from_member2() : member() {}

    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_BASE_FROM_MEMBER_MAX_ARITY), BOOST_PRIVATE_CTR_DEF, _)
};  // boost::base_from_member

/* It generating the such classs
template <typename MemberType, int UniqueID = 0>
class base_from_member2 {
  protected:
    MemberType member;

    base_from_member2() : member() {}

    template <typename T0>
    explicit base_from_member2(T0 x0) : member(x0) {}

    template <typename T0, typename T1>
    explicit base_from_member2(T0 x0, T1 x1) : member(x0, x1) {}

    template <typename T0, typename T1, typename T2>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2) : member(x0, x1, x2) {}

    template <typename T0, typename T1, typename T2, typename T3>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3) : member(x0, x1, x2, x3) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4) : member(x0, x1, x2, x3, x4) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5) : member(x0, x1, x2, x3, x4, x5) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6) : member(x0, x1, x2, x3, x4, x5, x6) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
      : member(x0, x1, x2, x3, x4, x5, x6, x7) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
              typename T8>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7, T8 x8)
      : member(x0, x1, x2, x3, x4, x5, x6, x7, x8) {}

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9>
    explicit base_from_member2(T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7, T8 x8, T9 x9)
      : member(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) {}
};

*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
