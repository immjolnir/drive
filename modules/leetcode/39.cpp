/*
 * https://leetcode.cn/problems/combination-sum/
 *
39, 题目描述

给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。
并以列表形式返回，你可以按 任意顺序 返回这些组合。


candidates 中的 同一个 数字可以 无限制重复被选取 。如果至少一个数字的被选数量不同，则两种组合是不同的。

对于给定的输入，保证和为 target 的不同组合数少于 150 个。


示例 1：

输入：candidates = [2,3,6,7], target = 7
输出：[[2,2,3],[7]]
解释：
2 和 3 可以形成一组候选，2 + 2 + 3 = 7 。注意 2 可以使用多次。
7 也是一个候选， 7 = 7 。
仅有这两种组合。


示例 2：

输入: candidates = [2,3,5], target = 8
输出: [[2,2,2,2],[2,3,3],[3,5]]

示例 3：
输入: candidates = [2], target = 1
输出: []


提示：

1 <= candidates.length <= 30
2 <= candidates[i] <= 40
candidates 的所有元素 互不相同
1 <= target <= 40


class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {

    }
};
*/

#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

namespace tr1 {
class Solution {
  public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        std::transform(std::begin(candidates), std::end(candidates), std::inserter(_candidates, _candidates.end()),
                       [](int c) { return std::make_pair(c, true); });
        vector<int> res;
        for (int c : candidates) {
            if (c > target) {
                return res;
            } else {
                res.empalce_back(c);
                int other = target - c;
                vector<int> tmp = combinationSum(candidates, target);
                // merge them
                std::copy(tmp.begin(), tmp.end(), std::back_inserter(res));
            }
        }
    }

    vector<vector<int>> combination_recursive(vector<int>& candidates, int c, int target) {
        vector<int> res;
        if (c > target) {
            return res;
        } else {
            res.empalce_back(c);
            int other = target - c;
            vector<int> tmp = combinationSum(candidates, target);
            // merge them
            std::copy(tmp.begin(), tmp.end(), std::back_inserter(res));
        }
    }

  private:
    std::map<int, bool> _candidates;
};

TEST(leetcode_39, tr1_case1) {
    Solution sol;
    auto res = sol.combinationSum({2, 3, 6, 7}, 7);
    EXPECT_EQ(2, res.size());
}

TEST(leetcode_39, tr1_case2) {
    Solution sol;
    auto res = sol.combinationSum({2, 3, 5}, 8);
    EXPECT_EQ(3, res.size());
}

TEST(leetcode_39, tr1_case3) {
    Solution sol;
    auto res = sol.combinationSum({2}, 1);
    EXPECT_EQ(0, res.size());
}

}  // namespace tr1

namespace tr1 {

class Solution {
  public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {}
};
}  // namespace tr1
