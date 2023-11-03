#include <gmock/gmock.h>
#include <gtest/gtest.h>

/**
 * https://en.cppreference.com/w/cpp/iterator/iterator_tags
 * <iterator>

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};

input_iterator_tag
   |_ forward_iterator_tag
          |_ bidirectional_iterator_tag
               |_ random_access_iterator_tag
                     |_ contiguous_iterator_tag

output_iterator_tag
*/
/**
 * Iterator_category is an iterator tag function: it is used to determine the category to which an iterator belongs.
 * Specifically, every iterator must belong to a type that is a model of the concept
 * - Output Iterator
 * - Input Iterator
 * - Forward Iterator
 * - Bidirectional Iterator
 * - Random Access Iterator
 *
 */

#include <list>
#include <type_traits>
#include <vector>
// https://en.cppreference.com/w/cpp/iterator/iterator_traits
#include <iterator>

#include <algorithm>  // iter_swap

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

/*
// error: too many template-parameter-lists
template <typename ExpectedType>
template <typename Iter>
bool check_container_iterator(Iter iter) {
    return std::is_same_v<std::iterator_traits<Iter::iterator>::iterator_category, ExpectedType>;
}
*/

template <typename ExpectedType, typename Iter>
bool check_container_iterator(Iter iter) {
    typedef typename std::iterator_traits<Iter>::iterator_category Category;
    return std::is_same_v<Category, ExpectedType>;
}

TEST(iterator_category, check_type) {
    std::vector<int> v{1, 2};
    //  In instantiation of 'bool check_container_iterator(Iter) [with ExpectedType = std::random_access_iterator_tag;
    //  Iter = __gnu_cxx::__normal_iterator<int*, std::vector<int> >]':
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));

    // 1. vector
    constexpr bool vector_is_random =
      std::is_same_v<std::iterator_traits<std::vector<int>::iterator>::iterator_category,
                     std::random_access_iterator_tag>;
    EXPECT_TRUE(vector_is_random);

    constexpr bool vector_is_bidirectional =
      std::is_same_v<std::iterator_traits<std::vector<int>::iterator>::iterator_category,
                     std::bidirectional_iterator_tag>;
    EXPECT_FALSE(vector_is_bidirectional);

    // 2. list
    constexpr bool list_is_random = std::is_same_v<std::iterator_traits<std::list<int>::iterator>::iterator_category,
                                                   std::random_access_iterator_tag>;
    EXPECT_FALSE(list_is_random);

    constexpr bool list_is_bidirectional =
      std::is_same_v<std::iterator_traits<std::list<int>::iterator>::iterator_category,
                     std::bidirectional_iterator_tag>;
    EXPECT_TRUE(list_is_bidirectional);

    // 3. raw array
    int data[]{1, 2, 3, 4, 5};
    // std::begin(data) error: 'decltype' evaluates to 'int*', which is not a class or enumeration type
}

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
