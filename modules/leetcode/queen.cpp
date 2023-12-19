#include <gtest/gtest.h>

// https://stackoverflow.com/questions/56978365/how-to-fix-error-call-to-abs-is-ambiguous
// #include <cmath> // <cmath> are abs(float), abs(double) and abs(long double);
#include <cstdlib>  // abs(int)
#include <iostream>

#include <chrono>

using namespace std;

namespace tr1 {
const int N = 8;  // 棋盘大小

int total = 0;

// 在棋盘board[row][col]处放置一个皇后
void place_queen(int board[N][N], int row, int col) {
    for (int i = 0; i < row; ++i) {
        if (board[i][col] == 1 || row - i == abs(col - board[i][col])) {
            return;
        }
    }
    board[row][col] = 1;
}

// 回溯算法求解八皇后问题
void eight_queens(int board[N][N], int row = 0) {
    if (row == N) {
        total++;
        // 找到了一种可能的解决方案
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        return;
    }

    for (int col = 0; col < N; ++col) {
        if (board[row][col] != 1) {
            place_queen(board, row, col);
            eight_queens(board, row + 1);
            board[row][col] = 0;  // 回溯
        }
    }
}

TEST(queen, tr1) {
    GTEST_SKIP() << "Skipping single test";
    int board[N][N] = {0};
    eight_queens(board);
    std::cout << "Found " << total << " Solutions" << std::endl;
}
}  // namespace tr1

namespace tr2 {
// https://zhuanlan.zhihu.com/p/96153100
template <size_t N = 8>
class Queen {
  public:
    /*
        .-----.-----.-----.-----.-----.
        | col | col | col | col | col |
        '-----'-----'-----'-----'-----'
    Row   0      1     2     3     4
    */
    std::array<size_t, N> rows;  // Store the column id
    static int count;
    static int judgeCount;

    bool judge(size_t n);
    void check(size_t n);
    void print();
};

template <size_t N>
int Queen<N>::count = 0;

template <size_t N>
int Queen<N>::judgeCount = 0;

template <size_t N>
bool Queen<N>::judge(size_t row) {
    judgeCount++;
    for (size_t i = 0; i < row; ++i) {
        if (rows[i] == rows[row]) {  // Col id occurrence before. Skip the same column
            return false;
        }
        // TODO: abs
        if ((row - i) == (rows[row] - rows[i])) {  // Skip the diagonal line
            return false;
        }
    }
    return true;
}

template <size_t N>
void Queen<N>::check(size_t row) {
    if (row == N) {
        // Found it
        ++count;
        print();
    } else {
        for (size_t col = 0; col < N; ++col) {
            rows[row] = col;
            if (judge(row)) {
                check(row + 1);
            }
        }
    }
}

template <size_t N>
void Queen<N>::print() {
    for (size_t i = 0; i < N; ++i) {
        std::cout << rows[i] << " ";
    }
    std::cout << std::endl;
}

TEST(queen, tr2) {
    GTEST_SKIP() << "Skipping single test";
    Queen<8> q;
    q.check(0);
    std::cout << "Total solutions: " << q.count << std::endl;
    std::cout << "Total Traversals: " << q.judgeCount << std::endl;
}
}  // namespace tr2

namespace tr3 {
static constexpr int N = 8;

int t;
char g[N][N];

//  列， 主对角线， 次对角线
bool col[N], dg[N], udg[N];

// 用dfs搜索出全部方案，然后剪去不符合题意的，最后注意一下格式就行了
void dfs(int u) {
    if (u == N) {
        if (t) cout << endl;
        // for (int i = 0; i < N; i++) {
        //     for (int j = 0; j < N; j++) {
        //         cout << g[i][j] << ' ';
        //     }
        //     cout << endl;
        // }
        t++;
        return;
    }
    for (int i = 0; i < N; i++) {
        if (!col[i] && !dg[u + i] && !udg[i - u + N])  //  剪枝
        {
            g[u][i] = 'Q';
            col[i] = dg[u + i] = udg[i - u + N] = true;
            dfs(u + 1);
            g[u][i] = '.';
            col[i] = dg[u + i] = udg[i - u + N] = false;
        }
    }
}

void init() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) g[i][j] = '.';
}

