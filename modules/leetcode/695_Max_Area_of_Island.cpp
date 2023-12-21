/*
695 题目描述
给定一个二维的 0-1 矩阵，其中 0 表示海洋，1 表示陆地。单独的或相邻的陆地可以形成岛
屿，每个格子只与其上下左右四个格子相邻。求最大的岛屿面积。

输入输出样例
输入是一个二维数组，输出是一个整数，表示最大的岛屿面积。
Input:
[[1,0,1,1,0,1,0,1],
[1,0,1,1,0,1,1,1],
[0,0,0,0,0,0,0,1]]
Output: 6
最大的岛屿面积为 6，位于最右侧。
*/

/*
对于四个方向的遍历，可以创造一个数组 [-1, 0, 1, 0, -1]，每相邻两位即为上下左右四个方向之一。

            .----.---.---.---.----.
 direction: | -1 | 0 | 1 | 0 | -1 |
            '----'---'---'---'----'
      i：      0   1   2   3    4
 i=0, (r-1, c)
 i=1, (r, c+1)
 i=2, (r+1, c)
 i=3, (r, c-1)
 .-------.-------.-------.
 |       |  x    |       |
 .-------.-------.-------.
 |  x    | (r,c) |   x   |
 .-------.-------.-------.
 |       |  x    |       |
 '-------'-------'-------'
 */
#include <gtest/gtest.h>

#include <stack>
#include <utility>  // std::pair
#include <vector>
using namespace std;

// clang-format off
static const std::vector<std::vector<int>> input = {
    {1,0,1,1,0,1,0,1},
    {1,0,1,1,0,1,1,1},
    {0,0,0,0,0,0,0,1}
};
// clang-format on

const vector<int> direction{-1, 0, 1, 0, -1};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wconversion"

namespace tr0 {
int maxAreaOfIsland(vector<vector<int>> grid) {
    int m = grid.size(), n = m ? grid[0].size() : 0, local_area, area = 0, x, y;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j]) {
                local_area = 1;
                grid[i][j] = 0;
                stack<pair<int, int>> island;
                island.push({i, j});
                while (!island.empty()) {
                    auto [r, c] = island.top();
                    island.pop();
                    for (int k = 0; k < 4; ++k) {
                        x = r + direction[k], y = c + direction[k + 1];
                        if (x >= 0 && x < m && y >= 0 && y < n && grid[x][y] == 1) {
                            grid[x][y] = 0;
                            ++local_area;
                            island.push({x, y});
                        }
                    }
                }
                area = max(area, local_area);
            }
        }
    }
    return area;
}

TEST(max_area_of_island, tr0) {
    int area = maxAreaOfIsland(input);
    EXPECT_EQ(6, area);
}
}  // namespace tr0

/**
 * 辅函数内递归搜索时，边界条件的判定边界判定一般有两种写法。
 * * 一种是先判定是否越界，只有在合法的情况下才进行下一步搜索（即判断放在调用递归函数前）
 * * 一种是不管三七二十一先进行下一步搜索，待下一步搜索开始时再判断是否合法（即判断放在辅函数第一行）
 */
namespace tr1 {
int dfs(vector<vector<int>>& grid, int r, int c);

// 主函数
int maxAreaOfIsland(vector<vector<int>> grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int max_area = 0;
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid[0].size(); ++col) {
            if (grid[row][col] == 1) {  // 边界条件
                max_area = max(max_area, dfs(grid, row, col));
            }
        }
    }
    return max_area;
}

// 辅函数
int dfs(vector<vector<int>>& grid, int r, int c) {
    if (grid[r][c] == 0) return 0;  // 递归结束的边界条件, 遇到它就不连续了；剩下的就是数个数了

    grid[r][c] = 0;  // why set to 0?
    int area = 1;

    for (int i = 0; i < 4; ++i) {
        int neighbor_r = r + direction[i], neighbor_c = c + direction[i + 1];
        if (neighbor_r >= 0 && neighbor_r < grid.size() && neighbor_c >= 0 && neighbor_c < grid[0].size()) {
            area += dfs(grid, neighbor_r, neighbor_c);
        }
    }
    return area;
}

TEST(max_area_of_island, tr1) {
    int area = maxAreaOfIsland(input);
    EXPECT_EQ(6, area);
}
}  // namespace tr1

namespace tr2 {
int dfs(vector<vector<int>>& grid, int r, int c);

// 主函数
int maxAreaOfIsland(vector<vector<int>> grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int max_area = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            max_area = max(max_area, dfs(grid, i, j));
        }
    }
    return max_area;
}

// 辅函数
int dfs(vector<vector<int>>& grid, int r, int c) {
    if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size() || grid[r][c] == 0) {
        return 0;  // 边界条件
    }

    grid[r][c] = 0;
    return 1 + dfs(grid, r + 1, c) + dfs(grid, r - 1, c) + dfs(grid, r, c + 1) + dfs(grid, r, c - 1);
}

TEST(max_area_of_island, tr2) {
    int area = maxAreaOfIsland(input);
    EXPECT_EQ(6, area);
}
}  // namespace tr2

#pragma GCC diagnostic pop
