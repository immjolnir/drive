project(examples CXX)

set(EXAMPLE_SOURCES
    employee.cpp
)

set(ALL_FILES
    ${EXAMPLE_SOURCES}
)

foreach(file ${ALL_FILES})
    add_example(${file}
        common
        demo_project
        ${GFLAGS_LIBRARIES}
        ${GLOG_LIBRARIES}
    )
endforeach()