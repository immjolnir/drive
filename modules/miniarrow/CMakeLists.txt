project(miniarrow CXX)

add_header()

set(TEST_FILES
    employee_test.cc
    arrow/util/aligned_storage_test.cc
    arrow/util/small_vector_test.cc
)

foreach(f ${TEST_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()
