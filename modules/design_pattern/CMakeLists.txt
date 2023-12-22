project(design_pattern CXX)

add_example_args(
    Target
      test_fred
    Destination
      reference_counting
    Sources
        reference_counting/shared_ptr/fred.h
        reference_counting/shared_ptr/fred.cpp
        reference_counting/shared_ptr/test_fred.cpp
    Libs
        ${GTEST_BOTH_LIBRARIES}
)

set(COPY_ON_WRITE_PATTERN
)

set(ALL_FILES
    ${COPY_ON_WRITE_PATTERN}
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()
