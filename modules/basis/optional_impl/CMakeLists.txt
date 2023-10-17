project(basis_optional_impl CXX)

set(FILES
    std2_optional_test.cpp
)

foreach(f ${FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()


