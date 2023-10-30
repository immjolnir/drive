project(google_protobuf)

# https://cmake.org/cmake/help/latest/module/FindProtobuf.html
# vs
# https://github.com/protocolbuffers/protobuf/blob/main/docs/cmake_protobuf_generate.md#

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
message(STATUS "PROTOBUF_FOUND: ${PROTOBUF_FOUND}")
message(STATUS "PROTOBUF_INCLUDE_DIRS: ${PROTOBUF_INCLUDE_DIRS}")
message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "PROTOBUF_LIBRARIES: ${PROTOBUF_LIBRARIES}")
message(STATUS "PROTOBUF_PROTOC_EXECUTABLE: ${PROTOBUF_PROTOC_EXECUTABLE}")
message(STATUS "PROTOBUF_PROTOC_LIBRARIES: ${PROTOBUF_PROTOC_LIBRARIES}")
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(PROTO_FILES
    proto/common/timestamp.proto
    proto/tutorial/addressbook.proto
    proto/tutorial/obstacle.proto
)

# https://github.com/protocolbuffers/protobuf/issues/1743
# https://groups.google.com/g/protobuf/c/eow2fNDUHvc
# https://github.com/elixir-protobuf/protobuf/issues/9
# https://github.com/Kitware/CMake/blob/master/Modules/FindProtobuf.cmake
#
#   ``IMPORT_DIRS``
#    A common parent directory for the schema files. For example, if the schema file is
#    ``proto/helloworld/helloworld.proto`` and the import directory ``proto/`` then the
#    generated files are ``${PROTOC_OUT_DIR}/helloworld/helloworld.pb.h`` and
#    ``${PROTOC_OUT_DIR}/helloworld/helloworld.pb.cc`
#
# A hierarchy (e.g. google.protobuf manifest as google/protobuf/...) of related Packages is independent of its file system location.
# https://stackoverflow.com/questions/69535429/what-is-the-relationship-between-proto-package-and-actual-directory-structure
#
# build/modules/google_protobuf/proto/
# ├── common
# │   ├── timestamp_pb2.py
# │   ├── timestamp.pb.cc
# │   └── timestamp.pb.h
# └── tutorial
#     ├── addressbook_pb2.py
#     ├── addressbook.pb.cc
#     └── addressbook.pb.h

unset(PROTOBUF_GENERATE_CPP_APPEND_PATH)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS ${PROTO_FILES}
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} STATIC
        ${PROTO_HDRS}
        ${PROTO_SRCS}
)
target_include_directories(${PROJECT_NAME} BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(${PROJECT_NAME}
    ${PROTOBUF_LIBRARIES}
)

add_executable(pb_example
    pb_example.cpp
)

target_link_libraries(pb_example
    ${PROJECT_NAME}
    ${Protobuf_LIBRARIES}
)

SET(BUILT_INS
    builtin_repeated_field.cpp
    builtin_coded_stream.cpp
)

set(TEST_SOURCES
    test_addressbook.cpp
    test_obstacle.cpp
    test_timestamp.cpp
)
set(ALL_TESTS
    ${TEST_SOURCES}
    ${BUILT_INS}
)

foreach(file ${ALL_TESTS})
    add_testcase(${file})
endforeach()