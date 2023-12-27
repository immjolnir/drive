#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

// Define ratios for Blue , Green , Red respectively
#define A 0.1140
#define B 0.5870
#define C 0.2989

// These are nearly perfect values of ratios for conversion of RGB to Grayscale.
// However, you may use any other value of these ratios, just make sure that the resultant lies between 0 and 255

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Usage: $0 color image" << std::endl;
        return -1;
    }

    std::string image_file(argv[1]);
    cv::Mat color_image = cv::imread(image_file, cv::IMREAD_COLOR);

    cv::Mat grayscale(color_image.rows, color_image.cols, CV_8UC1, cv::Scalar(0));
    for (int i = 0; i < color_image.rows; i++) {
        for (int j = 0; j < color_image.cols; j++) {
            const auto p = color_image.at<cv::Vec3b>(i, j);
            grayscale.at<uchar>(i, j) = (A * p[0] + B * p[1] + C * p[2]);
        }
    }

    cv::Mat bgr[3];               // destination array
    cv::split(color_image, bgr);  // split source
                                  // Note: OpenCV uses BGR color order
    int width = color_image.cols;
    int height = color_image.rows;
    std::cout << "depth=" << color_image.depth() << ", type=" << color_image.type() << ", bgr.type=" << bgr[0].type()
              << std::endl;

    // cv::Mat different_channels = cv::Mat::zeros(height, width * 3, CV_8UC3);
    cv::Mat different_channels = cv::Mat::zeros(height, width * 3, bgr[0].type());

    for (int i = 0; i < 3; ++i) {
        bgr[i].copyTo(different_channels(cv::Rect(width * i, 0, width, height)));
    }

    cv::imshow("blue", bgr[0]);
    cv::imshow("green", bgr[1]);
    cv::imshow("red", bgr[2]);

    /*https://stackoverflow.com/questions/20341910/opencv-taking-a-3-channel-rgb-image-splitting-channels-and-viewing-an-image-wi

    The most efficient way to do this is without any splits and merges. This saves both time and memory.

    Just bitwise-AND your image with cv::Scalar(0,255,255) and this will set your blue channel to zero.
    As in: imshow("R+G", src & cv::Scalar(0,255,255));.
    */
    // 这样三个就是彩色的了!!!
    cv::imshow("blue1", color_image & cv::Scalar(255, 0, 0));
    cv::imshow("green1", color_image & cv::Scalar(0, 255, 0));
    cv::imshow("red1", color_image & cv::Scalar(0, 0, 255));

    // imwrite("blue.png", bgr[0]);   // blue channel
    // imwrite("green.png", bgr[1]);  // green channel
    // imwrite("red.png", bgr[2]);    // red channel

    cv::namedWindow("Split Channels", cv::WINDOW_AUTOSIZE);
    cv::imshow("Split Channels", different_channels);  // it is gray color

    // 显示或保存彩色图像
    cv::namedWindow("Color", cv::WINDOW_AUTOSIZE);
    cv::imshow("Color", color_image);

    cv::namedWindow("Gray", cv::WINDOW_AUTOSIZE);
    cv::imshow("Gray", grayscale);
    cv::waitKey(0);

    return 0;
}