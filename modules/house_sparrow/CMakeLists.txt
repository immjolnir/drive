project(house_sparrow CXX)

set(ALL_FILES
    tax.cpp
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()
