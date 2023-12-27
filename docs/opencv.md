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

- Size_(_Tp _width, _Tp _height)Size是先宽后高，这一点如果不记得可以随时查看Size定义来确认；
- Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)在opencv中x方向指的都是水平方向；y方向指的都是y方向，这一点在各处都是一样的。其实对于Size也是按照先x后y的顺序的。
- matrix.at<char>(row, col)数组的访问当然还是按照先行后列的方式，这跟存储方式有关，不要搞混。

- Example: modules/learnopencv/test_mat.cpp


# [How to find out what type of a mat object?](https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv)
```c++
mat = imread("C:\someimage.jpg");
type = mat.type();
```
and type = 16, how do I find out what type of mat matrix is?.

For debugging purposes in case you want to look up a raw Mat::type in a debugger:

|      | C1 |	C2 | C3 |	C4 |C(5)|C(6)|C(7)|C(8)|
|:-----|:---|:---|:---|:---|:---|:---|:---|:---|
|CV_8U |	0 |	8  |	16|	 24|	32|	 40|	48|  56|
|CV_8S |	1 |	9  |	17|	 25|	33|	 41|	49|  57|
|CV_16U|	2 |	10 |	18|	 26|	34|	 42|	50|  58|
|CV_16S|	3 |	11 |	19|	 27|	35|	 43|	51|  59|
|CV_32S|	4 |	12 |	20|	 28|	36|	 44|	52|  60|
|CV_32F|	5 |	13 |	21|	 29|	37|	 45|	53|  61|
|CV_64F|	6 |	14 |	22|	 30|	38|	 46|	54|  62|

So for example, if type = 30 then OpenCV data type is CV_64FC4. If type = 50 then the OpenCV data type is CV_16UC(7).

Here is a handy function you can use to help with identifying your opencv matrices at runtime. I find it useful for debugging, at least.
```c++
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
```
To get depth and chans you could use the macros `CV_MAT_DEPTH(type)` and `CV_MAT_CN(type)`, respectively.
Their type should also be int, which would allow you to use `to_string(chans)` instead of chans+'0'.

## What does the "isContinuous()" function do?

- https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html#aa90cea495029c7d1ee0a41361ccecdf3

> The method returns true if the matrix elements are stored continuously without gaps at the end of each row. Otherwise, it returns false.

Obviously, 
- 1x1 or 1xN matrices are always continuous. 
- Matrices created with Mat::create are always continuous. 
- But if you extract a part of the matrix using `Mat::col`, `Mat::diag`, and so on, or constructed a matrix header for externally allocated data, such matrices may no longer have this property.

The continuity flag is stored as a bit in the `Mat::flags` field and is computed automatically when you construct a matrix header. Thus, the continuity check is a very fast operation, though theoretically it could be done as follows:
```c++
// alternative implementation of Mat::isContinuous()
bool myCheckMatContinuity(const Mat& m)
{
 //return (m.flags & Mat::CONTINUOUS_FLAG) != 0;
 return m.rows == 1 || m.step == m.cols*m.elemSize();
}
```
The method is used in quite a few of OpenCV functions. 


`isContinuous` is meant like: the memory in yout Mat is a continuous, single chunk.

there are some cases, where this isn't so, think of a ROI:

```
+-----------+
+           +
+   +---+   +
+   +   +   +
+   +---+   +
+-----------+
```
or `Mat::diag()`, or `Mat::col(i)`, all cases of non-continuous memory.


# [OpenCV Mat data member access](https://stackoverflow.com/questions/34042112/opencv-mat-data-member-access)

`Mat` data is an `uchar*`. If you have a, say, `float` matrix `CV_32FC1`, you need to access data as `float`.

You can do in different ways, not necessarily using casting:
```c++
#include <opencv2\opencv.hpp>
using namespace cv;

int main()
{
    cv::Mat matF(3, 3, CV_32F);
    randu(matF, Scalar(0), Scalar(10));

    int rowIdx = 1;
    int colIdx = 1;

    // 1
    float f1 = matF.at<float>(rowIdx, colIdx);

    // 2
    float* fData2 = (float*)matF.data;
    float f2 = fData2[rowIdx*matF.step1() + colIdx];

    // 3
    float* fData3 = matF.ptr<float>(0);
    float f3 = fData3[rowIdx*matF.step1() + colIdx];

    // 4
    float* fData4 = matF.ptr<float>(rowIdx);
    float f4 = fData4[colIdx];

    // 5
    Mat1f mm(matF); // Or directly create like: Mat1f mm(3, 3);
    float f5 = mm(rowIdx, colIdx);

    // f1 == f2 == f3 == f4 == f5

    return 0;
}
```


