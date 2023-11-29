#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <opencv2/opencv.hpp>

TEST(mat, Mat2Vector) {
    cv::Mat m = cv::Mat::ones(3, 2, CV_32SC1);
    std::vector<int> vec(m.begin<int>(), m.end<int>());
    EXPECT_EQ(4, m.type());
    EXPECT_EQ(6, vec.size());
    EXPECT_THAT(vec, testing::ElementsAre(1, 1, 1, 1, 1, 1));
}

TEST(mat, Mat2Vector2) {
    cv::Mat m = (cv::Mat_<int>(3, 2) << 1, 2, 3, 4, 5, 6);
    EXPECT_EQ(4, m.type());

    std::vector<int> vec;
    vec.reserve(6);
    vec.assign(m.begin<int>(), m.end<int>());

    EXPECT_EQ(6, vec.size());
    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3, 4, 5, 6));
}

TEST(mat, Mat2Vector_copyTo) {
    cv::Mat m = (cv::Mat_<int>(3, 2) << 1, 2, 3, 4, 5, 6);
    EXPECT_EQ(4, m.type());
    /*
    [1, 2;
     3, 4;
     5, 6]
    */
    // std::cout << m << std::endl;

    std::vector<int> vec;
    m.col(0).copyTo(vec);

    EXPECT_EQ(3, vec.size());
    EXPECT_THAT(vec, testing::ElementsAre(1, 3, 5));
}

TEST(mat, vector2Mat) {
    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    /*
    template<typename _Tp >
    Mat (const std::vector< _Tp > &vec, bool copyData=false)
    */
    cv::Mat m(vec, false);
    EXPECT_EQ(1, m.cols);
    EXPECT_EQ(6, m.rows);
    EXPECT_EQ(3, m.type());
}

TEST(mat, vector2Mat2) {
    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    /*
     Mat (int ndims, const int *sizes, int type, void *data, const size_t *steps=0)
    */
    int ndims = 1;
    cv::Mat m(ndims, vec.size(), CV_16SC1, vec.data());

    std::cout << "vector2Mat2: \n" << m << std::endl;
    /*
    vector2Mat2:
    [1, 2, 3, 4, 5, 6]
    */
    EXPECT_EQ(6, m.cols);
    EXPECT_EQ(1, m.rows);
    EXPECT_EQ(3, m.type());
}

TEST(mat, vector2Mat3) {
    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    /*
        Mat (Size size, int type, void *data, size_t step=AUTO_STEP)

    error: invalid conversion from 'short int*' to 'int' [-fpermissive]
   75 |     cv::Mat m(vec.size(), 3, vec.data());
      |                              ~~~~~~~~^~
      |                                      |
      |                                      short int*
    */
    // cv::Mat m(vec.size(), 3, vec.data());

    // Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    int rows = 3, cols = 2, type = 3;  // CV_16SC1
    cv::Mat m(rows, cols, type, vec.data());

    std::cout << "vector2Mat3: \n" << m << std::endl;
    /*
    vector2Mat3:
        [1, 2;
         3, 4;
         5, 6]
    */
    EXPECT_EQ(2, m.cols);
    EXPECT_EQ(3, m.rows);
    EXPECT_EQ(3, m.type());
}
