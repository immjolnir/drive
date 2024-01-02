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

/**
 * https://en.cppreference.com/w/cpp/types/result_of
 * std::result_of will be deprecated since c++20, better using invoke_result since c++17.
 *

template<class>
struct result_of;

template<class F, class... ArgTypes>
struct result_of<F(ArgTypes...)> : detail::invoke_result<void, F, ArgTypes...> {};

template<class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};
*/

namespace invoke_result {
struct S {
    double operator()(char, int&);

    float operator()(int) { return 1.0; }
};

template <class T>
typename std::result_of<T(int)>::type f(T& t) {
    std::cout << "overload of f for callable T\n";
    return t(0);
}

template <class T, class U>
int f(U u) {
    std::cout << "overload of f for non-callable T\n";
    return u;
}

TEST(type_traits, invoke_result) {
    // the result of invoking S with char and int& arguments is double
    std::result_of<S(char, int&)>::type x = 3.14;  // d has type double
    bool same_type = std::is_same<decltype(x), double>::value;
    EXPECT_TRUE(same_type);

    // std::invoke_result uses different syntax (no parentheses)
    std::invoke_result<S, char, int&>::type y = 3.14;
    bool same_type0 = std::is_same<decltype(y), double>::value;
    EXPECT_TRUE(same_type0);
}

TEST(type_traits, invoke_result2) {
    // the result of invoking S with int argument is float
    std::result_of<S(int)>::type x = 3.14;  // x has type float
    bool same_type = std::is_same<decltype(x), float>::value;
    EXPECT_TRUE(same_type);

    // std::invoke_result uses different syntax (no parentheses)
    std::invoke_result<S, int>::type y = 3.14;  // x has type float
    bool same_type0 = std::is_same<decltype(y), float>::value;
    EXPECT_TRUE(same_type0);
}

TEST(type_traits, invoke_result3) {
    // result_of can be used with a pointer to member function as follows
    struct C {
        double Func(char, int&);
    };

    {
        std::result_of<decltype (&C::Func)(C, char, int&)>::type x = 3.14;
        bool same_type = std::is_same<decltype(x), double>::value;
        EXPECT_TRUE(same_type);
    }

    {
        std::invoke_result<decltype(&C::Func), C, char, int&>::type x = 3.14;
        bool same_type = std::is_same<decltype(x), double>::value;
        EXPECT_TRUE(same_type);
    }
}

//  error: a template declaration cannot appear at block scope
template <typename T0, typename T1>
struct Student {
    double Func(T0, T1&);
};

TEST(type_traits, invoke_result30) {
    {
        std::result_of<decltype (&Student<char, int>::Func)(Student<char, int>, char, int&)>::type x = 3.14;
        bool same_type = std::is_same<decltype(x), double>::value;
        EXPECT_TRUE(same_type);
    }

    {
        std::invoke_result<decltype(&Student<char, int>::Func), Student<char, int>, char, int&>::type x = 3.14;
        bool same_type = std::is_same<decltype(x), double>::value;
        EXPECT_TRUE(same_type);
    }
}

// NEW CASE:
// https://stackoverflow.com/questions/46021734/what-is-the-reason-for-stdresult-of-deprecated-in-c17
auto add_auto_fn(int a, int b) { return a + b; }

template <typename U, typename V>
auto add_auto_template_fn(U a, V b) {
    return a + b;
}

int fortytwo(int a, int b) { return a + 42; }

struct my_functor {
    auto operator()(int a) { return a + 42; }
};

TEST(type_traits, invoke_result4) {
    {
        // For functions and auto function: use < decltype(&f), Args... >
        using T = std::invoke_result<decltype(&fortytwo), int, int>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }
    {
        // For functions and auto function: use < decltype(&f)(Args...) >
        using T = std::result_of<decltype (&fortytwo)(int, int)>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }
}

TEST(type_traits, invoke_result50) {
    {
        // For templated auto functions: use < decltype(&f)<Args...>, Args... >
        using T = std::invoke_result<decltype(&add_auto_template_fn<int, double>), int, double>::type;
        bool same_type = std::is_same<T, double>::value;
        EXPECT_TRUE(same_type);
    }
    {
        // For templated auto functions: use < decltype(&f<Args...>)(Args...) >
        using T = std::result_of<decltype (&add_auto_template_fn<int, double>)(int, double)>::type;
        bool same_type = std::is_same<T, double>::value;
        EXPECT_TRUE(same_type);
    }
}

TEST(type_traits, invoke_result5) {
    {
        // For simple lambdas: use < decltype(lambda), Args... >
        auto simple_lambda = [](int a) { return a + 42; };
        using T = std::invoke_result<decltype(simple_lambda), int>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }
    {
        // For simple lambdas: use < decltype(lambda)(Args...) >
        auto simple_lambda = [](int a) { return a + 42; };
        using T = std::result_of<decltype(simple_lambda)(int)>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }
}

TEST(type_traits, invoke_result6) {
    {
        // For generic lambdas: use < decltype(lambda), Args... >
        auto generic_lambda = [](auto a) { return a + 42; };
        using T = std::invoke_result<decltype(generic_lambda), double>::type;
        bool same_type = std::is_same<T, double>::value;
        EXPECT_TRUE(same_type);
    }
    {
        // For generic lambdas: use < decltype(lambda)(Args...) >
        auto generic_lambda = [](auto a) { return a + 42; };
        using T = std::result_of<decltype(generic_lambda)(double)>::type;
        bool same_type = std::is_same<T, double>::value;
        EXPECT_TRUE(same_type);
    }
}

TEST(type_traits, invoke_result7) {
    {
        // For functors: use < functor, Args... >
        using T = std::invoke_result<my_functor, int>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }

    {
        // For functors: use < functor(Args...) >
        using T = std::result_of<my_functor(int)>::type;
        bool same_type = std::is_same<T, int>::value;
        EXPECT_TRUE(same_type);
    }
}

//     f<C>(1);  // may fail to compile in C++11; calls the non-callable overload in C++14
// }
}  // namespace invoke_result