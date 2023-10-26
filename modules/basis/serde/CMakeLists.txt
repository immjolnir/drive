project(basis_serde CXX)

add_header()

set(TEST_FILES
    serde_vector.cpp
    steady_time.cpp
)

set(ALL_FILES
    ${TEST_FILES}
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()