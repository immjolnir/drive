project(learnopencv CXX)

add_header()

set(SOURCES_DIR ${CATCH_DIR}/${PROJECT_NAME})

# define the sources of the OpenCV APIs
# Please keep these ordered alphabetically
set(OPENCV_API_EXAMPLES
    minMaxLoc.cpp
)

set(HOG_EXAMPLES
    hog/visualize_hog_descriptor.cpp
)

set(ALL_FILES
    ${OPENCV_API_EXAMPLES}
    ${HOG_EXAMPLES}
)

foreach(file ${ALL_FILES})
    add_example(${file})
endforeach()

set(TEST_FILES
    test_storage_order_opencv_vs_eigen.cpp
    test_mat.cpp
    test_mat_ctor_vs_operator.cpp
    test_mat_vs_vector.cpp
)

foreach(file ${TEST_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()

set(HAS_MAIN_FILES
    applyColorMap.cpp
    bgr2gray.cpp
)

foreach(file ${HAS_MAIN_FILES})
    add_example(${file} ${GTEST_LIBRARIES})
endforeach()

add_footer()