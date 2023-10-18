#include <gtest/gtest.h>

#include <vector>

TEST(iterator, swap) {
    std::vector<int> vec{1, 2, 3};

    std::vector<int>::iterator k = vec.begin();
    std::vector<int>::iterator j = vec.end();

    EXPECT_NE(k, j);

    --j;
    EXPECT_EQ(1, *k);
    EXPECT_EQ(3, *j);

    // Swap their values
    std::swap(*k, *j);

    EXPECT_EQ(3, *k);
    EXPECT_EQ(1, *j);

    EXPECT_EQ(3, vec.front());
    EXPECT_EQ(1, vec.back());
}