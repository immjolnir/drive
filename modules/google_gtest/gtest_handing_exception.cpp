#include <gtest/gtest.h>

#include <stdexcept>

struct foo {
    int bar(int i) {
        if (i > 100) {
            throw std::out_of_range("Out of range");
        }
        return i;
    }
};

TEST(foo_test, out_of_range) {
    foo f;
    try {
        f.bar(111);
        FAIL() << "Expected std::out_of_range";
    } catch (std::out_of_range const& err) {
        EXPECT_EQ(err.what(), std::string("Out of range"));
    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }
}

TEST(foo_test, out_of_range_wrong_coding) {
    // It must be failed. Keep it.
    foo f;
    try {
        f.bar(1);
        FAIL() << "Expected std::out_of_range";
    } catch (std::out_of_range const& err) {
        EXPECT_EQ(err.what(), std::string("Out of range"));
    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }
}

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }