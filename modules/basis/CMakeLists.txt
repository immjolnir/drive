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
    initial_values.cpp

    union.cpp

    # Alignment
    trivial.cpp
    alignment.cpp
    alignment_alignas.cpp
    alignment_aligned_storage.cpp

    new_operator.cpp
    replacement_new_operator.cpp
    replacement_new_operator_dont_complain_about_out_of_bounds.cpp
    replacement_new_operator_dont_complain_about_out_of_bounds_with_fix.cpp

    # atomic
    atomic_exploring.cpp

    # Sequence Containers
    raw_array.cpp
    array.cpp
    vector.cpp
    vector-test-deep_copy-vs-shadow_copy.cpp
    sstream.cpp
    list.cpp
    forward_list.cpp

    # numeric library
    numeric.cpp

    # New API
    launder.cpp
    destroy.cpp

    # String
    string.cpp
    string_split.cpp
    string_size_length_capacity.cpp

    # C++ Specifiers
    qualifiers.cpp
    type_casting0.cpp
    type_casting1.cpp
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


    filesystem.cpp
)

set(RESOURCES
    resources
)
add_resource_tunnel(${RESOURCES})

# On the basis of Gtest, creating the cases.
foreach(file ${ALL_FILES})
    add_example(${file}
        ${GTEST_BOTH_LIBRARIES}
        # /usr/include/c++/11/atomic:361: error: undefined reference to '__atomic_compare_exchange_16'
        atomic
    )
endforeach()


add_subdirectory(iterator)
add_subdirectory(myio)
add_subdirectory(cpp_variables)
add_subdirectory(zero-copy)
add_subdirectory(memory_debugger)
add_subdirectory(optional_impl)

add_subdirectory(stl_algorithm)
add_subdirectory(template)

add_footer()
