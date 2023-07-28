#pragma once

#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

namespace vot
{
    class DataLoader {};
}  // namespace vot

int main(int argc, char* argv[]) {
    std::string config_file(argv[1]);
    YAML::Node config = YAML::LoadFile(config_file);
    std::cout << "config=" << config << std::endl;
}
// https://cloud.tencent.com/developer/article/1762157
// https://cloud.tencent.com/developer/article/1423468
// https://www.cnblogs.com/nbtech/p/use_yaml-cpp_library.html
// https://hackingcpp.com/cpp/libs/fmt.html#chars-width