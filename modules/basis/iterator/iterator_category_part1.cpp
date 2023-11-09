#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <forward_list>  // since c++11
#include <list>
#include <type_traits>
#include <vector>
// https://en.cppreference.com/w/cpp/iterator/iterator_traits
#include <algorithm>  // iter_swap
#include <iterator>
#include <ranges>  // since c++20

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

    // 模板匹配的时候，base class 也能匹配成功。即母板匹配逻辑时：相同类型（本类） > 可转换类型（父类）
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 2);

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
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(std::begin(v), std::end(v)), 5);

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
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 5);

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
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 2);

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
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 2);

    // cbegin:
    //      Returns a read-only (constant) iterator that points to the first element in the %list.
    //      Iteration is done in ordinary element order.
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.cbegin()));
    EXPECT_FALSE(check_container_iterator<std::bidirectional_iterator_tag>(v.cbegin()));
    EXPECT_TRUE(check_container_iterator<std::forward_iterator_tag>(v.cbegin()));

    // rbegin:
    //      forward_list doesn't have it.
}

// TODO: what kinds container are not forward_iterator_tag?
TEST(iterator_category, set) {
    std::set<int> v{1, 1, 2, 3};
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_TRUE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_FALSE(check_container_iterator<std::forward_iterator_tag>(v.begin()));

    // 模板匹配时，会沿着 相同类型 -> 直接父类 -> super parent 的继承路线来匹配模板类
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 3);
}

TEST(iterator_category, map) {
    std::map<char, int> v{{'a', 1}, {'b', 2}, {'c', 3}};
    EXPECT_FALSE(check_container_iterator<std::random_access_iterator_tag>(v.begin()));
    EXPECT_TRUE(check_container_iterator<std::bidirectional_iterator_tag>(v.begin()));
    EXPECT_FALSE(check_container_iterator<std::forward_iterator_tag>(v.begin()));

    // 模板匹配时，会沿着 相同类型 -> 直接父类 -> super parent 的继承路线来匹配模板类
    EXPECT_EQ(google::protobuf::internal::CalculateReserve(v.begin(), v.end()), 3);
}

// So, I cannot found a container that is not the child of forward_iterator_tag.