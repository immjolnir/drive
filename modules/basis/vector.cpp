#include <algorithm>  // sort
#include <numeric>    // std::accumulate
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
    std::vector<float> vec = std::vector<float>{5, 0.0f};
    EXPECT_EQ(2, vec.size());
}

TEST(vector, get_index_of_maximum_element) {
    // https://stackoverflow.com/questions/73448032/finding-the-maximum-index-of-the-biggest-element-of-an-array-vector

    std::vector<float> vec = {0.042816, 0.901856, 0.630371, 0.529785, 0.091736};
    EXPECT_EQ(5, vec.size());
    auto it = std::max_element(vec.begin(), vec.end());
    EXPECT_TRUE(it != vec.end());
    int the_index = std::distance(vec.begin(), it);
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