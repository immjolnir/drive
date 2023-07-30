#pragma once

struct COCOBBox {
    int top_left_x;
    int top_left_y;
    int width;
    int height;
};

using BBox = COCOBBox;