project(basis CXX)

add_header()

set(STRING_FILES
    string.cpp
    sstream.cpp
)

set(ALL_FILES
    ${STRING_FILES}
)

# On the basis of Gtest, creating the cases.
foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()
