project(basis_myio CXX)

# my IO examples
add_library(${PROJECT_NAME} SHARED # segement fault when using STATIC
    globals.cpp
    io_impl.cpp
)

add_example(test_io.cpp
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
