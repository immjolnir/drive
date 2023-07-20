project(examples CXX)

set(SOURCES_DIR ${CATCH_DIR}/examples)

set(EXAMPLE_SOURCES
    ${SOURCES_DIR}/employee.cpp
)

foreach(file ${EXAMPLE_SOURCES})
    add_example(${file} demo_project)
endforeach()
