project(three_dimensions CXX)

add_header()

# https://blog.csdn.net/me1171115772/article/details/106952865
find_package(Threads)


add_library(${PROJECT_NAME} STATIC
    rotation3d.cpp
)

set(TEST_SOURCES
    test_3d.cpp
    test_euler_angles_and_rotation_matrices.cpp
)

# Adds unit tests, and if enabled, cmake project tests.
message(STATUS "Adding tests under ${PROJECT_NAME}Tests...")
foreach(file ${TEST_SOURCES})
    add_testcase(${file})
endforeach()

add_footer()
