#include <gtest/gtest.h>

#include <type_traits>
#include <typeinfo>

struct A {
    void foo() {}
};

struct B {};

struct C {
    int foo;
};

namespace has_member {
/*
C++的标准库 type_traits 并不直接提供判断一个类是否包含特定成员（无论是变量还是函数）的机制。
这是因为类型 traits 主要用于在编译时获取或检查类型的属性，如是否为某种内置类型、是否有某种操作符重载等。
然而，你可以通过一些模板元编程技巧 和 SFINAE（Substitution Failure Is Not An Error）原则来实现这样的检查。
以下是一个基本的示例，展示如何检查一个类是否有一个名为 member 的公共数据成员：
*/

template <typename T>
class has_data {
  private:
    template <typename U>
    static std::true_type _test(decltype(&U::foo)*);

    template <typename>
    static std::false_type _test(...);

  public:
    // It must be declared after _test else we get such error:
    // there are no arguments to '_test' that depend on a template parameter, so a declaration of '_test' must be
    // available
    static constexpr bool value = decltype(_test<T>(nullptr))::value;
};

template <typename T>
inline constexpr bool has_data_v = has_data<T>::value;

TEST(has_member, data_member) {
    EXPECT_TRUE(has_data<A>::value);
    EXPECT_TRUE(has_data_v<A>);  // A::foo() method makes it true.

    EXPECT_FALSE(has_data<B>::value);
    EXPECT_FALSE(has_data_v<B>);

    EXPECT_TRUE(has_data<C>::value);
    EXPECT_TRUE(has_data_v<C>);
}
}  // namespace has_member

// 检查一个类是否有一个名为 foo的公共成员函数：
namespace has_member2 {
// 定义一个 traits 类
template <typename T>
class has_method {
    // 使用 SFINAE 技术，尝试实例化这个辅助类,声明一个只有当 U::foo 存在时才有效的类型
    template <typename U>
    static std::true_type _test(decltype(&U::foo)*);

    // 如果找不到 &U::foo，则实例化这个辅助类
    template <typename>
    static std::false_type _test(...);

  public:  // 判断上面哪个模板被实例化
    using type = decltype(_test<T>(nullptr));
};

template <typename T>
inline constexpr bool has_method_v = has_method<T>::type::value;

TEST(type_traits, method_member) {
    EXPECT_TRUE(has_method_v<A>);
    EXPECT_FALSE(has_method_v<B>);
    EXPECT_TRUE(has_method_v<C>);  // int foo; data member makes it true too.
}

/*
 在这个例子中，  has_foo 是一个traits类，它使用SFINAE来尝试调用 T::foo。
 如果 T 有一个名为 foo的成员函数，那么 test 的第一个模板版本会被选择，否则会选择第二个模板版本。
 通过这种方式，我们可以确定 T是否具有 foo成员函数。

 请注意，这个例子只能检查是否存在一个可调用的 foo成员，不能精确地判断它是一个函数、数据成员或其他类型的成员。
 如果你需要更复杂的检查，可能需要使用更复杂的技术，如编译期元编程或者C++20的反射特性（如果可用的话）。
*/
}  // namespace has_member2

// but 上面的两个例子都不能区分包含的是 数据成员 还是 方法成员。尴尬了.

namespace tell_me_what_is_it {
template <typename T>
class has_foo_data_member {
    template <typename _Tp, typename = decltype(std::declval<_Tp&>().foo)>
    static std::true_type __test(int);

    template <typename>
    static std::false_type __test(...);

  public:  // 判断上面哪个模板被实例化
    using type = decltype(__test<T>(0));
};

template <typename T>
inline constexpr bool has_foo_data_member_v = has_foo_data_member<T>::type::value;

template <typename T>
class has_foo_method_member {
    template <typename _Tp, typename = decltype(std::declval<_Tp&>().foo())>  // 这种方法，只适用于foo没有参数的情况
    static std::true_type __test(int);

    template <typename>
    static std::false_type __test(...);

  public:  // 判断上面哪个模板被实例化
    using type = decltype(__test<T>(0));
};

template <typename T>
inline constexpr bool has_foo_method_member_v = has_foo_method_member<T>::type::value;

TEST(type_traits, foo_data_member) {
    EXPECT_FALSE(has_foo_data_member_v<A>);
    EXPECT_FALSE(has_foo_data_member_v<B>);
    EXPECT_TRUE(has_foo_data_member_v<C>);
}

TEST(type_traits, foo_method_member) {
    EXPECT_TRUE(has_foo_method_member_v<A>);
    EXPECT_FALSE(has_foo_method_member_v<B>);
    EXPECT_FALSE(has_foo_method_member_v<C>);
}

}  // namespace tell_me_what_is_it

namespace has_size {
// /usr/include/boost/range/detail/has_member_size.hpp
template <typename T>
class has_member_size_impl {
  private:
    template <class U, U>
    class check {};

