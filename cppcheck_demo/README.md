# CppCheck Demo Project

```
# mkdir build
# cd build/
# cmake ..
# make
cppcheck_demo/main.cpp:4:6: error: Array 'a[10]' accessed at index 10, which is out of bounds. [arrayIndexOutOfBounds]
    a[10] = 0;
     ^
```
