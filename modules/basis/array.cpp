#include <algorithm>  // sort
#include <array>
#include <numeric>  // std::accumulate

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;

// Please use std::array in c++.
TEST(std_array, sort_empty_str_array) {
      GTEST_SKIP() << "Skipping single test";
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    // uninitialized state, contains random values, for empty: { -2016213120, 0, 0, 32766, 79760280 }
    std::array<int, 5> arr;  // left uninitialized
    EXPECT_EQ(5, arr.size());
    std::sort(arr.begin(), arr.end());

    std::array<int, 5> expected = {};  // initialize it with { 0, 0, 0, 0, 0 }
    /*
    Expected equality of these values:
    expected
        Which is: { 0, 0, 0, 0, 0 }
    arr
        Which is: { -911123304, -670999248, 0, 0, 32765 }
    */
    EXPECT_EQ(expected, arr);
}

TEST(std_array, sort_std_array) {
    std::array<int, 5> arr = {1, 5, 7, 2, 4};  // double braces never required after =
    EXPECT_EQ(5, arr.size());

    std::sort(arr.begin(), arr.end());
    std::array<int, 5> expected = {1, 2, 4, 5, 7};
    EXPECT_EQ(expected, arr);
}

TEST(std_array, aggregate_initialization) {
    // construction uses aggregate initialization
    std::array<int, 3> arr{{1, 2, 3}};  // double-braces required in C++11 prior to
                                        // the CWG 1270 revision (not needed in C++11
                                        // after the revision and in C++14 and beyond)

    EXPECT_EQ(3, arr.size());
    EXPECT_THAT(arr, testing::ElementsAre(1, 2, 3));
}