    template <class C>
    static uint8_t f(check<std::size_t (C::*)(void) const, &C::size>*);

    template <class C>
    static uint16_t f(...);

  public:
    static const bool value = (sizeof(f<T>(0)) == sizeof(uint8_t));
};

template <typename T>
struct has_member_size {
    static const bool value = std::is_class<T>::value && has_member_size_impl<const T>::value;
};

template <typename T>
static constexpr bool has_member_size_v = has_member_size<T>::value;

class MyClass {
  public:
    std::size_t size() const { return 1024; }
};

class MyClass_Mutable {
  public:
    std::size_t size() { return 1024; }  // no const
};

class MyClass_PrivateM {
    std::size_t size() const { return 1024; }
};

TEST(type_traits, has_size_method_member) {
    EXPECT_TRUE(has_member_size_v<MyClass>);
    EXPECT_FALSE(has_member_size_v<MyClass_Mutable>);
    EXPECT_FALSE(has_member_size_v<MyClass_PrivateM>);
    EXPECT_FALSE(has_member_size_v<A>);
}

}  // namespace has_size

// /usr/include/boost/intrusive/detail/has_member_function_callable_with.hpp
namespace boost_example {
typedef char yes_type;

struct no_type {
    char dummy[2];
};

struct dont_care {
    dont_care(...);
};

template <typename T, class... Args>
struct has_member_function_callable_with_allocate {
    // decltype里是逗号表达式吗?
    template <typename U>
    static decltype(std::declval<U>().allocate(std::declval<Args>()...), yes_type()) Test(U* f);

    template <typename U>
    static no_type Test(...);

    static constexpr bool value = sizeof(Test<T>(nullptr)) == sizeof(yes_type);
};

template <typename T, class... Args>
struct has_member_function_callable_with_destroy {
    template <typename U>
    static decltype((std::declval<U>().destroy(std::declval<Args>()...), yes_type())) Test(U* f);

    template <typename U>
    static no_type Test(...);

    static constexpr bool value = sizeof(Test<T>(nullptr)) == sizeof(yes_type);
};

template <typename T, class... Args>
struct has_member_function_callable_with_construct {
    template <typename U>
    static decltype(std::declval<U>().construct(std::declval<Args>()...), yes_type()) Test(U* f);

    template <typename U>
    static no_type Test(...);

    static constexpr bool value = sizeof(Test<T>(nullptr)) == sizeof(yes_type);
};

class MyClass {
  public:
    void allocate(int) {}

    void construct() {}

    void destroy(int, float) {}
};

TEST(type_traits, has_member_function_callable_allocate) {
    bool hasit = false;
    hasit = has_member_function_callable_with_allocate<MyClass, int>::value;
    EXPECT_TRUE(hasit);

    hasit = has_member_function_callable_with_allocate<MyClass, int, double>::value;
    EXPECT_FALSE(hasit);

    hasit = has_member_function_callable_with_allocate<A, int>::value;
    EXPECT_FALSE(hasit);

    hasit = has_member_function_callable_with_allocate<B, int>::value;
    EXPECT_FALSE(hasit);
}

TEST(type_traits, has_member_function_callable_destroy) {
    {
        bool hasit = false;
        hasit = has_member_function_callable_with_destroy<MyClass, int, float>::value;
        EXPECT_TRUE(hasit);
    }

    {  // Change the order of template params
        bool hasit = false;
        hasit = has_member_function_callable_with_destroy<MyClass, float, int>::value;
        std::cout << "Myclass has the destory(float, int) ? " << hasit << std::endl;  // 1 but why?
        EXPECT_FALSE(hasit);  // why changing the order, it still true?
    }
    {  // Change the parameter type: double -> float
        bool hasit = has_member_function_callable_with_destroy<MyClass, int, double>::value;
        std::cout << "Myclass has the destory(int, double) ? " << hasit << std::endl;  // 1 but why?
        EXPECT_FALSE(hasit);  // unsafety type convertion: double -> float
    }

    {  // wrong param
        bool hasit = has_member_function_callable_with_destroy<MyClass>::value;
        EXPECT_FALSE(hasit);
    }

    {
        bool hasit = has_member_function_callable_with_allocate<A, int, float>::value;
        EXPECT_FALSE(hasit);
    }

    {
        bool hasit = has_member_function_callable_with_allocate<B, int, float>::value;
        EXPECT_FALSE(hasit);
    }
}

TEST(type_traits, has_member_function_callable_construct) {
    bool hasit = false;
    hasit = has_member_function_callable_with_construct<MyClass>::value;
    EXPECT_TRUE(hasit);

    hasit = has_member_function_callable_with_construct<MyClass, std::string>::value;
    EXPECT_FALSE(hasit);

    hasit = has_member_function_callable_with_construct<MyClass, int, double>::value;
    EXPECT_FALSE(hasit);
}

}  // namespace boost_example