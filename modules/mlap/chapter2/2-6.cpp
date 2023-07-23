#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap;
    cap.open("bike.avi");
    cv::Mat frame;
    for (;;) {
        cap >> frame;
        if (frame.empty()) break;
        cv::imshow("Example2_3", frame);
        if (cv::waitKey(33) >= 0) break;
    }
    return 0;
}
