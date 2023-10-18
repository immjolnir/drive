/* 31. Next Permutation

Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.

If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).

The replacement must be in-place and use only constant extra memory.

Here are some examples.
Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.

1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>  // for next_permutation
#include <vector>

bool my_next_permutation(std::vector<int>& a) {
    if (a.empty() || a.size() == 1) {
        // Return false when it is empty or only has 1 element.
        return false;
    }

    // 1. Found the lagest index k such that a[k] < a[k+1].
    int k1 = a.size() - 1;  // a+1
    int k;
    while (k1 > 0) {
        k = k1 - 1;
        if (a[k] < a[k1]) {
            // 2. Find the largest l greater than k such that a[k] < a[l].
            int l = a.size() - 1;
            while (l > 0) {
                if (a[l] > a[k]) break;  // Found it;
                --l;
            }
            // At least l equals k + 1, but sometime it is greater than that.

            // 3. Swap the vale of a[k] with that of a[l].
            std::swap(a[k], a[l]);

            // 4. Reverse the squence from a[k+1] to the last.
            int last = a.size() - 1;
            int i = k + 1;
            while (i < last) {
                std::swap(a[i++], a[last--]);
            }
            return true;
        }
        --k1;
    }

    return false;  // when k1 == 0
}

// Solutions
// https://www.joshuachou.ink/lc31.next_permutation/
namespace std_internal
{
#if __cplusplus >= 202002L
#define _GLIBCXX20_CONSTEXPR constexpr
#else
#define _GLIBCXX20_CONSTEXPR
#endif

    template <typename _BidirectionalIterator>
    _GLIBCXX20_CONSTEXPR void __reverse(_BidirectionalIterator __first, _BidirectionalIterator __last) {
        while (true)
            if (__first == __last || __first == --__last)
                return;
            else {
                std::iter_swap(__first, __last);
                ++__first;
            }
    }

    struct _Iter_less_iter {
        template <typename _Iterator1, typename _Iterator2>
        bool operator()(_Iterator1 __it1, _Iterator2 __it2) const {
            return *__it1 < *__it2;
        }
    };

    _GLIBCXX20_CONSTEXPR
    inline _Iter_less_iter __iter_less_iter() { return _Iter_less_iter(); }

    template <typename _BidirectionalIterator, typename _Compare>
    _GLIBCXX20_CONSTEXPR bool next_permutation_impl(_BidirectionalIterator __first, _BidirectionalIterator __last,
                                                    _Compare __comp) {
        if (__first == __last) return false;  // empty container
        _BidirectionalIterator __i = __first;
        ++__i;
        if (__i == __last) return false;  // only 1 element.

        __i = __last;
        --__i;

        for (;;) {
            _BidirectionalIterator __ii = __i;
            --__i;
            if (__comp(__i, __ii)) {
                // __ii is the k1 and the __i is the k
                // 1. Find the largest index k such that a[k] < a[k+1].
                _BidirectionalIterator __j = __last;
                while (!__comp(__i, --__j)) {
                    // __j is the l
                    // 2. Find the largest index l greater than k such that a[k] < a[l].
                }
                // __i is the k, __j is the l.
                // 3. Swap the evalue of a[k] with that of a[l]
                std::iter_swap(__i, __j);
                // 4. Reverse the sequence from a[k+1] up to and including the final element a[n].
                __reverse(__ii, __last);
                return true;
            }

            if (__i == __first) {
                __reverse(__first, __last);  // No need to reverse again.
                return false;
            }
        }
    }

    template <typename _BidirectionalIterator>
    _GLIBCXX20_CONSTEXPR inline bool next_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last) {
        // range checks
        return next_permutation_impl(__first, __last, __iter_less_iter());
    }
}  // namespace std_internal

TEST(permutation, std_next_permutation_print0) {
    std::vector<int> arr = {1, 2, 3};
    do {
        for (auto i : arr) {
            std::cout << i << " ";
            /*
            1 2 3
            1 3 2
            2 1 3
            2 3 1
            3 1 2
            3 2 1
            */
        }
        std::cout << std::endl;
    } while (std::next_permutation(arr.begin(), arr.end()));
}

TEST(permutation, std_next_permutation) {
    std::vector<int> arr = {1, 2, 3};

    bool succ = std::next_permutation(arr.begin(), arr.end());
    EXPECT_TRUE(succ);

    // Comparing the std::vector
    // const std::vector<int> expected = {1, 2, 3};
    // for (int i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(expected[i], arr[i]) << "Vectors x and y differ at index " << i;
    // }

    // https://github.com/google/googletest/issues/817
    EXPECT_THAT(arr, testing::ElementsAre(1, 3, 2));

    // Continue...
    EXPECT_TRUE(std::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(2, 1, 3));

    EXPECT_TRUE(std::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(2, 3, 1));

    EXPECT_TRUE(std::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(3, 1, 2));

    EXPECT_TRUE(std::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(3, 2, 1));

    EXPECT_FALSE(std::next_permutation(arr.begin(), arr.end()));
}

