project(basis CXX)

add_header()

set(STRING_FILES
    # Sequence Containers
    raw_array.cpp
    array.cpp
    vector.cpp
    string.cpp
    sstream.cpp

    # Serialize/Deserialize
    serialize_unserialize.cpp

    # YAML-cpp
    yaml_cpp.cpp
)

set(HAS_MAIN_FILES
    variadic_template.cpp
    variadic_template_v1.cpp
    filewriter_main.cpp
)

set(ALL_FILES
    ${STRING_FILES}
    ${HAS_MAIN_FILES}
)

set(RESOURCES
    resources
)
add_resource_tunnel(${RESOURCES})

# On the basis of Gtest, creating the cases.
foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()
