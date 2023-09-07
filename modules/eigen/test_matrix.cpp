#include <gtest/gtest.h>

// Eigen 核心部分
#include <Eigen/Core>
// 稠密矩阵的代数运算（逆，特征值等）
#include <Eigen/Dense>

#include <iostream>

using namespace std;

TEST(matrix, matrix) {
    // Eigen 中所有向量和矩阵都是 Eigen::Matrix，它是一个模板类。它的前三个参数为：数据类型，行，列
    // 声明一个2*3的float矩阵
    Eigen::Matrix<float, 2, 3> matrix_23;

    // 下面是对Eigen阵的操作
    // 输入数据（初始化）
    matrix_23 << 1, 2, 3, 4, 5, 6;
    // 输出
    cout << "matrix 2x3 from 1 to 6: \n" << matrix_23 << endl;

    // 用()访问矩阵中的元素
    cout << "print matrix 2x3: " << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) cout << matrix_23(i, j) << "\t";
        cout << endl;
    }

    /*
    matrix 2x3 from 1 to 6:
    1 2 3
    4 5 6
    print matrix 2x3:
    1	2	3
    4	5	6
    */

    EXPECT_EQ(1, matrix_23(0, 0));
    EXPECT_EQ(2, matrix_23(0, 1));
    EXPECT_EQ(3, matrix_23(0, 2));
    EXPECT_EQ(4, matrix_23(1, 0));
    EXPECT_EQ(5, matrix_23(1, 1));
    EXPECT_EQ(6, matrix_23(1, 2));

    cout << "xxx=" << matrix_23.transpose() << endl;
    cout << "xxx=" << matrix_23.rotation() << endl;
}