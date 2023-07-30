#pragma once

#include "bbox.h"

#include <opencv2/opencv.hpp>

namespace vot
{
    struct Frame {
        BBox groundtruth;
        BBox predicted;
        BBox target;
        cv::Mat img;
    };

}  // namespace vot