## Draw rectangle in OpenCV

The `cv::rectangle` function that accepts two `cv::Point`'s takes both the top left and the bottom right corner of a rectangle (pt1 and pt2 respectively in the documentation). If that rectangle is used with the `cv::rectangle` function that accepts a `cv::Rect`, then you will get the same result.

```c++
// just some valid rectangle arguments
int x = 0;
int y = 0;
int width = 10;
int height = 20;
// our rectangle...
cv::Rect rect(x, y, width, height);
// and its top left corner...
cv::Point pt1(x, y);
// and its bottom right corner.
cv::Point pt2(x + width, y + height);

// These two calls...
cv::rectangle(img, pt1, pt2, cv::Scalar(0, 255, 0));
// essentially do the same thing
cv::rectangle(img, rect, cv::Scalar(0, 255, 0))
```
- https://docs.opencv.org/2.4/modules/core/doc/drawing_functions.html?highlight=rectangle#rectangle

- https://docs.opencv.org/4.x/dc/da5/tutorial_py_drawing_functions.html


## waitKey

```
int cv::waitKey	(	int 	delay = 0	)	
```
The function waitKey waits for a key event infinitely (when delay≤0 ) or for delay milliseconds, when it is positive. 


## cv::Mat
- Deep copy
```
Mat a=b.clone(); 
```

- Shadow copy
```
Mat a = b;
```
When you assign one matrix to another, the counter of references of smart pointer to matrix data increased by one, when you release matrix (it can be done implicitly when leave code block) it decreases by one. When it becomes equal zero the allocated memory deallocated.

Take a look at c++11 std::shared_ptr effectively works in the same way, by using a reference counter cv::Mat cleverly remembers every time the pointer is referenced, once the count reaches 0 it is automatically released i.e. memory is deallocated and cv::Mat is no longer available. This is effectively a "shallow copy" and saves resources in allocating/deallocating large amounts of memory.

On the other hand cv::Mat::clone will provide a "deep copy" that allocates a whole new block of memory for the matrix to reside in, this can be useful if you are making transformations to an image that you may want to undo however, more memory allocating/deallocating increases the amount of resources required.

- https://docs.opencv.org/3.1.0/d3/d63/classcv_1_1Mat.html#afb01ff6b2231b72f55618bfb66a5326b
```
Mat cv::Mat::clone	(		)	const
```
Creates a full copy of the array and the underlying data.

The method creates a full copy of the array. The original step[] is not taken into account. So, the array copy is a continuous array occupying total()*elemSize() bytes.


## cv::Mat -> std::vector
- https://answers.opencv.org/question/145214/convert-cvmat-to-stdvector-without-copying/

Fastest way I know is this:
```c++
cv::Mat res(m, n, CV_32FC1);
std::vector<float>vec(res.begin<float>(), res.end<float>());
```

or if you declare your vec before:
```c++
std::vector<float> vec;
cv::Mat res(m, n, CV_32FC1);
vec.reserve(m*n);
vec.assign(res.begin<float>(), res.end<float>());
```
- https://stackoverflow.com/questions/26681713/convert-mat-to-array-vector-in-opencv


## std::vector -> cv::Mat
```c++
Mat m(vec, false); // false(by default) -- do not copy data 
```

- Here is another possible solution assuming matrix have one column( you can reshape original Mat to one column Mat via reshape):
```c++
Mat matrix= Mat::zeros(20, 1, CV_32FC1);
vector<float> vec;
matrix.col(0).copyTo(vec);
```

- https://stackoverflow.com/questions/36722044/how-to-initialize-a-cvmat-using-a-vector-of-floats
```c++
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    vector<float> vec{0.1,0.9,0.2,0.8,0.3,0.7,0.4,0.6,0.5,1};

    Mat m1( vec ); 
    imshow("m1",m1);
    waitKey();

    Mat m2( 1,vec.size(), CV_32FC1,vec.data());
    imshow("m2",m2);
    waitKey();

    Mat1f m3( vec.size(), 1, vec.data());
    imshow("m3",m3);
    waitKey();

    Mat1f m4( 1, vec.size(), vec.data());
    imshow("m4",m4);
    waitKey();

    cout << "as seen below all Mat and vector use same data" << endl;
    cout << vec[0] << endl;
    m1 *= 2;
    cout << vec[0] << endl;
    m2 *= 2;
    cout << vec[0] << endl;
    m3 *= 2;
    cout << vec[0] << endl;
    m4 *= 2;
    cout << vec[0] << endl;

    return 0;
}
```

