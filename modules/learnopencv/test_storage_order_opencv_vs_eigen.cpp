#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opencv2/opencv.hpp>
// Eigen 核心部分
#include <Eigen/Core>
// 稠密矩阵的代数运算（逆，特征值等）
#include <Eigen/Dense>

#include <iostream>

// Exploring the OpenCV'mat storage

/*
// https://stackoverflow.com/questions/8184053/accessing-elements-of-a-cvmat-with-atfloati-j-is-it-x-y-or-row-col

OpenCV, like many other libraries, treat matrix access in row-major order. That means every access is defined as (row,
column). Note that if you're working with x and y coordinates of an image, this becomes (y, x), if y is your vertical
axis.

Most matrix libraries are the same in that regards, the access is (row, col) as well in, for example, Matlab or Eigen (a
C++ matrix library).

Where these applications and libraries do differ however is how the data is actually stored in memory. OpenCV stores the
data in row-major order in memory (i.e. the rows come first), while for example Matlab stores the data in column-major
order in memory. But if you're just a user of these libraries, and accessing the data via a (row, col) accessor, you'll
never actually see this difference in memory storage order.
*/

/*
Note that in the Eigen library,
  there is an optional template argument Options to specify the storage order which specifies how the data should will
be stored in the memory.Its values can be ColMajor or RowMajor.

  If the storage order is not specified, Eigen defaults to storing the data in column - major.

  However, in either case, matrix access is still defined as(row, col) i.e.row - major Source:
  https://eigen.tuxfamily.org/dox/group__TopicStorageOrders.html
*/

TEST(storage_order, OpenCVStorageOrder_row_major) {
    cv::Mat m(3, 3, CV_32FC1, 0.0f);
    m.at<float>(1, 0) = 2;

    const int x = 0, y = 1;
    EXPECT_EQ(2, m.at<float>(y, x));

    const int row = 1, column = 0;
    EXPECT_EQ(2, m.at<float>(row, column));

    {
        /*
        [0, 0, 0;
        2, 0, 0;
        0, 0, 0]
        */
        std::cout << m << std::endl;
    }
    // Traverse 2
    {
        EXPECT_TRUE(m.isContinuous());  // Because it is Continuous, we can traverse it like this.
        std::vector<float> actual;
        float* mp = &m.at<float>(0);
        for (int i = 0; i < 9; i++) {
            actual.emplace_back(mp[i]);
        }
        // std::cout << std::endl;
        EXPECT_THAT(actual, testing::ElementsAre(0, 0, 0, 2, 0, 0, 0, 0, 0));
    }
    // Traverse 3
    {
        std::vector<float> actual;
        for (auto iter = m.begin<float>(); iter != m.end<float>(); ++iter) {
            actual.emplace_back(*iter);
        }
        EXPECT_THAT(actual, testing::ElementsAre(0, 0, 0, 2, 0, 0, 0, 0, 0));
    }
    // Traverse 3
    {
        // Algorithms that traverse a matrix row by row will go faster when the matrix is stored in row-major order
        // because of better data locality.
        std::vector<float> actual;
        for (int row = 0; row < m.rows; ++row) {
            for (int column = 0; column < m.cols; ++column) {
                actual.emplace_back(m.at<float>(row, column));
            }
        }
    }
}

TEST(storage_order, EigenStorageOrder_column_order_by_default) {
    using namespace std;
    // If the storage order is not specified, then Eigen defaults to storing the entry in column-major

    // Specify the Column Major
    {
        Eigen::Matrix<int, 3, 4, Eigen::ColMajor> Acolmajor;
        Acolmajor << 8, 2, 2, 9, 9, 1, 4, 4, 3, 5, 4, 5;
        cout << "The matrix A:" << endl;
        cout << Acolmajor << endl << endl;

        cout << "In memory (column-major):" << endl;
        for (int i = 0; i < Acolmajor.size(); i++) cout << *(Acolmajor.data() + i) << "  ";
        cout << endl << endl;
    }

    // Specify the Row Major
    {
        Eigen::Matrix<int, 3, 4, Eigen::RowMajor> Arowmajor;
        Arowmajor << 8, 2, 2, 9, 9, 1, 4, 4, 3, 5, 4, 5;
        cout << "The matrix A:" << endl;
        cout << Arowmajor << endl << endl;

        cout << "In memory (row-major):" << endl;
        for (int i = 0; i < Arowmajor.size(); i++) cout << *(Arowmajor.data() + i) << "  ";
        cout << endl;
    }

    // Specify the Row Major but assign by Acolmajor
    {
        Eigen::Matrix<int, 3, 4, Eigen::ColMajor> Acolmajor;
        Acolmajor << 8, 2, 2, 9, 9, 1, 4, 4, 3, 5, 4, 5;
        Eigen::Matrix<int, 3, 4, Eigen::RowMajor> Arowmajor = Acolmajor;
        cout << "The matrix A:" << endl;
        cout << Arowmajor << endl << endl;

        cout << "In memory (row-major):" << endl;
        for (int i = 0; i < Arowmajor.size(); i++) cout << *(Arowmajor.data() + i) << "  ";
        cout << endl;
    }
    // default order
    {
        Eigen::Matrix<int, 3, 4> A;
        A << 8, 2, 2, 9, 9, 1, 4, 4, 3, 5, 4, 5;
        cout << "The matrix A:" << endl;
        cout << A << endl << endl;

        cout << "In memory (default order):" << endl;
        // It uses the PlainObjectBase::data() function,
        // which returns a pointer to the memory location of the first entry of the matrix.
        for (int i = 0; i < A.size(); i++) cout << *(A.data() + i) << "  ";
        cout << endl;
    }
}
