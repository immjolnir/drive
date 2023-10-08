project(basis CXX)

add_header()

set(GTEST_EXAMPLES
    gtest_handing_exception.cpp
)

set(EXCEPTION_EXAMPLES
    exception_stardardarized_example.cpp
    exception_throw_any_type.cpp
    exception_api_current_exception.cpp
)

set(STRING_FILES
    # Sequence Containers
    raw_array.cpp
    array.cpp
    vector.cpp
    vector-test-deep_copy-vs-shadow_copy.cpp
    sstream.cpp
    list.cpp

    # String
    string.cpp
    string_split.cpp
    string_size_length_capacity.cpp

    # Serialize/Deserialize
    serialize_unserialize.cpp

    # C++ Specifiers
    qualifiers.cpp
    casting_type.cpp
    type_conversion_dynamic_cast.cpp

    # io
    iomanip.cpp

    # YAML-cpp
    yaml_cpp.cpp
)

set(BIT_FILES
    bit.cpp
    machine_word.cpp
)

set(HAS_MAIN_FILES
    variadic_template.cpp
    variadic_template_v1.cpp
    filewriter_main.cpp
    float-issue-precision.cpp
)

set(ALL_FILES
    ${GTEST_EXAMPLES}
    ${STRING_FILES}
    ${EXCEPTION_EXAMPLES}
    ${HAS_MAIN_FILES}
    ${BIT_FILES}

    test_steadytime.cpp
)

set(RESOURCES
    resources
)
add_resource_tunnel(${RESOURCES})

# On the basis of Gtest, creating the cases.
foreach(file ${ALL_FILES})
    add_example(${file} ${GTEST_BOTH_LIBRARIES})
endforeach()


add_subdirectory(myio)
add_subdirectory(cpp_variables)
add_subdirectory(zero-copy)

add_footer()
