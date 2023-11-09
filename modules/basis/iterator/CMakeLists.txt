project(basis_iterator CXX)

set(ALL_FILES
    swap.cpp
    iter_swap.cpp
    iterator_category_part1.cpp
    iterator_category_part2.cpp

    read_whole_file.cpp
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()

