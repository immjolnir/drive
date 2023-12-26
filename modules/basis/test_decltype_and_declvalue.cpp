#include <gtest/gtest.h>

#include <type_traits>

namespace decltype_and_declval {

typedef char yes_type;

struct no_type {
    char dummy[2];
};

template <typename T, class... Args>
struct has_method_concat {
    template <typename U>
    static decltype(std::declval<U>().concat(std::declval<Args>()...), yes_type()) Test(U* f);

    template <typename U>
    static no_type Test(...);

    static constexpr bool value = sizeof(Test<T>(nullptr)) == sizeof(yes_type);
};

template <typename T, class... Args>
static constexpr bool has_method_concat_v = has_method_concat<T, Args...>::value;

struct Foo {};

struct Bar {};

class MyClass {
  public:
    void concat(int, double) {}

    void concat(const std::string&, double) {}

    void concat(Foo, Bar) {}
};

TEST(decltype_and_declval, concat_example) {
    {
        bool left = has_method_concat<MyClass, int, double>::value;
        bool right = has_method_concat_v<MyClass, int, double>;
        EXPECT_EQ(left, right);
    }

    {
        bool left = has_method_concat<MyClass, const std::string&, double>::value;
        bool right = has_method_concat_v<MyClass, const std::string&, double>;
        EXPECT_EQ(left, right);
    }
}

TEST(decltype_and_declval, concat_reverse_order) {
    {
        bool has = has_method_concat_v<MyClass, int, double>;
        EXPECT_TRUE(has);

        bool has0 = has_method_concat_v<MyClass, double, int>;
        EXPECT_TRUE(has0);  // Still be true. It may be cause by implicit convertion.
    }

    {
        bool has = has_method_concat_v<MyClass, const std::string&, double>;
        EXPECT_TRUE(has);

        bool has2 = has_method_concat_v<MyClass, std::string, double>;  // reverse order
        EXPECT_TRUE(has2);

        bool has0 = has_method_concat_v<MyClass, double, const std::string&>;
        EXPECT_FALSE(has0);

        bool has1 = has_method_concat_v<MyClass, double, std::string>;
        EXPECT_FALSE(has1);
    }
}

// std::declval 对参数类型要求并不严格，就是说，只要可以相互转换，即可。
TEST(decltype_and_declval, concat_reverse_order2) {
    bool has = has_method_concat_v<MyClass, Foo, Bar>;
    EXPECT_TRUE(has);

    bool has0 = has_method_concat_v<MyClass, Bar, Foo>;
    EXPECT_FALSE(has0);  // yes, 对于 默认不支持相互转换的类型，declvalue 就严格起来了。
}
}  // namespace decltype_and_declval

namespace check_has_member_or_not {

template <typename T>
auto constexpr has_size_member(int) -> decltype(std::declval<T&>().size(), bool{}) {
    // 这里的 decltype 应该就是 逗号表达式
    return true;
}

template <typename>
auto constexpr has_size_member(long) -> bool {
    return false;
}

template <typename T>
static constexpr bool has_size_member_v = has_size_member<T>(0);

struct A {
    size_t size() const;
};

struct B {};

TEST(decltype_and_declval, has_size_member) {
    EXPECT_TRUE(has_size_member_v<A>);
    EXPECT_TRUE(!has_size_member_v<B>);  // B should not have a size member
}

}  // namespace check_has_member_or_not
