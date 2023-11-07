#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <opencv2/opencv.hpp>

/*
                  width(cols)
         O-------------------------------> x
         |    top_left
         |        +----------.
         |        |          |
height(rows) |        |     o    |h
         |        | (cx, cy) |
         |        '----------'
         |            w
         |
         v
         y
*/
TEST(mat, matrix_roi_crash) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = 1;
    int roi_y = 1;
    int roi_width = 2;
    int roi_height = 2;

    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    try {
        cv::Mat roi_mat = m(roi_rect);
        // C++ exception with description "OpenCV(4.5.4) ./modules/core/src/matrix.cpp:810: error: (-215:Assertion
        // failed) 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y
        // + roi.height <= m.rows in function 'Mat' " thrown in the test body.
        FAIL() << "Expected c++ exception";
    } catch (std::exception& e) {
        EXPECT_EQ(std::string("OpenCV(4.5.4) ./modules/core/src/matrix.cpp:810: error: (-215:Assertion failed) 0 <= "
                              "roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height "
                              "&& roi.y + roi.height <= m.rows in function 'Mat'\n"),
                  e.what());
    } catch (...) {
        FAIL() << "Expected c++ exception";
    }
}

TEST(mat, matrix_roi_fix) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = 1;
    int roi_y = 1;
    int roi_width = 2;
    int roi_height = 2;

    // Not always work!
    //    int top_left_x = std::max(0, roi_x);
    //    int top_left_y = std::max(0, roi_y);
    //    int width = std::max(top_left_x + roi_width > m.cols ? m.cols - top_left_x : roi_width, 0);
    //    int height = std::max(top_left_y + roi_height > m.rows ? m.rows - top_left_y : roi_height, 0);
    //
    //    std::cout << "top_left_x=" << top_left_x << ", top_left_y=" << top_left_y << ", width=" << width
    //              << ", height=" << height << std::endl;
    //    //   top_left_x=1, top_left_y=1, width=3, height=2
    // cv::Rect roi_rect = cv::Rect(top_left_x, top_left_y, width, height);

    // New fix: you need to be sure if the Rect roi is inside the image bounds
    // https://answers.opencv.org/question/5042/opencv-crop-function-fatal-signal-11/
    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    roi_rect = roi_rect & cv::Rect(0, 0, m.cols, m.rows);
    std::cout << "roi_rect=" << roi_rect << std::endl;
    cv::Mat roi_mat = m(roi_rect);
    EXPECT_EQ(roi_rect.size(), roi_mat.size());  // roi_rect=[2 x 1 from (1, 1)]
}

TEST(mat, matrix_roi) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = -2;
    int roi_y = 4;
    int roi_width = 2;
    int roi_height = 2;

    // New fix: you need to be sure if the Rect roi is inside the image bounds
    // https://answers.opencv.org/question/5042/opencv-crop-function-fatal-signal-11/
    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    roi_rect = roi_rect & cv::Rect(0, 0, m.cols, m.rows);
    std::cout << "roi_rect=" << roi_rect << std::endl;

    cv::Mat roi_mat = m(roi_rect);
    EXPECT_EQ(cv::Size(0, 0), roi_mat.size());
}