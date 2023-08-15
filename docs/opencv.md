# OpenCV


## Issues
- error: 'CV_LOAD_IMAGE_COLOR' was not declared in this scope
- error: 'CV_LOAD_IMAGE_UNCHANGED' was not declared in this scope
  - include the legency header
    - `#include <opencv2/imgcodecs/imgcodecs_c.h>`
    - `#include <opencv2/imgcodecs/legacy/constants_c.h>`
  - Use the replaced flags:
  ```
  CV_LOAD_IMAGE_UNCHANGED = -1 ( = cv::IMREAD_NCHANGED)
  CV_LOAD_IMAGE_GRAYSCALE = 0 ( = cv::IMREAD_GRAYSCALE)
  CV_LOAD_IMAGE_COLOR     = 1 ( = cv::IMREAD_COLOR_
  CV_LOAD_IMAGE_ANYDEPTH  = 2 ( = cv::IMREAD_ANYDEPTH)
  CV_LOAD_IMAGE_ANYCOLOR  = 4
  ```

- error: 'CV_FM_8POINT' was not declared in this scope
  - Remove the prefix: `CV_`

- error: 'CV_GRAY2BGR' was not declared in this scope
  - use `cv::COLOR_GRAY2BGR`
  - `#include <opencv2/imgproc/types_c.h>`

