project(google_gtest CXX)

set(TEST_FILES
    gtest_handing_exception.cpp
    test_log_every_n.cpp
    test_suite.cpp
    test_IEEE_754.cpp
    test_death.cpp

    # test_matcher_impl.cpp
)

foreach(f ${TEST_FILES})
    add_example(${f} 
        ${GTEST_BOTH_LIBRARIES}
        ${JsonCpp_LIBRARIES}
    )
endforeach()
