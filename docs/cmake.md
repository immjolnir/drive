# cmake

## Define a preprocessor macro through cmake

- https://stackoverflow.com/questions/9017573/define-preprocessor-macro-through-cmake
  - using [add_definitions](https://cmake.org/cmake/help/latest/command/add_definitions.html)
  ```
  add_definitions(-DFOO -DBAR ...)
  add_definitions( -DBOOST_FILESYSTEM_VERSION=2 )
  ```

  - using [add_compile_definitions](https://cmake.org/cmake/help/latest/command/add_compile_definitions.html)
    ```
    add_compile_definitions(OPENCV_VERSION=${OpenCV_VERSION})
    add_compile_definitions(WITH_OPENCV2)
    ```

  - using [target_compile_definitions](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html)
  ```
  target_compile_definitions(foo PUBLIC FOO)
  target_compile_definitions(my_target PRIVATE FOO=1 BAR=1)
  ```