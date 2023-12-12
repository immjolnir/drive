#include <algorithm>  // sort
#include <array>
#include <numeric>  // std::accumulate

#include <gtest/gtest.h>
using namespace std;

// Please use std::array in c++.
TEST(raw_array, sort_empty_str_array) {
    // uninitialized state, contains random values, for empty: { -2016213120, 0, 0, 32766, 79760280 }
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    std::array<int, 5> arr;  // left uninitialized
    EXPECT_EQ(5, arr.size());
    std::sort(arr.begin(), arr.end());

    std::array<int, 5> expected = {};  // initialize it with { 0, 0, 0, 0, 0 }
    EXPECT_EQ(expected, arr);
}

TEST(raw_array, sort_std_array) {
    std::array<int, 5> arr = {1, 5, 7, 2, 4};
    EXPECT_EQ(5, arr.size());
    std::sort(arr.begin(), arr.end());
    std::array<int, 5> expected = {1, 2, 4, 5, 7};
    EXPECT_EQ(expected, arr);
}
