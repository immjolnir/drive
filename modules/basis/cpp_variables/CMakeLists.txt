project(basis_cpp_variables CXX)

# my IO examples
add_library(${PROJECT_NAME} SHARED # segement fault when using STATIC
    global_variables.cpp
)

add_example(test_global_variables.cpp
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
)

add_subdirectory(extern_vs_static)