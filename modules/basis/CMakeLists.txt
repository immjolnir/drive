project(basis CXX)

add_header()

set(STRING_FILES
    # Sequence Containers
    raw_array.cpp
    array.cpp
    vector.cpp
    sstream.cpp

    # String
    string.cpp
    string_split.cpp

    # Serialize/Deserialize
    serialize_unserialize.cpp

    # C++ Specifiers
    const_specifier_removed.cpp
    casting_type.cpp

    # io
    iomanip.cpp

    # YAML-cpp
    yaml_cpp.cpp
)

set(HAS_MAIN_FILES
    variadic_template.cpp
    variadic_template_v1.cpp
    filewriter_main.cpp
    float-issue-precision.cpp
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