TEST(queen, tr3) {
    // GTEST_SKIP() << "Skipping single test";

    init();
    dfs(0);

    if (!t) std::cout << "None" << std::endl;
    EXPECT_EQ(92, t);
}
}  // namespace tr3

namespace tr4 {
/*
  . . Q . . . . .
  . . . . . Q . .
  . Q . . . . . .
  . . . . . . Q .
  . . . . Q . . .
  Q . . . . . . .
  . . . . . . . Q
  . . . Q . . . .

true: occupancy
false: available
*/
template <typename ParamT>
size_t AbsMinus(ParamT a, ParamT b) {
    int a_ = static_cast<int>(a);
    int b_ = static_cast<int>(b);
    int res = a_ - b_;
    return res > 0 ? (size_t)res : size_t(-res);
}

template <size_t N>
struct Boarder {
    static constexpr size_t SIZE = N;
    std::array<std::array<bool, SIZE>, SIZE> data;  // Square boarder

    Boarder() {
        for (size_t row = 0; row < SIZE; ++row) {
            for (size_t col = 0; col < SIZE; ++col) {
                data[row][col] = false;
            }
        }
    }

    void print() {
        for (size_t row = 0; row < SIZE; ++row) {
            for (size_t col = 0; col < SIZE; ++col) {
                cout << (data[row][col] ? "Q" : ".") << " ";
            }
            cout << endl;
        }
    }

    bool columnOccupied(size_t col) {
        // 因为我们是从上往下的，所以col+1必然是空的
        for (size_t row = 0; row < SIZE; ++row) {
            if (data[row][col]) return true;  // The column number is occupied.
        }
        return false;
    }

    bool slopeOccupied(size_t target_row, size_t target_col) {
        for (size_t row = 0; row < target_row; ++row) {
            for (size_t col = 0; col < SIZE; ++col) {
                if (target_row - row == AbsMinus(target_col, col) && data[row][col]) {
                    return true;
                }
            }
        }
        return false;
    }

    bool diagonalOccupied(size_t target_row, size_t target_col) {
        // 主对角线（从左上到右下）：
        // 如果要找同一主对角线上的其他点，它们的行索引和列索引之差（绝对值）是不变的。
        // 也就是说，对于每个点 (i, j) 和 (k, l) 在同一条主对角线上，有 |i - k| = |j - l|。
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
        // error: conversion to 'size_t' {aka 'long unsigned int'} from 'long int' may change the sign of the result
        long int expected_diff = std::abs((long int)target_row - (long int)target_col);
        for (size_t row = 0; row < target_row; ++row) {
            for (size_t col = 0; col < target_col; ++col) {
                if (std::abs((long int)row - (long int)col) == expected_diff /* diagonal occupied*/
                    && data[row][col])
                    return true;
            }
        }
#pragma GCC diagnostic pop

        return false;
    }

    bool secondaryDiagonalOccupied(size_t target_row, size_t target_col) {
        // 次对角线 （从右上到左下）：
        // 如果要找同一次对角线上的其他点，它们的行索引和列索引之和（保持正数）是相同的。
        // 也就是，对于每个点 (i, j) 和 (k, l) 在同一条次对角线上，有 i + j = k + l。
        size_t expected_sum = target_row + target_col;
        for (size_t row = 0; row < target_row; ++row) {
            for (size_t col = target_col + 1; col < SIZE; ++col) {
                if (row + col == expected_sum /* found the exact */
                    && data[row][col]         /* oocupied */
                )
                    return true;
            }
        }
        return false;
    }
};

struct Queen {
    static int solutions;

