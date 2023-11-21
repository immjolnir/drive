project(google_gtest CXX)

set(TEST_FILES
    test_log_every_n.cpp
    test_suite.cpp
)

foreach(f ${TEST_FILES})
    add_example(${f} 
        ${GTEST_BOTH_LIBRARIES}
        ${JsonCpp_LIBRARIES}
    )
endforeach()
