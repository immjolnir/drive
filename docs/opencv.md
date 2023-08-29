# OpenCV

## Understanding the Differences in Displaying Images with OpenCV and Matplotlib
- The BGR vs RGB Conundrum
The primary difference between how OpenCV and Matplotlib display the same image lies in the color space they use. OpenCV reads images in the BGR (Blue, Green, Red) format by default, while Matplotlib, along with most other image processing libraries, uses the RGB (Red, Green, Blue) format.

When you read an image using OpenCV and then display it using Matplotlib, the image may appear distorted. This is because Matplotlib assumes the image is in RGB format. However, since OpenCV reads it in BGR, the Red and Blue channels get swapped, leading to a color discrepancy.

- How to Resolve the Discrepancy?
To ensure consistency when displaying images, you need to convert the color space of the image read by OpenCV to RGB. Here’s how you can do it:
```
import cv2
import matplotlib.pyplot as plt

# Read the image using OpenCV
img = cv2.imread('image.jpg')

# Convert the BGR image into RGB
img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# Display the image using Matplotlib
plt.imshow(img_rgb)
plt.show()
```
In the above code, `cv2.cvtColor()` is used to convert the BGR image into RGB. Now, when you display the image using Matplotlib, the colors will appear as they should.

- Conclusion

While OpenCV and Matplotlib are powerful libraries in their own right, it’s essential to understand how they handle image data differently. Remembering the BGR (OpenCV) and RGB (Matplotlib) distinction can save you from unexpected results when analyzing and visualizing image data.

Understanding these nuances not only helps in accurate data representation but also in creating more effective and accurate image processing models.






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



# opencv中Size与Rect的宽高顺序

opencv中关于Rect和Size的宽高顺序折磨了我很久，略作记录。

Size_(_Tp _width, _Tp _height)Size是先宽后高，这一点如果不记得可以随时查看Size定义来确认；
Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)在opencv中x方向指的都是水平方向；y方向指的都是y方向，这一点在各处都是一样的。其实对于Size也是按照先x后y的顺序的。
matrix.at<char>(row, col)数组的访问当然还是按照先行后列的方式，这跟存储方式有关，不要搞混。

