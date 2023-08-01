project(exploring CXX)

add_header()

set(FMT_FILES
    fmt/numberic_format.cpp
)

foreach(file ${FMT_FILES})
    add_example(${file}
        fmt
        ${GTEST_BOTH_LIBRARIES}
    )
endforeach()

add_footer()