- https://stackoverflow.com/questions/20980723/convert-mat-to-vector-float-and-vectorfloat-to-mat-in-opencv

```c++

// Generate some test data
int r=3;
int c=3;
Mat M(r,c,CV_32FC1);
for(int i=0;i<r*c;++i)
{
    M.at<float>(i)=i;
}
// print out matrix
cout << M << endl;

// Create vector from matrix data (data with data copying)
vector<float> V;
V.assign((float*)M.datastart, (float*)M.dataend);

// print out vector
cout << "Vector" << endl;
for(int i=0;i<r*c;++i)
{
    cout << V[i] << endl;
}

// Create matrix from vector

// Without copying data (only pointer assigned)
//Mat M2=Mat(r,c,CV_32FC1,(float*)V.data());

// With copying data
Mat M2=Mat(r,c,CV_32FC1);
memcpy(M2.data,V.data(),V.size()*sizeof(float));


// Print out matrix created from vector
cout << "Second matrix" << endl;
cout << M2 <<endl;
// wait for a key
getchar();
```
or 
```c++
// Convert a 1-channel Mat<float> object to a vector. 
void MatToVector(const Mat& in, vector<float>& out) {
  if (in.isContinuous()) { 
    out.assign((float*)in.datastart, (float*)in.dataend); 
  } else { 
    for (int i = 0; i < in.rows; ++i) { 
      out.insert(out.end(), in.ptr<float>(i), in.ptr<float>(i) + in.cols); 
    }
  }
  return;
}
```

## cv::Mat(rect) vs cv::Mat operator()
- modules/learnopencv/test_mat_ctor_vs_operator.cpp

- Compare two cv::Mat objects
  - https://stackoverflow.com/questions/25660020/compare-2-cvmat

  If you need to check if 2 cv::Mat are equal, you can perform the XOR operator and check if the result is a cv::Mat full of zeros:
```c++
    cv::bitwise_xor(mat1, mat2, dst);
    if(cv::countNonZero(dst) > 0) //check non-0 pixels
    //do stuff in case cv::Mat are not the same
    else
    //do stuff in case they are equal
```
  - https://stackoverflow.com/questions/9905093/how-to-check-whether-two-matrices-are-identical-in-opencv
The problem of using cv::countNonZero is that this function only works for one-channel images.

```c++
bool isEqual(cv::Mat firstImage, cv::Mat secondImage){
    cv::Mat dst;
    std::vector<cv::Mat>channels;
    int count = 0;
    cv::bitwise_xor(firstImage, secondImage, dst);
    cv::split(dst, channels);
    for (int ch = 0; ch<dst.channels();ch++){
        count += cv::countNonZero(channels[ch]);
    }
    return count == 0 ? true : false;
}
```

## opencv对象的 depth方法和 type方法分别是什么意思？

在OpenCV中，cv::Mat对象是用来存储图像和矩阵数据的主要数据结构。以下是对 depth和 type方法的解释：
- depth()方法返回 cv::Mat 对象中元素的深度或者说是每个数据元素的数据类型。
  • 数据深度表示每个像素值在内存中占用的位数（bit）。
  • OpenCV预定义了一系列的深度常量，如
    * CV_8U 无符号8位
    * CV_8S 有符号8位
    * CV_16U 无符号16位
    * CV_16S 有符号16位
    * CV_32S 有符号32位
    * CV_32F 单精度浮点数
    * CV_64F 双精度浮点

- type()方法返回 cv::Mat对象的完整类型信息。
  • 类型信息是由两个部分组成的：深度（由depth()方法返回的值）和通道数（即每个像素包含的颜色通道数，如灰度图是1个通道，BGR彩色图是3个通道 RGBA图是4个通道等）。
  • 类型信息的计算通常是将深度和通道数进行位级操作得到一个整数值。

例如，对于一个3通道的8位无符号整数图像（BGR格式），其类型为 CV_8UC3，其中 CV_8U 表示深度， 3表示通道数。
type()方法将返回一个代表这个类型的整数值。
总的来说，depth()方法提供了 cv::Mat中每个元素的数值精度信息，而 type()方法则包含了深度和通道数的综合信息，用于全面描述 cv::Mat对象的数据类型。这两个方法常常用于确保在进行图像处理或矩阵运算时数据类型的正确性和兼容性。

