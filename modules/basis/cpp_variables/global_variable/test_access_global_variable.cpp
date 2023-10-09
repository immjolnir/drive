#include <gtest/gtest.h>

// class VariableScopeAndStorage : public ::testing::Test {};

// Global variable
int a = 3;
int b = 3;

TEST(global_variable, access_a_global_besides_a_local) {
    // Local variable
    int a = 7;

    EXPECT_EQ(::a, 3);
    EXPECT_EQ(a, 7);
}

TEST(global_variable, working_with_a_global) {
    // global variables are declared at the top of the program outside any function or block of code. These variables
    // can be accessed from within any function of the block of code. They can be accessed and modified anywhere in the
    // program without any compilation error.
    b = b * 3;

    EXPECT_EQ(::b, 9);
    EXPECT_EQ(b, 9);
}
