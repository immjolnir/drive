#include <gtest/gtest.h>

#include "arrow/util/small_vector.h"

namespace arrow
{

    // Static Vector test cases
    TEST(StaticVector, empty_container) {
        StaticVector<int, 4> vec;
        EXPECT_TRUE(vec.empty());
        EXPECT_EQ(0, vec.size());
        EXPECT_EQ(4, vec.capacity());
    }

    TEST(StaticVector, push_back_the_container) {
        StaticVector<int, 4> vec;
        vec.push_back(1);
        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(1, vec.size());
        EXPECT_EQ(4, vec.capacity());

        EXPECT_EQ(1, vec.front());
        EXPECT_EQ(1, vec.back());
    }

    TEST(StaticVector, emplace_back_the_container) {
        StaticVector<int, 4> vec;
        vec.emplace_back(1);
        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(1, vec.size());
        EXPECT_EQ(4, vec.capacity());

        EXPECT_EQ(1, vec.front());
        EXPECT_EQ(1, vec.back());
    }

    TEST(StaticVector, full_container_with_push_back) {
        StaticVector<int, 4> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);

        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(4, vec.size());
        EXPECT_EQ(4, vec.capacity());

        EXPECT_EQ(1, vec.front());
        EXPECT_EQ(4, vec.back());
    }

    TEST(StaticVector, full_container_emplace_back) {
        StaticVector<int, 4> vec;
        vec.emplace_back(1);
        vec.emplace_back(2);
        vec.emplace_back(3);
        vec.emplace_back(4);

        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(4, vec.size());
        EXPECT_EQ(4, vec.capacity());

        EXPECT_EQ(1, vec.front());
        EXPECT_EQ(1, vec[0]);

        EXPECT_EQ(4, vec.back());
        EXPECT_EQ(4, vec[3]);
    }

    TEST(StaticVector, mutate_container) {
        StaticVector<int, 4> vec;
        vec.emplace_back(1);
        vec.emplace_back(2);
        vec.emplace_back(3);
        vec.emplace_back(4);

        vec[0] = 10;
        vec[2] = 12;
        EXPECT_EQ(10, vec.front());
        EXPECT_EQ(10, vec[0]);
        EXPECT_EQ(12, vec[2]);
    }

    // SmallVector test cases
    TEST(SmallVector, Traits) { EXPECT_FALSE((std::is_trivially_destructible<SmallVector<int, 4>>::value)); }

}  // namespace arrow