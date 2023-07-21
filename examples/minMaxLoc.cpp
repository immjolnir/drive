// https://github.com/opencv/opencv/issues/7912
#include <iomanip>
#include <opencv2/opencv.hpp>

#include <iostream>

int main(void) {
    /*
    see https://zhuanlan.zhihu.com/p/91175671

    利用矩阵尺寸和类型参数构造Mat类
    cv::Mat::Mat(int rows, int cols, int type)
    rows：构造矩阵的行数
    cols：矩阵的列数
    type：矩阵中存储的数据类型。此处除了CV_8UC1、CV_64FC4等从1到4通道以外，还提供了更多通道的参数，通过CV_8UC(n)中的n来构建多通道矩阵，其中n最大可以取到512.
    */
    cv::Mat m(1, 3, CV_32FC1);
    /*
    size：2D数组变量尺寸，通过Size(cols, rows)进行赋值。
    */
    // cv::Mat m0(cv::Size(3, 1), CV_32FC1);
    /*
    at(row, col)
    row: index of rows
    row: index of cols

    implementation: ((const _Tp*)(data + step.p[0] * i0))[i1];
    */
    m.at<double>(0, 0) = std::numeric_limits<double>::quiet_NaN();
    m.at<double>(0, 1) = std::numeric_limits<double>::signaling_NaN();
    m.at<double>(0, 2) = std::numeric_limits<double>::infinity();

    std::cout << "cols=" << m.cols << ", rows=" << m.rows << ", data=" << m << std::endl;

    double min = 0, max = 0;
    cv::Point minLoc(-1, -1), maxLoc(-1, -1);
    cv::minMaxLoc(m, &min, &max, &minLoc, &maxLoc);

    std::cout << "min: " << std::setw(3) << min << " @ " << minLoc << " is finite? " << std::isfinite(min) << std::endl;
    std::cout << "max: " << std::setw(3) << max << " @ " << maxLoc << " is finite? " << std::isfinite(max) << std::endl;

    /*
    converts NaNs to the given number

    void cv::patchNaNs(InputOutputArray a, double val = 0)
    @param a:	input/output matrix (CV_32F type).
    @param val:	value to convert the NaNs
    */
    cv::patchNaNs(m, 0);
    std::cout << "cols=" << m.cols << ", rows=" << m.rows << ", data=" << m << std::endl;

    cv::minMaxLoc(m, &min, &max, &minLoc, &maxLoc);

    std::cout << "min: " << std::setw(3) << min << " @ " << minLoc << " is finite? " << std::isfinite(min) << std::endl;
    std::cout << "max: " << std::setw(3) << max << " @ " << maxLoc << " is finite? " << std::isfinite(max) << std::endl;

    {
        // 利用枚举法赋值示例
        cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
        // cv::Mat b = (cv::Mat_<double>(2, 3) << 1.0, 2.1, 3.2, 4.0, 5.1, 6.2);
        std::cout << "rows=" << m.rows << ", cols=" << m.cols << ", data=" << m << std::endl;

        double min_val, max_val;
        cv::Point min_loc, max_loc;  // point format: (x, y). And x is col index, y is row index.
        cv::minMaxLoc(m, &min_val, &max_val, &min_loc, &max_loc);
        std::cout << "min:" << min_loc << " has " << min_val << std::endl;
        std::cout << "max:" << max_loc << " has " << max_val << std::endl;  // max:[3, 1] has 8.
        std::cout << m.at<int>(1, 3) << std::endl;
    }
    {
        // 利用数组赋值示例
        int a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        cv::Mat m = cv::Mat(2, 4, CV_32S, a);
        // cv::Mat b = (cv::Mat_<double>(2, 3) << 1.0, 2.1, 3.2, 4.0, 5.1, 6.2);
        std::cout << "rows=" << m.rows << ", cols=" << m.cols << ", data=" << m << std::endl;

        double min_val, max_val;
        cv::Point min_loc, max_loc;  // point format: (x, y). And x is col index, y is row index.
        cv::minMaxLoc(m, &min_val, &max_val, &min_loc, &max_loc);
        std::cout << "min:" << min_loc << " has " << min_val << std::endl;
        std::cout << "max:" << max_loc << " has " << max_val << std::endl;  // max:[3, 1] has 8.
        std::cout << m.at<int>(1, 3) << std::endl;
    }
    return 0;
}