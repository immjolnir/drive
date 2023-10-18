project(leetcode CXX)

add_header()

set(TEST_FILES
    31.cpp
)

set(ALL_FILES
    ${TEST_FILES}
)

foreach(f ${ALL_FILES})
    add_example(${f}
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
    )
endforeach()

add_footer()