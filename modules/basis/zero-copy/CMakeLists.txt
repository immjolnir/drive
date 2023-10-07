project(basis_zero_copy CXX)

add_example(test_arrow.cpp
    ${GTEST_BOTH_LIBRARIES}
)
