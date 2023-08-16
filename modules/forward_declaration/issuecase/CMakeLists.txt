cmake_minimum_required(VERSION 3.22)
project(fwd_issuecase CXX)

# Create library
add_library(${PROJECT_NAME} STATIC
    src/myclass.cpp
)
target_include_directories(${PROJECT_NAME} BEFORE
   PUBLIC
       $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(${PROJECT_NAME}_myclass_size STATIC
    src/myclass_size.cpp
)
target_include_directories(${PROJECT_NAME}_myclass_size BEFORE
   PUBLIC
       $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

## Create main entry
add_executable(${PROJECT_NAME}_main src/main.cpp)
target_include_directories(${PROJECT_NAME}_main BEFORE
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(${PROJECT_NAME}_main
    ${PROJECT_NAME}
)
