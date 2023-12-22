/**
 * Calling class method through null class pointer
 * https://stackoverflow.com/questions/2505328/calling-class-method-through-null-class-pointer
 */
#include <gtest/gtest.h>

class Student {
  public:
    int get() const { return _m; }

    int getConstant() const { return 1; }

  private:
    int _m = 0;
};

TEST(calling_method_through_nullptr, Segmentation_fault) {
    Student* st = nullptr;
    EXPECT_EXIT((st->get(), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(calling_method_through_nullptr, failed_case) {
    Student* st = nullptr;
    // Death test: (st->getConstant(), exit(0))
    // Result: died but not with expected exit code:
    //         Exited with exit status 0
    EXPECT_EXIT((st->getConstant(), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(calling_method_through_nullptr, ok) {
    Student* st = nullptr;
    EXPECT_EQ(1, st->getConstant());
}