### 什么是位深度？
位深度在图像处理中是指用来存储每个像素所需的数据位数。
在 OpenCV 中，通过 Mat.depth() 函数可以获取图像的位深度。位深度决定了每个像素可以表示的颜色或灰度级别数量。
具体来说，不同的位深度对应如下：
- CV_8U：无符号8位，范围是0-255，通常用于表示灰度图像或者彩色图像的每个通道。
- CV_8S：有符号8位，范围是-128-127。
- CV_16U：无符号16位，范围是0-65535。
- CV_16S：有符号16位，范围是-32768-32767。
- CV_32S：有符号32位，范围是-2147483648 to 2147483647。
- CV_32F：单精度32位浮点数，范围大约是-3.4e38 to 3.4e38。
- CV_64F：双精度64位浮点数，范围大约是-1.7e308 to 1.7e308。

位深度越高，能够表示的颜色或灰度级别就越丰富，图像的质量通常也更高，但同时也会增加存储和处理的数据量。在进行图像处理和分析时，理解并正确处理位深度是非常重要的。

### 在opencv中，什么时候使用depth() 什么时候使用 type()?
在OpenCV中，depth() 和 type()都是与 cv::Mat 数据结构相关的函数，它们用于获取矩阵的数据类型信息。
- depth()函数主要用于返回矩阵中每个元素的基本数据类型，也就是数据的位深度或精度。它不考虑矩阵的通道数（例如在彩色图像中的BGR通道），只是表示单个像素值所占用的位数。常见的深度类型包括CV_8U（8位无符号整型）、CV_16U（16位无符号整型）、CV_32F（32位浮点型）等。

- type()函数则提供更全面的类型信息，它不仅包含了数据的深度，还考虑了矩阵的通道数。因此，type()返回的值是深度和通道数的组合。这对于处理多通道图像（如RGB、BGR、HSV等颜色空间的图像）非常有用，因为这些图像的每个像素通常由多个通道值组成。

简而言之，以下是一些使用 depth() 和 type() 的情景：
- 使用depth()：
  - 当你需要知道矩阵中单个元素的位深度或者要进行与数据精度相关的操作时。
  - 在进行数据类型转换或者计算时，需要确保源数据和目标数据具有相同的深度。

- 使用 type()：
  - 当你需要了解图像的颜色模型或者通道数时，例如判断一个图像是否是灰度图（单通道）还是彩色图（多通道）。
  - 在进行图像处理操作时，如果操作对图像的通道数有特定要求，那么需要使用 type() 来检查图像的类型。

总的来说，如果你关心的是数据的精度或者位深度，使用 depth()；如果你需要了解图像的完整类型信息，包括颜色模型和通道数，使用 type()。

## opencv分别显示不同的channel时，为什么都是灰色 ?
在 OpenCV 中，当你试图单独显示一个图像的某个颜色通道（如红、绿、蓝通道）时，如果该通道被单独显示为一幅灰度图像，这是因为颜色通道本身只包含了图像颜色的一个组成部分。
颜色图像通常是由三个颜色通道（红、绿、蓝，即RGB）组成的。
每个通道的值范围通常是0-255（对于8位深度图像）。
当这三个通道的值组合在一起时，人眼会看到各种颜色。
但是，如果你单独查看一个通道，例如红色通道，它只包含红色的强度信息，没有绿色和蓝色的信息。
因此，当你将一个颜色通道作为独立的图像显示时，由于缺少其他颜色通道的信息，图像看起来就像是灰度的。
这是因为灰度图像只有一个亮度通道，而你正在查看的单个颜色通道可以被视为一种亮度信息。

要正确地查看颜色图像的各个通道，你可以使用OpenCV的一些函数来分离和显示各个通道，如下所示：
```python
import cv2

# 加载彩色图像
img = cv2.imread('image.jpg')

# 分离颜色通道
b, g, r = cv2.split(img)

# 分别显示各个通道
cv2.imshow('Blue Channel', b) 
cv2.imshow('Green Channel', g)
cv2.imshow('Red Channel', r)

cv2.waitKey(0) cv2.destroyAllWindows()
```

这样，你就可以看到每个颜色通道单独呈现的灰度图像了。虽然它们看起来是灰色的，但实际上是表示原始彩色图像中对应颜色成分的强度分布。
