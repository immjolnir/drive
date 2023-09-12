project(boostorg CXX)

set(FILES
    filesystem.cpp
    regex.cpp
)

set(ALL_FILES
    ${FILES}
)

foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()