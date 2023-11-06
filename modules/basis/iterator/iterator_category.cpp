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
 *   - Increment and decrement are inverses of each other:
 *      - If a is incrementable and bool(a == b), then bool(--(++a) == b).
 *      - If a is decrementable and bool(a == b), then bool(++(--a) == b).
 * - Random Access Iterator
 *
 */

#include <forward_list>  // since c++11
#include <list>
#include <type_traits>
#include <vector>
// https://en.cppreference.com/w/cpp/iterator/iterator_traits
#include <iterator>

#include <algorithm>  // iter_swap

// Part 1: familar with the container's iterator
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

namespace google::protobuf::internal {
// src/google/protobuf/repeated_field.h

// 注释掉本类
// template <typename Iter>
// inline int CalculateReserve(Iter begin, Iter end, std::random_access_iterator_tag) {
//     return -2;
// }

template <typename Iter>
inline int CalculateReserve(Iter begin, Iter end, std::forward_iterator_tag) {
    return static_cast<int>(std::distance(begin, end));
}

template <typename Iter>
inline int CalculateReserve(Iter /*begin*/, Iter /*end*/, std::input_iterator_tag /*unused*/) {
    return -1;
}

template <typename Iter>
inline int CalculateReserve(Iter begin, Iter end) {
    typedef typename std::iterator_traits<Iter>::iterator_category Category;
    return CalculateReserve(begin, end, Category());
}
}  // namespace google::protobuf::internal

TEST(iterator_category, vector_iterator_type) {
    // vector is random_access_iterator_tag not bidirectional_iterator_tag
    std::vector<int> v{1, 2};
    //  In instantiation of 'bool check_container_iterator(Iter) [with ExpectedType = std::random_access_iterator_tag;
    //  Iter = __gnu_cxx::__normal_iterator<int*, std::vector<int> >]':
    EXPECT_FALSE(check_container_iterator<std::forward_iterator_tag>(v.begin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_GT(google::protobuf::internal::CalculateReserve(v.begin(), v.end()),
              -1);  // 模板匹配的时候，base class 也能匹配成功。即母板匹配逻辑时：相同类型（本类） > 可转换类型（父类）

    // cbegin:
    //    Returns a read-only (constant) iterator that points to the first element in the %vector.
    //    Iteration is done in ordinary element order.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.cbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.cbegin()));

    // rbegin:
    //   Returns a read/write reverse iterator that points to the last element in the %vector.
    //   Iteration is done in reverse element order.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.rbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.rbegin()));

    // rcbegin:
    //
}

TEST(iterator_category, raw_array_iterator_type) {
    // raw array
    int v[]{1, 2, 3, 4, 5};
    // std::begin(data) error: 'decltype' evaluates to 'int*', which is not a class or enumeration type
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(std::begin(v)));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(std::begin(v)));
    EXPECT_GT(google::protobuf::internal::CalculateReserve(std::begin(v), std::end(v)), -1);

    // cbegin:
    //      Returns a read-only (constant) iterator that points to the first element in the %list.
    //      Iteration is done in ordinary element order.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(std::cbegin(v)));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(std::cbegin(v)));

    // rbegin:
    //       Return a reverse iterator pointing to the last element of the array.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(std::rbegin(v)));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(std::rbegin(v)));

    // Use std::array instead the raw array in c++.
}

TEST(iterator_category, std_array_iterator_type) {
    // Use std::array instead the raw array in c++.
    std::array<int, 5> v{1, 2, 3, 4, 5};

    // std::begin(data) error: 'decltype' evaluates to 'int*', which is not a class or enumeration type
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_GT(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), -1);

    // cbegin:
    //      Returns a read-only (constant) iterator that points to the first element in the %list.
    //      Iteration is done in ordinary element order.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.cbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.cbegin()));

    // rbegin:
    //       Return a reverse iterator pointing to the last element of the array.
    EXPECT_TRUE(check_container_iterator<std::random_access_iterator_tag>(v.rbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.rbegin()));
    // rcbegin:
    //
}

// List is bidirectional_iterator instead of random_access_iterator
/**
 *  /usr/include/c++/12/bits/stl_list.h
   *  @brief A list::iterator.
   *
   *  All the functions are op overloads.
  template<typename _Tp>
    struct _List_iterator
    {
      typedef _List_iterator<_Tp>		_Self;
      typedef _List_node<_Tp>			_Node;

      typedef ptrdiff_t				difference_type;
      typedef std::bidirectional_iterator_tag	iterator_category; // hardcoded iterator tag.
      typedef _Tp				value_type;
      typedef _Tp*				pointer;
      typedef _Tp&				reference;

 */
TEST(iterator_category, list_iterator_type) {
    std::list<int> v{1, 2};
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_TRUE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_GT(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), -1);

    // cbegin:
    //      Returns a read-only (constant) iterator that points to the first element in the %list.
    //      Iteration is done in ordinary element order.
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.cbegin()));
    EXPECT_TRUE(check_container_iterator<std::bidirectional_iterator_tag>(v.cbegin()));

    // rbegin:
    //      Returns a read/write reverse iterator that points to the last element in the %list.
    //      Iteration is done in reverse element order.
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.rbegin()));
    EXPECT_TRUE(check_container_iterator<std::bidirectional_iterator_tag>(v.rbegin()));
}

// std::forward_list is forward_iterator only.
/**
 * /usr/include/c++/12/bits/forward_list.h
 template<typename _Tp>
    struct _Fwd_list_iterator
    {
      typedef _Fwd_list_iterator<_Tp>		_Self;
      typedef _Fwd_list_node<_Tp>		_Node;

      typedef _Tp				value_type;
      typedef _Tp*				pointer;
      typedef _Tp&				reference;
      typedef ptrdiff_t				difference_type;
      typedef std::forward_iterator_tag		iterator_category; // hardcoded iterator tag.
*/
TEST(iterator_category, forward_list_iterator_type) {
    std::forward_list<int> v{1, 2};
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_TRUE(check_container_iterator<std::forward_iterator_tag>(v.begin()));
    EXPECT_GT(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), -1);

    // cbegin:
    //      Returns a read-only (constant) iterator that points to the first element in the %list.
    //      Iteration is done in ordinary element order.
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.cbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.cbegin()));
    EXPECT_TRUE(check_container_iterator<std::forward_iterator_tag>(v.cbegin()));

    // rbegin:
    //      forward_list doesn't have it.
}

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
