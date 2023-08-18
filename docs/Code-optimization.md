# Code Optimization

* Keywords: Optimizing C++, Code optimization, Compile time optimization


- When possible put code that is reused into `.cpp` files and compile as reusable libraries instead of placing more code in header files to minimize the amount of redundant compilation.
  - 控制每个编译单元的代码行数，尤其是 header file

- Use templates to speed up compile time by compiling the parts of the interface that you use rather than compiling every possibility all the time, one particular use case here is for macros.
  - instantial 实例化出现次数较多的模板类

- Minimize included code to only include the exact features you need, rather than using catch all include files

- Use forward declarations to avoid including files because it completely eliminates redundant compilation of code.
  - raw pointer: `fwdcls *` 
  - smart pointer: `std::shared_ptr<fwdcls>` or `std::unique_ptr<fwdcls>`