TEST(permutation, std_internal_next_permutation) {
    std::vector<int> arr = {1, 2, 3};

    bool succ = std_internal::next_permutation(arr.begin(), arr.end());
    EXPECT_TRUE(succ);

    // Comparing the std::vector
    // const std::vector<int> expected = {1, 2, 3};
    // for (int i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(expected[i], arr[i]) << "Vectors x and y differ at index " << i;
    // }

    // https://github.com/google/googletest/issues/817
    EXPECT_THAT(arr, testing::ElementsAre(1, 3, 2));

    // Continue...
    EXPECT_TRUE(std_internal::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(2, 1, 3));

    EXPECT_TRUE(std_internal::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(2, 3, 1));

    EXPECT_TRUE(std_internal::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(3, 1, 2));

    EXPECT_TRUE(std_internal::next_permutation(arr.begin(), arr.end()));
    EXPECT_THAT(arr, testing::ElementsAre(3, 2, 1));

    EXPECT_FALSE(std_internal::next_permutation(arr.begin(), arr.end()));
}

TEST(permutation, my_next_permutation) {
    std::vector<int> arr = {1, 2, 3};

    bool succ = my_next_permutation(arr);
    EXPECT_TRUE(succ);

    // Comparing the std::vector
    // const std::vector<int> expected = {1, 2, 3};
    // for (int i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(expected[i], arr[i]) << "Vectors x and y differ at index " << i;
    // }

    // https://github.com/google/googletest/issues/817
    EXPECT_THAT(arr, testing::ElementsAre(1, 3, 2));

    // Continue...
    EXPECT_TRUE(my_next_permutation(arr));
    EXPECT_THAT(arr, testing::ElementsAre(2, 1, 3));

    EXPECT_TRUE(my_next_permutation(arr));
    EXPECT_THAT(arr, testing::ElementsAre(2, 3, 1));

    EXPECT_TRUE(my_next_permutation(arr));
    EXPECT_THAT(arr, testing::ElementsAre(3, 1, 2));

    EXPECT_TRUE(my_next_permutation(arr));
    EXPECT_THAT(arr, testing::ElementsAre(3, 2, 1));

    EXPECT_FALSE(my_next_permutation(arr));
}

// Skip the checking on return value
TEST(permutation, std_solution) {
    {
        // 1,2,3 -> 1,3,2
        std::vector<int> arr = {1, 2, 3};
        std::next_permutation(arr.begin(), arr.end());
        EXPECT_THAT(arr, testing::ElementsAre(1, 3, 2));
    }

    {
        // 3,2,1 -> 1,2,3
        std::vector<int> arr = {3, 2, 1};
        std::next_permutation(arr.begin(), arr.end());
        EXPECT_THAT(arr, testing::ElementsAre(1, 2, 3));
    }
    {
        // 1,1,5 -> 1,5,1
        std::vector<int> arr = {1, 1, 5};
        std::next_permutation(arr.begin(), arr.end());
        EXPECT_THAT(arr, testing::ElementsAre(1, 5, 1));
    }
}

void solution(std::vector<int>& a) {
    if (a.empty() || a.size() == 1) {
        // Return false when it is empty or only has 1 element.
        return;
    }

    // 1. Found the lagest index k such that a[k] < a[k+1].
    int k1 = a.size() - 1;  // a+1
    int k;
    while (k1 > 0) {
        k = k1 - 1;
        if (a[k] < a[k1]) {
            // 2. Find the largest l greater than k such that a[k] < a[l].
            int l = a.size() - 1;
            while (l > 0) {
                if (a[l] > a[k]) break;  // Found it;
                --l;
            }
            // At least l equals k + 1, but sometime it is greater than that.

            // 3. Swap the vale of a[k] with that of a[l].
            std::swap(a[k], a[l]);

            // 4. Reverse the squence from a[k+1] to the last.
            int last = a.size() - 1;
            int i = k + 1;
            while (i < last) {
                std::swap(a[i++], a[last--]);
            }
            return;
        }
        --k1;
    }
    // k1 == 0 means a[0] is the biggest
    // Reverses the order of the elements in the range
    std::reverse(a.begin(), a.end());
}

TEST(permutation, my_solution) {
    {
        // 1,2,3 -> 1,3,2
        std::vector<int> arr = {1, 2, 3};
        solution(arr);
        EXPECT_THAT(arr, testing::ElementsAre(1, 3, 2));
    }

    {
        // 3,2,1 -> 1,2,3
        std::vector<int> arr = {3, 2, 1};
        solution(arr);
        EXPECT_THAT(arr, testing::ElementsAre(1, 2, 3));
    }
    {
        // 1,1,5 -> 1,5,1
        std::vector<int> arr = {1, 1, 5};
        solution(arr);
        EXPECT_THAT(arr, testing::ElementsAre(1, 5, 1));
    }
}