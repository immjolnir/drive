/*
https://www.geeksforgeeks.org/stdnext_permutation-prev_permutation-c/

It is used to rearrange the elements in the range [first, last) into the previous lexicographically-ordered permutation.
A permutation is each one of the N! possible arrangements the elements can take (where N is the number of elements in
the range). Different permutations can be ordered according to how they compare lexicographically to each other. The
time complexity of the code is O(n*n!) as each permutation takes linear time.
*/

#include <gtest/gtest.h>
#include <algorithm>

#include <iostream>

TEST(permutation, pre_permutation_illustration) {
    int arr[] = {1, 2, 3};

    std::sort(arr, arr + 3);
    std::reverse(arr, arr + 3);

    int n = 0;
    std::cout << "The 3! possible permutations with 3 elements:\n";
    do {
        ++n;
        std::cout << arr[0] << " " << arr[1] << " " << arr[2] << "\n";
    } while (std::prev_permutation(arr, arr + 3));
    std::cout << "After loop: " << arr[0] << ' ' << arr[1] << ' ' << arr[2] << '\n';
    // 3! = 6
    EXPECT_EQ(6, n);

    EXPECT_EQ(3, arr[0]);
    EXPECT_EQ(2, arr[1]);
    EXPECT_EQ(1, arr[2]);
}