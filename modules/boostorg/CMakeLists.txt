project(boostorg CXX)

set(FILES
    boost_filesystem.cpp
    boost_regex.cpp
    boost_math_factorial_and_binomial_coefficient.cpp
    boost_serialization.cpp
)

set(ALL_FILES
    ${FILES}
)

foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()