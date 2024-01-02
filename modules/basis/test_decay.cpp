#include <type_traits>

#include <gtest/gtest.h>

/*
https://en.cppreference.com/w/cpp/types/decay

template< class T > struct decay; (since C++11)

Performs the type conversions equivalent to the ones performed when passing function arguments by value.
Formally:
* If T is "array of U" or reference to it, the member typedef type is U*.
* Otherwise, if T is a function type F or reference to one, the member typedef type is std::add_pointer<F>::type.
* Otherwise, the member typedef type is std::remove_cv<std::remove_reference<T>::type>::type.

The behavior of a program that adds specializations for std::decay is undefined.

template< class T > using decay_t = typename decay<T>::type; (since C++14)
*/

template <typename T, typename U>
constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>, U>;

TEST(decay, tutorial) {
    bool same = is_decay_equ<int, int>;
    EXPECT_TRUE(same);
}

// cv限定符去除
TEST(decay, tutorial2) {
    bool same = is_decay_equ<int&, int>;
    EXPECT_TRUE(same);
}

// cv限定符去除
TEST(decay, tutorial3) {
    bool same = is_decay_equ<int&&, int>;
    EXPECT_TRUE(same);
}

// cv限定符去除
TEST(decay, tutorial4) {
    bool same = is_decay_equ<const int&, int>;
    EXPECT_TRUE(same);
}

TEST(decay, tutorial5) {
    bool same = is_decay_equ<int, float>;
    EXPECT_FALSE(same);
}

// 数组到指针
TEST(decay, tutorial6) {
    bool same = is_decay_equ<int[2], int*>;
    EXPECT_TRUE(same);
}

TEST(decay, tutorial7) {
    bool same = is_decay_equ<int[4][2], int*>;
    EXPECT_FALSE(same);
}

TEST(decay, tutorial8) {
    bool same = is_decay_equ<int[4][2], int**>;
    EXPECT_FALSE(same);
}

TEST(decay, tutorial9) {
    bool same = is_decay_equ<int[4][2], int(*)[2]>;
    EXPECT_TRUE(same);
}

// 函数到指针
TEST(decay, tutorial10) {
    bool same = is_decay_equ<int(int), int (*)(int)>;
    EXPECT_TRUE(same);
}
