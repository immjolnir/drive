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


# Reduce Compilation Time
Speedup compilation time ? Precompiled header ? c++20 modules ? extern template ?

- Precompiling Eigen is a good idea. Something like this would compile:
  - https://gitlab.com/libeigen/eigen/-/issues/1920
  - 

```c++
template class Eigen::EigenBase<Eigen::MatrixXf>;

template class Eigen::EigenBase<
    Eigen::CwiseBinaryOp<Eigen::internal::scalar_sum_op<float, float>,
                         Eigen::MatrixXf const, Eigen::MatrixXf const>>;
```


## [C++ is too slow to compile, can you share all your tips to lower compilation time ?](https://www.reddit.com/r/cpp/comments/hj66pd/c_is_too_slow_to_compile_can_you_share_all_your/)
C++ compilation time is killing my productivity and will probably by my reason of not using this language anymore.
Can you share any tips that you used to lower the compilation time of your c++ projects ?

Anything is welcome: optimizing any step of the compilation (header, template instantiation, link, ...), tools, hardware, build systems, not using expensive language features, compilation time profiler ...

> The trick to reduce to compilation time is the same as the trick to reduce runtime - you figure out what's slowing you down. For compilation using clang, https://aras-p.info/blog/2019/01/16/time-trace-timeline-flame-chart-profiler-for-Clang/ is a very handy tool. 

> Use ccache, pimpl, and maybe forward declarations and `extern template`.

> Last thing to keep in mind is templates. Try to avoid crazy template metaprogramming, and think of using extern templates for something very common in your app (like std::vector<int> for example).

* https://cmake.org/cmake/help/latest/command/target_precompile_headers.html
* https://edgarluque.com/blog/cmake-precompiled-headers/
* https://stackoverflow.com/questions/148570/using-pre-compiled-headers-with-cmake
* https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html

这个实现了
* https://github.com/google/flatbuffers/issues/5823
  - https://github.com/google/flatbuffers/pull/5827

* https://github.com/stan-dev/cmdstan/pull/875/files
  * [stan](https://mc-stan.org/): Stan is a state-of-the-art platform for statistical modeling and high-performance statistical computation. Thousands of users rely on Stan for statistical modeling, data analysis, and prediction in the social, biological, and physical sciences, engineering, and business.


* https://stackoverflow.com/questions/28968631/how-to-write-a-third-party-library-wrapper-class-around-expression-templates



- https://docs.oracle.com/cd/E19205-01/819-5267/bkafi/index.html

Template Instantiation involves generating a concrete class or function (instance) for a particular combination of template arguments.
For example, the compiler generates a class for `Array<int>` and a different class for `Array<double>`.

The new classes are defined by substituting the template arguments for the template parameters in the definition of the template class.
In the `Array<int>` example, shown in the preceding “Class Templates” section, the compiler substitutes `int` wherever Elem appears.

- Implicit Template Instantiation
The use of a template function or template class introduces the need for an instance.
If that instance does not already exist, the compiler implicitly instantiates the template for that combination of template arguments.

- Explicit Template Instantiation
The compiler implicitly instantiates templates only for those combinations of template arguments that are actually used.
This approach may be inappropriate for the construction of libraries that provide templates.
C++ provides a facility to explicitly instantiate templates, as seen in the following examples.

  - Explicit Instantiation of Template Functions

To instantiate a template function explicitly, follow the template keyword by a declaration (not definition) for the function, with the function identifier followed by the template arguments.

    template float twice<float>(float original);

Template arguments may be omitted when the compiler can infer them.

    template int twice(int original);


  - Explicit Instantiation of Template Classes

To instantiate a template class explicitly, follow the template keyword by a declaration (not definition) for the class, with the class identifier followed by the template arguments.

  template class Array<char>;

  template class String<19>;

When you explicitly instantiate a class, all of its members are also instantiated.

  - Explicit Instantiation of Template Class Function Members

To explicitly instantiate a template class function member, follow the template keyword by a declaration (not definition) for the function, with the function identifier qualified by the template class, followed by the template arguments.


  template int Array<char>::GetSize();

  template int String<19>::length();

  - Explicit Instantiation of Template Class Static Data Members

To explicitly instantiate a template class static data member, follow the template keyword by a declaration (not definition) for the member, with the member identifier qualified by the template class, followed by the template argument.

  template int String<19>::overflows;

在 eigen里面就这么用的, 即
  # 504 "/usr/include/eigen3/Eigen/src/Core/Matrix.h"
  typedef Matrix<int, 2, 2> Matrix2i;
  typedef Matrix<int, 2, 1> Vector2i;
  typedef Matrix<int, 1, 2> RowVector2i;
  typedef Matrix<int, 3, 3> Matrix3i;
  typedef Matrix<int, 3, 1> Vector3i;
  typedef Matrix<int, 1, 3> RowVector3i;
  typedef Matrix<int, 4, 4> Matrix4i;
  typedef Matrix<int, 4, 1> Vector4i;
  typedef Matrix<int, 1, 4> RowVector4i;
  typedef Matrix<int, Dynamic, Dynamic> MatrixXi;
  typedef Matrix<int, Dynamic, 1> VectorXi;
  typedef Matrix<int, 1, Dynamic> RowVectorXi;

但需要提前 precompile它们才可以达到目的生效.
