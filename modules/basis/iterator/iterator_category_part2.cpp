#include <gmock/gmock.h>
#include <gtest/gtest.h>



#include <forward_list>  // since c++11
#include <list>
#include <type_traits>
#include <vector>
// https://en.cppreference.com/w/cpp/iterator/iterator_traits
#include <iterator>
#include <ranges>  // since c++20

#include <algorithm>  // iter_swap

// Part 2: Practice
/**
 * Example
 * Reverse can be implemented for either Bidirectional Iterators or for Random Access Iterators, but the algorithm for
 * Random Access Iterators is more efficient. Consequently, reverse uses iterator_category to select whichever algorithm
 * is appropriate for the iterator type. This dispatch takes place at compile time, and should not incur any run-time
 * penalty.
 */
namespace internal {

template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag) {
    while (first < last) std::iter_swap(first++, --last);
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, std::bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last) {
            return;
        } else {
            std::iter_swap(first++, last);
        }
    }
}

}  // namespace internal

namespace my {
// If not puting it under a namespace, the compiler complains:
// error: call of overloaded 'reverse(std::vector<int>::iterator, std::vector<int>::iterator)' is ambiguous
// note: candidate: 'void std::reverse(_BIter, _BIter) [with _BIter = std::_List_iterator<int>]'
// /usr/include/c++/11/bits/stl_algo.h:1145:5
// Even the std namespace is not imported.

template <class Iter>
inline void reverse(Iter first, Iter last) {
    typedef typename std::iterator_traits<Iter>::iterator_category Category;
    internal::__reverse(first, last, Category());
}
}  // namespace my

namespace std2 {

// Similar to https://en.cppreference.com/w/cpp/algorithm/reverse
//
template <class BidirIt>
constexpr  // since C++20
  void
  reverse(BidirIt first, BidirIt last) {
    using iter_cat = typename std::iterator_traits<BidirIt>::iterator_category;

    // Tag dispatch, e.g. calling reverse_impl(first, last, iter_cat()),
    // can be used in C++14 and earlier modes.
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, iter_cat>) {
        if (first == last) return;

        for (--last; first < last; (void)++first, --last) std::iter_swap(first, last);
    } else
        while (first != last && first != --last) std::iter_swap(first++, last);
}
}  // namespace std2

TEST(iterator_category, my_reverse) {
    // int a[]{1, 2, 3, 4, 5};
    std::vector<int> data{1, 2, 3, 4, 5};
    my::reverse(data.begin(), data.end());
    EXPECT_THAT(data, testing::ElementsAre(5, 4, 3, 2, 1));
}

TEST(iterator_category, std2_reverse) {
    // int a[]{1, 2, 3, 4, 5};
    std::vector<int> data{1, 2, 3, 4, 5};
    std2::reverse(data.begin(), data.end());
    EXPECT_THAT(data, testing::ElementsAre(5, 4, 3, 2, 1));
}

// Continue using my functions
TEST(iterator_category, reverse_on_vector) {
    // int a[]{1, 2, 3, 4, 5};
    std::vector<int> data{1, 2, 3, 4, 5};
    my::reverse(data.begin(), data.end());
    EXPECT_THAT(data, testing::ElementsAre(5, 4, 3, 2, 1));
}

TEST(iterator_category, reverse_on_list) {
    // int a[]{1, 2, 3, 4, 5};
    std::list<int> data{1, 2, 3, 4, 5};
    my::reverse(data.begin(), data.end());
    EXPECT_THAT(data, testing::ElementsAre(5, 4, 3, 2, 1));
}

TEST(iterator_category, reverse_on_raw_array) {
    int data[]{1, 2, 3, 4, 5};
    my::reverse(std::begin(data), std::end(data));
    EXPECT_THAT(data, testing::ElementsAre(5, 4, 3, 2, 1));
}

// https://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-beginning
TEST(reverse, range_view) {
    std::vector<int> const vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    {
        // Since c++14
        std::vector<int> v0(vec.rbegin(), vec.rend());
        EXPECT_THAT(v0, testing::ElementsAre(10, 9, 8, 7, 6, 5, 4, 3, 2, 1));
    }

    //  std::ranges::reverse_view since c++20
    {
        auto view = std::ranges::views::reverse(vec);
        std::vector<int> v0(view.begin(), view.end());
        EXPECT_THAT(v0, testing::ElementsAre(10, 9, 8, 7, 6, 5, 4, 3, 2, 1));
    }

    {
        auto view = vec | std::ranges::views::reverse;
        std::vector<int> v0(view.begin(), view.end());
        EXPECT_THAT(v0, testing::ElementsAre(10, 9, 8, 7, 6, 5, 4, 3, 2, 1));
    }

    for (auto& i : std::ranges::views::reverse(vec)) {
        std::cout << i << ",";
    }
    std::cout << std::endl;

    for (auto& i : vec | std::ranges::views::reverse) {
        std::cout << i << ";";
    }
    std::cout << std::endl;
}