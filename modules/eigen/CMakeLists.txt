project(eigen CXX)

add_header()

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "EIGEN3_VERSION: ${EIGEN3_VERSION}")

add_library(${PROJECT_NAME} STATIC
    eigen_explicit_template_instantiation.cpp
)

set(TEST_FILES
    test_matrix.cpp
    test_geometry.cpp
    test_geometry_euler_angle.cpp
)

# On the basis of Gtest, creating the cases.
foreach(file ${TEST_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()
