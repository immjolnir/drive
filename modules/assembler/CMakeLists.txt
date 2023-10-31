project(assembler CXX)

set(ALL_FILES
    test_ptr_increment.cpp
)

foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()