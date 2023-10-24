#include <gtest/gtest.h>
#include <algorithm>
#include <map>
#include <vector>

/*
 https://en.cppreference.com/w/cpp/algorithm/min_element

constexpr ForwardIt min_element(ForwardIt first, ForwardIt last);
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp);

comp - comparison function object (i.e. an object that satisfies the requirements of Compare) which returns true if a is
less than b. The signature of the comparison function should be equivalent to the following: bool cmp(const Type1& a,
const Type2& b);

Return value:
    Iterator to the smallest element in the range [first, last). If several elements in the range are equivalent to the
smallest element, returns the iterator to the first such element. Returns last if the range is empty.

Possible implementation:

template <class ForwardIt, class Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) return last;

    ForwardIt smallest = first;
    ++first;

    for (; first != last; ++first)
        if (comp(*first, *smallest)) smallest = first;

    return smallest;
}
*/

TEST(min_element, vector_smallest) {
    std::vector<int> v{3, 1, -4, 1, 5, 9};

    std::vector<int>::iterator result = std::min_element(v.begin(), v.end());
    EXPECT_EQ(-4, *result);
    EXPECT_EQ(2, std::distance(v.begin(), result));
}

TEST(min_element, vector_smallest_with_compare_func) {
    std::vector<int> v{3, 1, -4, 1, 5, 9};

    std::vector<int>::iterator result = std::min_element(
      v.begin(), v.end(), [](std::vector<int>::value_type a, std::vector<int>::value_type b) { return a < b; });

    EXPECT_EQ(-4, *result);
    EXPECT_EQ(2, std::distance(v.begin(), result));
}

TEST(min_element, vector_smallest_with_compare_func2) {
    std::vector<int> v{3, 1, -4, 1, 5, 9};

    // Different lambda expression
    std::vector<int>::iterator result = std::min_element(v.begin(), v.end(), [](auto& a, auto& b) { return a < b; });

    EXPECT_EQ(-4, *result);
    EXPECT_EQ(2, std::distance(v.begin(), result));
}

TEST(min_element, vector_closest_to_a_number) {
    std::vector<int> v{3, 1, -4, 1, 5, 9};

    int number = 0;

    // Different lambda expression
    std::vector<int>::iterator result = std::min_element(v.begin(), v.end(), [&number](auto& a, auto& b) {
        auto a_diff = std::abs(a - number);
        auto b_diff = std::abs(b - number);
        return a_diff < b_diff;
    });

    EXPECT_EQ(1, *result);
    /*
    3 1 -4 1 5 9
      ^
    */
    EXPECT_EQ(1, std::distance(v.begin(), result));

    ++result;
    /*
    3 1 -4 1 5 9
        ^
    */
    EXPECT_EQ(-4, *result);
}

// For non-empty containers, it can always return an iterator