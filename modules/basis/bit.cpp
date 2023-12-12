#include <gtest/gtest.h>

#include <limits.h>  // for CHAR_BIT

bool if_opposite_signs(int x, int y) {  // input values to compare signs
    // http://graphics.stanford.edu/~seander/bithacks.html
    // Detect if two integers have opposite signs

    // No need to take care of the order of x and y in the formula
    bool f = ((x ^ y) < 0);  // true iff x and y have opposite signs
    return f;
}

TEST(bithacks, detect_two_var_has_opposite_signs) {
    EXPECT_FALSE(if_opposite_signs(1, 1));
    EXPECT_FALSE(if_opposite_signs(-1, -1));

    EXPECT_TRUE(if_opposite_signs(1, -1));
    EXPECT_TRUE(if_opposite_signs(-1, 1));

    // What's the sign of zereo?
    EXPECT_FALSE(if_opposite_signs(0, 1));  // So, 0 has positive sign in this implemetation.
    EXPECT_TRUE(if_opposite_signs(0, -1));
}

// unsigned int 与 int 不能混用
int get_abs(int v) {
    // we want to find the absolute value of v
    int r;  // the result goes here
    int const mask = v >> sizeof(int) * CHAR_BIT - 1;

    // error: conversion to 'unsigned int' from 'int' may change the sign of the result [-Werror=sign-conversion]
    // r = (v + mask) ^ mask; // Is the `v + mask` possibly overflow?
    // VS
    r = (v ^ mask) - mask;
    return r;
}

TEST(bithacks, positive_case) {
    int i = -1;
    int abs_i = get_abs(i);
    EXPECT_EQ(1, abs_i);
}

TEST(bithacks, positive_case2) {
    int i = 1;
    int abs_i = get_abs(i);
    EXPECT_EQ(1, abs_i);
}
