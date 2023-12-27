#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

std::string longestPalindrome(std::string s) {
    size_t n = s.size();
    if (n < 2) return s;

    std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));
    size_t start = 0, max_len = 1;

    for (size_t i = 0; i < n; ++i) {
        dp[i][i] = true;
    }

    for (size_t k = 1; k < n; ++k) {
        for (size_t i = 0; i + k < n; ++i) {
            size_t j = i + k;
            if (s[i] == s[j]) {
                if (k == 1 || dp[i + 1][j - 1]) {
                    dp[i][j] = true;
                    if (k + 1 > max_len) {
                        start = i;
                        max_len = k + 1;
                    }
                }
            }
        }
    }

    return s.substr(start, max_len);
}

TEST(palindromic, exa1) {
    std::string input = "abc";
    std::string result = longestPalindrome(input);
    EXPECT_EQ("a", result);
}

TEST(palindromic, exa2) {
    std::string input = "abcba";
    std::string result = longestPalindrome(input);
    std::cout << "The longest palindrome substring is: " << result << std::endl;
    EXPECT_EQ("abcba", result);
}

TEST(palindromic, exa3) {
    std::string input = "abcaba";
    std::string result = longestPalindrome(input);
    std::cout << "The longest palindrome substring is: " << result << std::endl;
    EXPECT_EQ("aba", result);
}