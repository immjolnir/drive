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

add_footer()