#pragma once

#include <opencv2/opencv.hpp>

namespace vot
{

    struct COCOBBox {
        int x;  // top left x
        int y;  // top left y
        int width;
        int height;
    };

    using BBox = COCOBBox;

    /*
    Bounding box format:
      2 3
      1 4
    */
    struct VOTPolygon {
        float x1;  // bottom left
        float y1;

        float x2;  // top left
        float y2;

        float x3;  // top right
        float y3;

        float x4;  // bottom right
        float y4;
    };

    struct Frame {
        cv::Mat image;
        BBox groundtruth;
        BBox predicted;
        BBox target;
    };

}  // namespace vot