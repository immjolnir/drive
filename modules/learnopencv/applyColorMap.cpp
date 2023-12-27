/*
cv::applyColorMap是OpenCV库中的一个函数，用于将灰度图像或者特定颜色通道的图像转换为具有伪色彩的图像。

以下是对该函数的基本使用说明：
void cv::applyColorMap(InputArray src, OutputArray dst, int colormap)
参数解释：
src：输入图像，应为单通道8位或浮点型图像（通常为灰度图像）。
dst：输出图像，将存储应用了颜色映射后的结果。图像类型与输入图像相同，但会扩展为三通道（BGR或RGB，取决于OpenCV的颜色顺序设置）。
colormap：指定要使用的颜色映射类型。这是一个整数值，可以是以下预定义的颜色映射之一：
    - COLORMAP_AUTUMN• （0）
    - COLORMAP_BONE• （1）
    - COLORMAP_JET• （2）（默认值）
    - COLORMAP_WINTER• （3）
    - COLORMAP_RAINBOW• （4）
    - COLORMAP_OCEAN• （5）
    - COLORMAP_SUMMER• （6）
    - COLORMAP_SPRING• （7）
    - COLORMAP_COOL• （8）
    - COLORMAP_HSV• （9）
    - COLORMAP_PINK• （10）
    - COLORMAP_HOT• （11）

示例用法：
*/

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Usage: $0 gray_image" << std::endl;
        return -1;
    }

    std::string image_file(argv[1]);
    // 加载灰度图像
    // https://unsplash.com/s/photos/grayscale
    // https://www.pexels.com/search/grayscale/
    cv::Mat gray_image = cv::imread(image_file, cv::IMREAD_GRAYSCALE);

    // 创建目标彩色图像
    cv::Mat color_image;

    // 应用颜色映射，这里使用默认的COLORMAP_JET
    cv::applyColorMap(gray_image, color_image, cv::COLORMAP_JET);

    // 显示或保存彩色图像
    cv::namedWindow("Color Map", cv::WINDOW_AUTOSIZE);
    cv::imshow("Color Map", color_image);
    cv::waitKey(0);

    return 0;
}

// 在这个例子中，我们将一个灰度图像转换为使用COLORMAP_JET颜色映射的彩色图像，并显示结果。你可以根据需要选择不同的颜色映射。请注意，如果你的原始图像是彩色的，你可能需要先将其转换为灰度图像再进行颜色映射。