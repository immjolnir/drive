#include <algorithm>  // sort
#include <numeric>    // std::accumulate
#include <string>
#include <vector>

#include <gtest/gtest.h>
using namespace std;

// Please use std::array in c++.
TEST(raw_array, sort_empty_array) {
    int arr[] = {};
    int n = sizeof(arr) / sizeof(arr[0]);
    std::sort(arr, arr + n);
    EXPECT_EQ(0, n);
}

TEST(raw_array, sort_array) {
    int arr[] = {1, 5, 7, 2, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    std::sort(arr, arr + n);
    // construct the vector after you've gotten the array and array size
    // https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
    std::vector<int> actual(arr, arr + n);
    std::vector<int> expected({1, 2, 4, 5, 7});
    EXPECT_EQ(expected, actual);
}