    template <typename Boarder>
    void solve(Boarder& boarder, size_t row = 0) {
        if (row == Boarder::SIZE) {
            ++solutions;
            // Found it;
        } else {
            for (size_t col = 0; col < Boarder::SIZE; ++col) {
                if (!boarder.columnOccupied(col) && !boarder.diagonalOccupied(row, col) &&
                    !boarder.secondaryDiagonalOccupied(row, col)) {
                    boarder.data[row][col] = true;
                    solve(boarder, row + 1);
                    // boarder.data[row][col] = false;  // reset
                }
            }
        }
    }
};

int Queen::solutions = 0;

TEST(Boarder, columnOccupied) {
    Boarder<8> boarder;
    static constexpr size_t row = 0;
    // Set all column occupied state
    for (size_t col = 0; col < boarder.SIZE; ++col) {
        boarder.data[row][col] = true;
    }

    for (size_t col = 0; col < boarder.SIZE; ++col) {
        EXPECT_TRUE(boarder.columnOccupied(col));
    }
}

TEST(Boarder, diagonalOccupied) {
    Boarder<8> boarder;

    EXPECT_FALSE(boarder.diagonalOccupied(0, 0));
    EXPECT_FALSE(boarder.diagonalOccupied(1, 1));
    EXPECT_FALSE(boarder.diagonalOccupied(2, 2));
    EXPECT_FALSE(boarder.diagonalOccupied(3, 3));

    // Set all column occupied state
    boarder.data[0][0] = true;
    EXPECT_TRUE(boarder.diagonalOccupied(1, 1));
    EXPECT_TRUE(boarder.diagonalOccupied(2, 2));
    EXPECT_TRUE(boarder.diagonalOccupied(3, 3));
}

TEST(Boarder, diagonalOccupied2) {
    Boarder<8> boarder;

    EXPECT_FALSE(boarder.diagonalOccupied(2, 0));
    EXPECT_FALSE(boarder.diagonalOccupied(3, 1));
    EXPECT_FALSE(boarder.diagonalOccupied(4, 2));

    EXPECT_FALSE(boarder.slopeOccupied(2, 0));
    EXPECT_FALSE(boarder.slopeOccupied(3, 1));
    EXPECT_FALSE(boarder.slopeOccupied(4, 2));

    // Set all column occupied state
    boarder.data[2][0] = true;
    EXPECT_TRUE(boarder.diagonalOccupied(3, 1));
    EXPECT_TRUE(boarder.diagonalOccupied(4, 2));

    EXPECT_TRUE(boarder.slopeOccupied(3, 1));
    EXPECT_TRUE(boarder.slopeOccupied(4, 2));
}

TEST(Boarder, secondaryDiagonalOccupied) {
    Boarder<8> boarder;
    EXPECT_FALSE(boarder.secondaryDiagonalOccupied(0, 4));
    EXPECT_FALSE(boarder.secondaryDiagonalOccupied(1, 3));
    EXPECT_FALSE(boarder.secondaryDiagonalOccupied(2, 2));

    EXPECT_FALSE(boarder.slopeOccupied(0, 4));
    EXPECT_FALSE(boarder.slopeOccupied(1, 3));
    EXPECT_FALSE(boarder.slopeOccupied(2, 2));

    boarder.data[0][4] = true;
    EXPECT_TRUE(boarder.secondaryDiagonalOccupied(1, 3));
    EXPECT_TRUE(boarder.secondaryDiagonalOccupied(2, 2));

    EXPECT_TRUE(boarder.slopeOccupied(1, 3));
    EXPECT_TRUE(boarder.slopeOccupied(2, 2));
    boarder.print();
}

TEST(queen, tr4) {
    Boarder<8> boarder;
    Queen q;
    q.solve(boarder);
    EXPECT_EQ(92, q.solutions);
}

}  // namespace tr4

// https://juejin.cn/post/7109086840879054879
// https://blog.csdn.net/m0_60634555/article/details/128785161
// https://cloud.tencent.com/developer/article/1424758
