#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <opencv2/opencv.hpp>

/*
                  width(cols)
         O-------------------------------> x
         |    top_left
         |        +----------.
         |        |          |
height(rows) |        |     o    |h
         |        | (cx, cy) |
         |        '----------'
         |            w
         |
         v
         y
*/
TEST(mat, matrix_roi_crash) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = 1;
    int roi_y = 1;
    int roi_width = 2;
    int roi_height = 2;

    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    try {
        cv::Mat roi_mat = m(roi_rect);
        // C++ exception with description "OpenCV(4.5.4) ./modules/core/src/matrix.cpp:810: error: (-215:Assertion
        // failed) 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y
        // + roi.height <= m.rows in function 'Mat' " thrown in the test body.
        FAIL() << "Expected c++ exception";
    } catch (std::exception& e) {
        EXPECT_EQ(std::string("OpenCV(4.5.4) ./modules/core/src/matrix.cpp:810: error: (-215:Assertion failed) 0 <= "
                              "roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height "
                              "&& roi.y + roi.height <= m.rows in function 'Mat'\n"),
                  e.what());
    } catch (...) {
        FAIL() << "Expected c++ exception";
    }
}

TEST(mat, matrix_roi_fix) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = 1;
    int roi_y = 1;
    int roi_width = 2;
    int roi_height = 2;

    // Not always work!
    //    int top_left_x = std::max(0, roi_x);
    //    int top_left_y = std::max(0, roi_y);
    //    int width = std::max(top_left_x + roi_width > m.cols ? m.cols - top_left_x : roi_width, 0);
    //    int height = std::max(top_left_y + roi_height > m.rows ? m.rows - top_left_y : roi_height, 0);
    //
    //    std::cout << "top_left_x=" << top_left_x << ", top_left_y=" << top_left_y << ", width=" << width
    //              << ", height=" << height << std::endl;
    //    //   top_left_x=1, top_left_y=1, width=3, height=2
    // cv::Rect roi_rect = cv::Rect(top_left_x, top_left_y, width, height);

    // New fix: you need to be sure if the Rect roi is inside the image bounds
    // https://answers.opencv.org/question/5042/opencv-crop-function-fatal-signal-11/
    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    roi_rect = roi_rect & cv::Rect(0, 0, m.cols, m.rows);
    std::cout << "roi_rect=" << roi_rect << std::endl;
    cv::Mat roi_mat = m(roi_rect);
    EXPECT_EQ(roi_rect.size(), roi_mat.size());  // roi_rect=[2 x 1 from (1, 1)]
}

TEST(mat, matrix_roi) {
    cv::Mat m = (cv::Mat_<int>(2, 4) << 1, 2, 3, 4, 5, 6, 7, 8);
    std::cout << "m:\n" << m << std::endl;

    EXPECT_EQ(2, m.rows);
    EXPECT_EQ(4, m.cols);

    int roi_x = -2;
    int roi_y = 4;
    int roi_width = 2;
    int roi_height = 2;

    // New fix: you need to be sure if the Rect roi is inside the image bounds
    // https://answers.opencv.org/question/5042/opencv-crop-function-fatal-signal-11/
    cv::Rect roi_rect = cv::Rect(roi_x, roi_y, roi_width, roi_height);
    roi_rect = roi_rect & cv::Rect(0, 0, m.cols, m.rows);
    std::cout << "roi_rect=" << roi_rect << std::endl;

    cv::Mat roi_mat = m(roi_rect);
    EXPECT_EQ(cv::Size(0, 0), roi_mat.size());
}

