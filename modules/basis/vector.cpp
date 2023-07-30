#include <algorithm>  // sort
#include <numeric>    // std::accumulate
#include <string>
#include <vector>

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
