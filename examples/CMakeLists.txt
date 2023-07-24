project(examples CXX)

set(EXAMPLE_SOURCES
    employee.cpp
)

set(ALL_FILES
    ${EXAMPLE_SOURCES}
)

foreach(file ${ALL_FILES})
    add_example(${file} demo_project)
endforeach()