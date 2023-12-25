#include <gtest/gtest.h>

#include <type_traits>
#include <typeinfo>

// https://en.cppreference.com/w/cpp/types/conditional
TEST(type_traits, conditional) {
    using Type1 = std::conditional<true, int, double>::type;
    using Type2 = std::conditional<false, int, double>::type;
    using Type3 = std::conditional<sizeof(int) >= sizeof(double), int, double>::type;

    // /usr/include/c++/12/typeinfo
    /** Returns an @e implementation-defined byte string; this is not
     *  portable between compilers!  */
    // const char* name() const _GLIBCXX_NOEXCEPT { return __name[0] == '*' ? __name + 1 : __name; }

    EXPECT_EQ("i", std::string(typeid(Type1).name()));  // int
    EXPECT_EQ("d", std::string(typeid(Type2).name()));  // double
    EXPECT_EQ("d", std::string(typeid(Type3).name()));  // double
}
