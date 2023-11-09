#include <gtest/gtest.h>

#include <type_traits>

template <typename T>
struct foo {};

template <typename T>
struct is_instantiation_of_foo : std::false_type {};

template <typename T>
struct is_instantiation_of_foo<foo<T>> : std::true_type {};

// Why define it like this? And how to use it?
template <typename... Ts>
struct is_instantiation_of_foo<foo<Ts...>> : std::true_type {};

template <typename FooType>
struct bar {
    static_assert(is_instantiation_of_foo<FooType>::value, "");  // fail
};

template <typename T>
using foo_alt = foo<T>;

TEST(template, substitution_is_not_fail) {
    // both of these fail:
    bar<foo<int>> b;
    bar<foo_alt<int>> b2;

    EXPECT_TRUE(is_instantiation_of_foo<foo<int>>::value);
    EXPECT_TRUE(is_instantiation_of_foo<foo<bool>>::value);

    EXPECT_TRUE(is_instantiation_of_foo<foo_alt<bool>>::value);
}
