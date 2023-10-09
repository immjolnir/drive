project(miniarrow CXX)

add_header()

set(TEST_FILES
    test_employee.cpp
)

foreach(f ${TEST_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()
