project(logging CXX)

set(TEST_FILES
    test_udl.cpp
)

set(ALL_FILES
    ${TEST_FILES}
)

foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()