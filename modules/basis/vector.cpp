#include <algorithm>  // sort
#include <list>       // for push_front
#include <numeric>    // std::accumulate
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

/**
 * Vector

- insert

*/
using namespace std;

template <typename T>
std::string join(std::vector<T> const& vec, std::string delim) {
    if (vec.empty()) {
        return std::string();
    }
    /*
        return std::accumulate(vec.begin() + 1, vec.end(), std::to_string(vec[0]),
                    [](const std::string& a, T b){
                        return a + ", " + std::to_string(b);
                    });
                    */
    std::string res(vec[0]);
    return std::accumulate(vec.begin() + 1, vec.end(), res,
                           [&delim](const std::string& a, T b) { return a + delim + b; });
}

TEST(vector, stringify) {
    std::vector<std::string> vec{"6", "3", "8", "-9", "1", "-2", "8"};
    std::string delim = ",";
    EXPECT_EQ("6,3,8,-9,1,-2,8", join(vec, delim));
}

TEST(vector, sort_vector) {
    std::vector<int> arr = {1, 5, 7, 2, 4};
    std::sort(arr.begin(), arr.end());
    EXPECT_EQ(std::vector<int>({1, 2, 4, 5, 7}), arr);
}

TEST(vector, sort_empty_vector) {
    std::vector<int> arr;
    std::sort(arr.begin(), arr.end());  // sort 可以 work在空的 array 上
    EXPECT_TRUE(arr.empty());
}

TEST(vector, ctor) {
    // use the ctor:
    // vector( size_type count, const T& value,
    std::vector<float> vec = std::vector<float>(5, 0.0f);
    EXPECT_EQ(5, vec.size());
}

TEST(vector, ctor_with_initializer_list) {
    // use the ctor:
    // constexpr vector(std::initializer_list<T> init,
    std::vector<float> vec{5, 0.0f};
    // Same as
    // std::vector<float> vec = std::vector<float>{5, 0.0f};
    EXPECT_EQ(2, vec.size());
}

// Call vector's ctor
/**
 *  @brief  Creates a %vector with copies of an exemplar element.
 *  @param  __n  The number of elements to initially create.
 *  @param  __value  An element to copy.
 *  @param  __a  An allocator.
 *
 *  This constructor fills the %vector with @a __n copies of @a __value.
 */
// vector(size_type __n, const value_type& __value, const allocator_type& __a = allocator_type())
TEST(vector, ctor_with_n_default_value) {
    std::vector<float> vec(5, 0.0f);
    EXPECT_EQ(5, vec.size());
    EXPECT_THAT(vec, testing::ElementsAre(0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
}

TEST(vector, get_index_of_maximum_element) {
    // https://stackoverflow.com/questions/73448032/finding-the-maximum-index-of-the-biggest-element-of-an-array-vector

    std::vector<float> vec = {0.042816, 0.901856, 0.630371, 0.529785, 0.091736};
    EXPECT_EQ(5, vec.size());
    auto it = std::max_element(vec.begin(), vec.end());
    EXPECT_TRUE(it != vec.end());

    // error: conversion from 'std::__iterator_traits<__gnu_cxx::__normal_iterator<float*, std::vector<float> >,
    // void>::difference_type' {aka 'long int'} to 'int' may change value [-Werror=conversion]
    long int the_index = std::distance(vec.begin(), it);
    EXPECT_EQ(1, the_index);
    EXPECT_NEAR(0.901856, *it, 1e-6);
}

TEST(vector, erase_duplicate_element) {
    // https://en.cppreference.com/w/cpp/algorithm/unique
    // a vector containing several duplicate elements
    std::vector<int> vec{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
    std::sort(vec.begin(), vec.end());

    // remove consecutive (adjacent) duplicates
    auto last = std::unique(vec.begin(), vec.end());

    vec.erase(last, vec.end());
    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3, 4, 5));
}

// template <typename T, template <template, template = std::allocator<T>> class CONTAINER>
template <typename T>
std::vector<T> unique(const std::vector<T>& in) {
    // std::unordered_set<T> out(std::begin(in), std::end(in));
    // use std::set to keep the output's order.
    // std::set is used so that the order of the elements of the returned array does not change each time
    std::set<T> out(std::begin(in), std::end(in));
    return {std::begin(out), std::end(out)};
}

TEST(vector, erase_duplicate_element_best_way) {
    // https://www.reddit.com/r/cpp_questions/comments/bjims8/the_best_way_to_remove_duplicates_from_vector_of/
    std::vector<int> vec{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
    auto unique_vec = unique(vec);
    EXPECT_THAT(unique_vec, testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(vector, back_insert_vector) {
    std::vector<int> a{1, 2, 3}, b{4, 5};
    // Copy the contents of b into a.
    std::copy(b.begin(), b.end(), std::back_inserter(a));
    EXPECT_THAT(a, testing::ElementsAre(1, 2, 3, 4, 5));
}

// constexpr iterator insert( const_iterator pos, InputIt first, InputIt last );
TEST(vector, back_insert_vector2) {
    std::vector<int> a{1, 2, 3}, b{4, 5};
    // Copy the contents of b into a.
    a.insert(a.end(), b.begin(), b.end());
    EXPECT_THAT(a, testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(vector, front_insert_vector) {
    // std::vector<int> a{1, 2, 3}, b{4, 5};
    // Copy the contents of b into a.
    // /usr/include/c++/11/bits/stl_iterator.h:794:20:
    // error: 'class std::vector<int>' has no member named 'push_front'
    //   794 |         container->push_front(__value);
    // std::copy(b.begin(), b.end(), std::front_inserter(a));
    // std::vector doesn't have the push_front method.

    std::list<int> a{1, 2, 3}, b{4, 5};
    std::copy(b.begin(), b.end(), std::front_inserter(a));
    EXPECT_THAT(a, testing::ElementsAre(5, 4, 1, 2, 3));
}

TEST(vector, front_insert_vector2) {
    std::vector<int> a{1, 2, 3}, b{4, 5};
    a.insert(a.begin(), b.begin(), b.end());
    EXPECT_THAT(a, testing::ElementsAre(4, 5, 1, 2, 3));
}

/**
 *  @brief  Assigns a range to a %vector.
 *  @param  __first  An input iterator.
 *  @param  __last   An input iterator.
 *
 *  This function fills a %vector with copies of the elements in the
 *  range [__first,__last).
 *
 *  Note that the assignment completely changes the %vector and
 *  that the resulting %vector's size is the same as the number
 *  of elements assigned.
 */
// fill, overwrite.
TEST(vector, reserve_and_assign) {
    const std::vector<int> vec{0, 1, 2};
    std::vector<int> disparities;

    disparities.reserve(3);
    EXPECT_EQ(0, disparities.size());
    EXPECT_TRUE(disparities.empty());

    disparities.assign(vec.begin(), vec.end());
    EXPECT_EQ(3, disparities.size());
    EXPECT_THAT(disparities, testing::ElementsAre(0, 1, 2));
}

TEST(vector, resize_and_assign) {
    const std::vector<int> vec{0, 1, 2};
    std::vector<int> disparities;

    disparities.resize(3);
    EXPECT_EQ(3, disparities.size());
    EXPECT_FALSE(disparities.empty());

    disparities.assign(vec.begin(), vec.end());
    EXPECT_EQ(3, disparities.size());
    EXPECT_THAT(disparities, testing::ElementsAre(0, 1, 2));
}

// So assign will over