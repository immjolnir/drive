#pragma once
/*
/usr/include/eigen3/Eigen/src/Core/AssignEvaluator.h:786:1: internal compiler error: Segmentation fault
  786 | }
      | ^
0x7f714b44951f ???
        ./signal/../sysdeps/unix/sysv/linux/x86_64/libc_sigaction.c:0
0x7f714b430d8f __libc_start_call_main
        ../sysdeps/nptl/libc_start_call_main.h:58
0x7f714b430e3f __libc_start_main_impl
        ../csu/libc-start.c:392
Please submit a full bug report,
with preprocessed source if appropriate.
Please include the complete backtrace with any bug report.
See <file:///usr/share/doc/gcc-11/README.Bugs> for instructions.

It's hard to compile eigen dependent project.
Here we use the explicit template instantiation trick to lower the compilation time.
*/

#include <Eigen/Core>
#include <Eigen/Geometry>

template <typename Scalar>
struct EigenTypes {
    typedef Eigen::Matrix<Scalar, 3, 3> Matrix3;
    typedef Eigen::Matrix<Scalar, 4, 4> Matrix4;
    typedef Eigen::Matrix<Scalar, 2, 1> Vector2;
    typedef Eigen::Matrix<Scalar, 3, 1> Vector3;
    typedef Eigen::Matrix<Scalar, 4, 1> Vector4;
    typedef Eigen::Quaternion<Scalar> Quaternionx;
    typedef Eigen::AngleAxis<Scalar> AngleAxisx;

    // typedef Eigen::Transform<Scalar, 2, Mode, Options> Transform2;
    // typedef Eigen::Transform<Scalar, 3, Mode, Options> Transform3;
    // typedef typename Transform3::MatrixType MatrixType;

    typedef Eigen::DiagonalMatrix<Scalar, 3> AlignedScaling3;
    typedef Eigen::Translation<Scalar, 2> Translation2;
    typedef Eigen::Translation<Scalar, 3> Translation3;
};

using Eigenxd = EigenTypes<double>;
using Eigenxf = EigenTypes<float>;