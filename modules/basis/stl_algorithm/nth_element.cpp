#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

// nth_element
// https://en.cppreference.com/w/cpp/algorithm/nth_element
// nth_element is a partial sorting algorithm that rearranges elements in [first, last) such that:
//
// It does not sort the list, just that all the elements, which precede the nth element are not greater than it, and all
// the elements which succeed it are not less than it.
//
// Where can we apply std::nth_element() ?
// It can be used if we want to find the first n smallest numbers, but they may or maynot be ordered
//
void printVec(const std::vector<int>& vec) {
    std::cout << "v = {";
    // warning: range-based 'for' loops with initializer only available with '-std=c++20' or '-std=gnu++20'
    for (char sep[]{0, ' ', 0}; const int i : vec) {
        std::cout << sep << i, sep[0] = ',';
    }
    std::cout << "};\n";
}

TEST(nth_element, quick_start) {
    std::vector<int> v{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
    printVec(v);
    auto m = v.begin() + static_cast<long int>(v.size()) / 2;

    EXPECT_EQ(2, *m);
    std::nth_element(v.begin(), m, v.end());
    EXPECT_EQ(6, *m);
    // Median Element
    EXPECT_EQ(6, v[v.size() / 2]);
    printVec(v);
}

TEST(nth_element, reverse_order) {
    std::vector<int> v{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
    printVec(v);

    EXPECT_EQ(10, *(v.begin() + 1));
    // Note: comp function changed
    std::nth_element(v.begin(), v.begin() + 1, v.end(),
                     std::greater{});  // using greater() to find the first n largest numbers/
    EXPECT_EQ(9, v[1]);                // the second largest element
    EXPECT_EQ(10, v[0]);               // the largest element
    printVec(v);
}