TEST(mat, access_data_in_correct_way) {
    // Disparity Acccessor
    cv::Mat m = (cv::Mat_<short>(2, 3) << 0x0111, 0x0222, 0x0333, 0x0444, 0x0555, 0x0666);
    std::cout << "Type=" << m.type() << ", size=" << m.size() << ", total=" << m.total() << std::endl;

    /*
    * https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-iscontinuous
    * Reports whether the matrix is continuous or not.
    The method returns true if the matrix elements are stored continuously without gaps at the end of each row.
    Otherwise, it returns false.

    Obviously, 1x1 or 1xN matrices are always continuous.
    Matrices created with Mat::create() are always continuous.
    But if you extract a part of the matrix using Mat::col(), Mat::diag() , and so on, or constructed a matrix header
    for externally allocated data, such matrices may no longer have this property.
    */
    // alternative implementation of Mat::isContinuous()
    auto myCheckMatContinuity = [](const cv::Mat& m) -> bool {
        // return (m.flags & Mat::CONTINUOUS_FLAG) != 0;
        return m.rows == 1 || m.step == m.cols * m.elemSize();
    };

    EXPECT_EQ(1, m.channels());
    EXPECT_TRUE(m.isContinuous());
    EXPECT_TRUE(myCheckMatContinuity(m));

    EXPECT_EQ(3, m.type());  // short

    int width = 3, height = 2;
    EXPECT_EQ(width, m.cols);
    EXPECT_EQ(height, m.rows);
    EXPECT_EQ(cv::Size(width, height), m.size());
    // https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-total
    // Returns the total number of array elements(a number of pixels if the array represents an image).
    EXPECT_EQ(6, m.total());

    std::vector<int> vec;  // different type to m
    vec.reserve(m.total());
    /*
    /usr/include/opencv4/opencv2/core/mat.hpp:2012:51: note: candidate: 'template<class _Tp> cv::MatConstIterator_<_Tp>
cv::Mat::begin() const' 2012 |     template<typename _Tp> MatConstIterator_<_Tp> begin() const; | ^~~~~
/usr/include/opencv4/opencv2/core/mat.hpp:2012:51: note:   template argument deduction/substitution failed:

    vec.assign(m.begin(), m.end());
    */

    //     #1 0x7fb25ce27c08 in cv::fastMalloc(unsigned long) (/lib/x86_64-linux-gnu/libopencv_core.so.4.5d+0x72c08)
    {
        // heap-buffer-overflow
        // Mat data is an uchar*. If you have a, say, float matrix CV_32FC1, you need to access data as float.
        // uchar* srcptr = m.data;
        short* srcptr = (short*)m.data;
        for (int row = 0; row < m.rows; ++row) {
            for (int col = 0; col < m.cols; ++col) {
                short value = srcptr[row * m.step1() + col];
                std::cout << std::hex << (srcptr + row * m.step1() + col)
                          << " stores the value: " << reinterpret_cast<void*>(value) << std::endl;
            }
        }
        /*
        0x608000003340 stores the value: 0x111
        0x608000003342 stores the value: 0x222
        0x608000003344 stores the value: 0x333
        0x608000003346 stores the value: 0x444
        0x608000003348 stores the value: 0x555
        0x60800000334a stores the value: 0x666
        */
        std::cout << "======" << std::endl;
    }
    for (auto iter = m.begin<short>(); iter != m.end<short>(); ++iter) {  // Access the cv::Mat as short.
        // Print the value as hex

        // error: invalid cast from type 'cv::MatIterator_<int>' to type 'void*'
        // std::cout << std::hex << reinterpret_cast<void*>(iter) << std::endl;

        std::cout << std::hex << std::addressof(*iter) << " stores the value: " << reinterpret_cast<void*>(*iter)
                  << std::endl;
        /*
        0x608000003340 stores the value: 0x111
        0x608000003342 stores the value: 0x222
        0x608000003344 stores the value: 0x333
        0x608000003346 stores the value: 0x444
        0x608000003348 stores the value: 0x555
        0x60800000334a stores the value: 0x666
        */
    }

    int mat_element_number = 0;
    for (auto iter = m.begin<int>(); iter != m.end<int>(); ++iter) {  // Access the cv::Mat as int
        std::cout << std::hex << std::addressof(*iter) << " stores the value: " << reinterpret_cast<void*>(*iter)
                  << std::endl;
        /*
        0x608000003340 stores the value: 0x2220111
        0x608000003342 stores the value: 0x3330222
        0x608000003344 stores the value: 0x4440333
        0x608000003346 stores the value: 0x5550444
        0x608000003348 stores the value: 0x6660555
        0x60800000334a stores the value: 0xffffffffbebe0666
        */
        ++mat_element_number;
    }
    EXPECT_EQ(6, mat_element_number);

    vec.assign(m.begin<int>(), m.end<int>());  // type conversion

    EXPECT_EQ(0x0222'0111, vec[0]);
    EXPECT_EQ(0x0333'0222, vec[1]);
    EXPECT_EQ(0x0444'0333, vec[2]);
    EXPECT_EQ(0x0555'0444, vec[3]);
    EXPECT_EQ(0x0666'0555, vec[4]);
    EXPECT_EQ(0x0666, vec[5]);
    // So we get the memory layout of it
    // Stored as LSB

    /**
     ** cv::Mat's Logical Memory Storage
     * Iter<short>  | +0      |   +1    |   +2     |   +3   |   +4    |   +5    |
     *              .----.----.----.----.----.----.----.----.----.----.----.----.----.----.
     *              | 01 | 11 | 02 | 22 | 03 | 33 | 04 | 44 | 05 | 55 | 06 | 66 | ?? | ?? |
     *              '----'----'----'----'----'----'----'----'----'----'----'----'----'----'
     *
     ** Possible Physical Memory Storage on Heap
     * High Address
     *             .----.----.
     *             | 06 | 66 |
     *             +----+----+
     *             | 05 | 55 |
     *             +----+----+
     *             | 04 | 44 |
     *             +----+----+
     *             | 03 | 33 |
     *             .----+----+
     *             | 02 | 22 |
     *             +----+----.
     * m.data      | 01 | 11 |
     *             '----'----'
     * Low Address
     *
     ** When they are accessed as int, they will be parsed in a new way.
     * Because of it is the little endianness system, the smallest address stores the least-significant byte.
     * The first int is 0x0222'0111 and the second is 0x0333'0222
     */
}

// What is the least significant bit (LSB)? The LSB refers to the rightmost bit in a binary number representation. It
// holds the lowest value in the binary place value system, representing 2^0 or 1. In other words, the LSB is the bit
// that carries the smallest weight in a binary number.

/*
#if __cplusplus > 201703L && __cpp_lib_concepts
// libstdc++-v3/include/bits/vector.tcc
template <typename _Tp, typename _Alloc>
template <typename _InputIterator>
void vector<_Tp, _Alloc>::_M_assign_aux(_InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
    pointer __cur(this->_M_impl._M_start);
    for (; __first != __last && __cur != this->_M_impl._M_finish; ++__cur, (void)++__first) *__cur = *__first;
    if (__first == __last)
        _M_erase_at_end(__cur);
    else
        _M_range_insert(end(), __first, __last, std::__iterator_category(__first));
}

template <typename _Tp, typename _Alloc>
template <typename _ForwardIterator>
void vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
    const size_type __len = std::distance(__first, __last);

    if (__len > capacity()) {
        _S_check_init_len(__len, _M_get_Tp_allocator());
        pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
        std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _GLIBCXX_ASAN_ANNOTATE_REINIT;
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __tmp;
        this->_M_impl._M_finish = this->_M_impl._M_start + __len;
        this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    } else if (size() >= __len)
        _M_erase_at_end(std::copy(__first, __last, this->_M_impl._M_start));
    else {
        _ForwardIterator __mid = __first;
        std::advance(__mid, size());
        std::copy(__first, __mid, this->_M_impl._M_start);
        const size_type __attribute__((__unused__)) __n = __len - size();
        _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
        this->_M_impl._M_finish =
          std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
    }
}

// libstdc++-v3/include/bits/stl_uninitialized.h
// Extensions: versions of uninitialized_copy, uninitialized_fill,
//  and uninitialized_fill_n that take an allocator parameter.
//  We dispatch back to the standard versions when we're given the
//  default allocator.  For nondefault allocators we do not use
//  any of the POD optimizations.
template <typename _InputIterator, typename _ForwardIterator, typename _Allocator>
_ForwardIterator __uninitialized_copy_a(_InputIterator __first, _InputIterator __last, _ForwardIterator __result,
                                        _Allocator& __alloc) {
    _ForwardIterator __cur = __result;
    __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for (; __first != __last; ++__first, (void)++__cur)
            __traits::construct(__alloc, std::__addressof(*__cur), *__first);
        return __cur;
    }
    __catch(...) {
        std::_Destroy(__result, __cur, __alloc);
        __throw_exception_again;
    }
}

template <typename _InputIterator, typename _ForwardIterator, typename _Tp>
inline _ForwardIterator __uninitialized_copy_a(_InputIterator __first, _InputIterator __last, _ForwardIterator __result,
                                               allocator<_Tp>&) {
    return std::uninitialized_copy(__first, __last, __result);
}

template <typename _InputIterator, typename _ForwardIterator, typename _Allocator>
inline _ForwardIterator __uninitialized_move_a(_InputIterator __first, _InputIterator __last, _ForwardIterator __result,
                                               _Allocator& __alloc) {
    return std::__uninitialized_copy_a(_GLIBCXX_MAKE_MOVE_ITERATOR(__first), _GLIBCXX_MAKE_MOVE_ITERATOR(__last),
                                       __result, __alloc);
}

// libstdc++-v3/include/bits/stl_iterator.h
#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) std::make_move_iterator(_Iter)
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) std::__make_move_if_noexcept_iterator(_Iter)
#else
#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) (_Iter)
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) (_Iter)
#endif  // C++11
*/
