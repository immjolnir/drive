project(boostorg CXX)

set(FILES
    boost_filesystem.cpp
    boost_regex.cpp
    boost_math_factorial_and_binomial_coefficient.cpp
    #boost_serialization.cpp
    boost_pfr.cpp
    boost_algorithm_string.cpp
)

set(ALL_FILES
    ${FILES}
)

foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_example(boost_preprocessor.cpp ${GTEST_LIBRARIES})
