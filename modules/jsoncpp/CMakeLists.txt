project(jsoncpp CXX)

add_header()

set(TEST_FILES
    test_jsoncpp.cpp
)

foreach(f ${TEST_FILES})
    add_example(${f} 
        ${GTEST_BOTH_LIBRARIES}
        ${JsonCpp_LIBRARIES}
    )
endforeach()

add_footer()
