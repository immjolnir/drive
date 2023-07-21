project(examples CXX)

set(SOURCES_DIR ${CATCH_DIR}/examples)

set(EXAMPLE_SOURCES
    ${SOURCES_DIR}/employee.cpp
)
# define the sources of the OpenCV APIs
# Please keep these ordered alphabetically
set(OPENCV_API_EXAMPLES 
    ${SOURCES_DIR}/minMaxLoc.cpp
)

set(ALL_FILES
    ${EXAMPLE_SOURCES}
    ${OPENCV_API_EXAMPLES}
)

foreach(file ${ALL_FILES})
    add_example(${file} demo_project)
endforeach()