#include <gtest/gtest.h>

/*

*/
int deref(int* pint) { return *pint; }

TEST(test_deref_1, will_segfault) { ASSERT_EXIT((deref(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*"); }

TEST(test_dref_2, will_not_segfault) {
    int i = 42;
    EXPECT_EQ(42, deref(&i));
    ASSERT_EXIT((deref(&i), exit(0)), ::testing::ExitedWithCode(0), ".*");
}

/*
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
