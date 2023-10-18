/*
https://en.cppreference.com/w/cpp/algorithm/next_permutation


bool next_permutation( BidirIt first, BidirIt last );

bool next_permutation( BidirIt first, BidirIt last, Compare comp );

It is used to rearrange the elements in the range [first, last) into the next lexicographically greater permutation. A
permutation is each one of the N! possible arrangements the elements can take (where N is the number of elements in the
range). Different permutations can be ordered according to how they compare lexicographically to each other. The
complexity of the code is O(n*n!) which also includes printing all the permutations.


true if the new permutation is lexicographically greater than the old. false if the last permutation was reached and the
range was reset to the first permutation.

Possible implementation
template<class BidirIt>
bool next_permutation(BidirIt first, BidirIt last)
{
    auto r_first = std::make_reverse_iterator(last);
    auto r_last = std::make_reverse_iterator(first);
    auto left = std::is_sorted_until(r_first, r_last);

    if (left != r_last)
    {
        auto right = std::upper_bound(r_first, left, *left);
        std::iter_swap(left, right);
    }

    std::reverse(left.base(), last);
    return left != r_last;
}
*/

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

TEST(permutation, next_permutation) {
    std::string s = "aba";

    do std::cout << s << '\n';
    while (std::next_permutation(s.begin(), s.end()));

    std::cout << s << '\n';
}

TEST(permutation, next_permutation2) {
    // We start off by creating a vector of integers containing the numbers 1-9.
    // This ordering is very important, because it's the "lexicographically
    // smallest" ordering of the numbers 1-9.
    //
    // You can think of the "lexicographically smallest" ordering as the ordering
    // which would be produced if you sorted your elements.
    std::vector<int> arrangement;
    for (int i = 1; i <= 9; i++) {
        arrangement.push_back(i);
    }

    int n = 0;

    // We'll loop over all permutations of our vector...
    while (true) {
        // These two lines are the really fun part of this algorithm.
        //
        // The next_permutation function will rearrange the elements inside of
        // 'arrangement', so that they represent a new and unique permutation of
        // the elements 1-9.  The function will do all of the reordering for us,
        // and the boolean return value will indicate whether or not there are more
        // values to consider.
        //
        // By repeatedly calling this function in a loop, we end up iterating over
        // all of the possible permutations of the numbers 1-9, and hence all magic
        // square candidates which use each of the numbers 1-9 only once.
        bool another = std::next_permutation(arrangement.begin(), arrangement.end());
        if (!another) {
            break;
        } else {
            ++n;
            // std::cout << "Found a solution: ";
            // for (auto i : arrangement) {
            //     std::cout << i << " ";
            // }
            // std::cout << std::endl;
        }
    }
    // 9! = 362880
    EXPECT_EQ(362879, n);
}
