#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <opencv2/opencv.hpp>

/*
* Comparing the effective:
  - Mat (const Mat &m, const Rect &roi);
    - No data is copied by these constructors. Instead, the header pointing to m data or its sub-array is constructed
and associated with it. The reference counter, if any, is incremented. So, when you modify the matrix formed using such
a constructor, you also modify the corresponding elements of m . If you want to have an independent copy of the
sub-array, use Mat::clone().
    - 修改 它构造的对象，会影响到 m.

  - Mat operator() (const Rect &roi) const
    - 修改 它构造的对象，会影响到 m.

Note: 得到roi后，不能对齐进行重新赋值

https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html


- Compare two cv::Mat objects
  - https://stackoverflow.com/questions/25660020/compare-2-cvmat

  If you need to check if 2 cv::Mat are equal, you can perform the XOR operator and check if the result is a cv::Mat
full of zeros:

    cv::bitwise_xor(mat1, mat2, dst);
    if(cv::countNonZero(dst) > 0) //check non-0 pixels
    //do stuff in case cv::Mat are not the same
    else
    //do stuff in case they are equal
*/

TEST(mat, modify_a_rect_of_mat) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    cv::Mat roi(m, rect);  // alternatively you can use: cv::Mat roi = m(rect);

    double a = 0.1;
    double b = 20;
    roi = (roi + a) * b;  // this will modify the relevant area in img
    std::cout << "modify_a_rect_of_mat: \n" << m << std::endl;
    std::cout << "modify_a_rect_of_mat: roi: \n" << roi << std::endl;
    /*
    modify_a_rect_of_mat:
    [0, 0, 0, 0, 0;
    0, 0, 2, 2, 2;
    0, 0, 2, 2, 2;
    0, 0, 0, 0, 0;
    0, 0, 0, 0, 0]
    modify_a_rect_of_mat: roi:
    [2, 2, 2;
    2, 2, 2]
    */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(0, cv::countNonZero(dst));
}

TEST(mat, mat_assignment_operator_not_work) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    cv::Mat roi = m(rect);

    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    // 	Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    int rows = height, cols = width;
    roi = cv::Mat(rows, cols, 3, vec.data());  // 对 roi 重新赋值了

    std::cout << "mat_assignment_operator_not_work: \n" << m << std::endl;
    std::cout << "mat_assignment_operator_not_work: roi: \n" << roi << std::endl;
    /*
    mat_assignment_operator_not_work:
    [0, 0, 0, 0, 0;
    0, 0, 0, 0, 0;
    0, 0, 0, 0, 0;
    0, 0, 0, 0, 0;
    0, 0, 0, 0, 0]
    mat_assignment_operator_not_work: roi:
    [1, 2, 3;
    4, 5, 6]

    // roi.setTo(0); // Set all pixel to 0 on both src and roi
 really?
    */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(6, cv::countNonZero(dst));
}

TEST(mat, mat_ctor_not_work) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    // cv::Mat roi = m(rect);
    cv::Mat roi(m, rect);  // use cv::Mat constructor

    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    // 	Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    int rows = height, cols = width;
    roi = cv::Mat(rows, cols, 3, vec.data());  // 对 roi 重新赋值了

    std::cout << "mat_ctor_not_work: \n" << m << std::endl;
    std::cout << "mat_ctor_not_work: roi: \n" << roi << std::endl;
    /*
    mat_ctor_not_work:
    [0, 0, 0, 0, 0;
     0, 0, 0, 0, 0;
     0, 0, 0, 0, 0;
     0, 0, 0, 0, 0;
     0, 0, 0, 0, 0]
    mat_ctor_not_work: roi:
    [1, 2, 3;
     4, 5, 6]
     */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(6, cv::countNonZero(dst));
}

TEST(mat, mat_assignment_operator_work_for_no_reassign_roi) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    cv::Mat roi = m(rect);

    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    // 	Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    int rows = height, cols = width;
    cv::Mat rect_m(rows, cols, 3, vec.data());
    rect_m.copyTo(roi);  // 对 roi 没有重新赋值了

    std::cout << "mat_assignment_operator_work_for_no_reassign_roi: \n" << m << std::endl;
    std::cout << "mat_assignment_operator_work_for_no_reassign_roi: roi: \n" << roi << std::endl;
    /*
    mat_assignment_operator_work_for_no_reassign_roi:
    [0, 0, 0, 0, 0;
    0, 0, 1, 2, 3;
    0, 0, 4, 5, 6;
    0, 0, 0, 0, 0;
    0, 0, 0, 0, 0]
    mat_assignment_operator_work_for_no_reassign_roi: roi:
    [1, 2, 3;
    4, 5, 6]
    */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(0, cv::countNonZero(dst));
}

TEST(mat, mat_ctor_work_for_noreassign_roi) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    // cv::Mat roi = m(rect);
    cv::Mat roi(m, rect);  // use cv::Mat constructor

    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    // 	Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    int rows = height, cols = width;
    // It will detach the roi from m.
    // roi = cv::Mat(rows, cols, 3, vec.data());
    cv::Mat rect_m(rows, cols, 3, vec.data());
    rect_m.copyTo(roi);  // 对 roi 没有重新赋值了

    std::cout << "mat_ctor_work_for_noreassign_roi: \n" << m << std::endl;
    std::cout << "mat_ctor_work_for_noreassign_roi: roi: \n" << roi << std::endl;
    /*
    Yes, it works!

    mat_ctor_work_for_noreassign_roi:
    [0, 0, 0, 0, 0;
     0, 0, 1, 2, 3;
     0, 0, 4, 5, 6;
     0, 0, 0, 0, 0;
     0, 0, 0, 0, 0]
    mat_ctor_work_for_noreassign_roi: roi:
    [1, 2, 3;
     4, 5, 6]
    */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(0, cv::countNonZero(dst));
}

TEST(mat, general_method) {
    cv::Mat m = cv::Mat::zeros(5, 5, CV_16SC1);

    //  Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    int width = 3, height = 2;
    cv::Rect rect(2, 1, width, height);
    cv::Mat roi(m, rect);

    std::vector<short> vec{1, 2, 3, 4, 5, 6};
    /*
        MatConstIterator_<VT> it1 = src1.begin<VT>(), it1_end = src1.end<VT>();
        MatConstIterator_<VT> it2 = src2.begin<VT>();
        MatIterator_<VT> dst_it = dst.begin<VT>();
    */
    cv::MatIterator_<short> roi_it = roi.begin<short>();
    for (auto& e : vec) {
        *roi_it++ = e;
    }

    std::cout << "general_method: \n" << m << std::endl;
    std::cout << "general_method: roi: \n" << roi << std::endl;
    /*
    Yes, it also works but ugly.
    general_method:
    [0, 0, 0, 0, 0;
     0, 0, 1, 2, 3;
     0, 0, 4, 5, 6;
     0, 0, 0, 0, 0;
     0, 0, 0, 0, 0]
    general_method: roi:
    [1, 2, 3;
     4, 5, 6]
    */
    cv::Mat dst;
    cv::bitwise_xor(m(rect), roi, dst);
    EXPECT_EQ(0, cv::countNonZero(dst));
}
