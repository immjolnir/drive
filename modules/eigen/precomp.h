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

// Precompiled headers
// Eigen 核心部分
#include <Eigen/Core>

// 稠密矩阵的代数运算（逆，特征值等）
#include <Eigen/Dense>

#include <Eigen/Geometry>

// Pre-explicit template instantiations
/*
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
typedef Matrix<int, 2, Dynamic> Matrix2Xi;
typedef Matrix<int, Dynamic, 2> MatrixX2i;
typedef Matrix<int, 3, Dynamic> Matrix3Xi;
typedef Matrix<int, Dynamic, 3> MatrixX3i;
typedef Matrix<int, 4, Dynamic> Matrix4Xi;
typedef Matrix<int, Dynamic, 4> MatrixX4i;
typedef Matrix<float, 2, 2> Matrix2f;
typedef Matrix<float, 2, 1> Vector2f;
typedef Matrix<float, 1, 2> RowVector2f;
typedef Matrix<float, 3, 3> Matrix3f;
typedef Matrix<float, 3, 1> Vector3f;
typedef Matrix<float, 1, 3> RowVector3f;
typedef Matrix<float, 4, 4> Matrix4f;
typedef Matrix<float, 4, 1> Vector4f;
typedef Matrix<float, 1, 4> RowVector4f;
typedef Matrix<float, Dynamic, Dynamic> MatrixXf;
typedef Matrix<float, Dynamic, 1> VectorXf;
typedef Matrix<float, 1, Dynamic> RowVectorXf;
typedef Matrix<float, 2, Dynamic> Matrix2Xf;
typedef Matrix<float, Dynamic, 2> MatrixX2f;
typedef Matrix<float, 3, Dynamic> Matrix3Xf;
typedef Matrix<float, Dynamic, 3> MatrixX3f;
typedef Matrix<float, 4, Dynamic> Matrix4Xf;
typedef Matrix<float, Dynamic, 4> MatrixX4f;
typedef Matrix<double, 2, 2> Matrix2d;
typedef Matrix<double, 2, 1> Vector2d;
typedef Matrix<double, 1, 2> RowVector2d;
typedef Matrix<double, 3, 3> Matrix3d;
typedef Matrix<double, 3, 1> Vector3d;
typedef Matrix<double, 1, 3> RowVector3d;
typedef Matrix<double, 4, 4> Matrix4d;
typedef Matrix<double, 4, 1> Vector4d;
typedef Matrix<double, 1, 4> RowVector4d;
typedef Matrix<double, Dynamic, Dynamic> MatrixXd;
typedef Matrix<double, Dynamic, 1> VectorXd;
typedef Matrix<double, 1, Dynamic> RowVectorXd;
typedef Matrix<double, 2, Dynamic> Matrix2Xd;
typedef Matrix<double, Dynamic, 2> MatrixX2d;
typedef Matrix<double, 3, Dynamic> Matrix3Xd;
typedef Matrix<double, Dynamic, 3> MatrixX3d;
typedef Matrix<double, 4, Dynamic> Matrix4Xd;
typedef Matrix<double, Dynamic, 4> MatrixX4d;
typedef Matrix<std::complex<float>, 2, 2> Matrix2cf;
typedef Matrix<std::complex<float>, 2, 1> Vector2cf;
typedef Matrix<std::complex<float>, 1, 2> RowVector2cf;
typedef Matrix<std::complex<float>, 3, 3> Matrix3cf;
typedef Matrix<std::complex<float>, 3, 1> Vector3cf;
typedef Matrix<std::complex<float>, 1, 3> RowVector3cf;
typedef Matrix<std::complex<float>, 4, 4> Matrix4cf;
typedef Matrix<std::complex<float>, 4, 1> Vector4cf;
typedef Matrix<std::complex<float>, 1, 4> RowVector4cf;
typedef Matrix<std::complex<float>, Dynamic, Dynamic> MatrixXcf;
typedef Matrix<std::complex<float>, Dynamic, 1> VectorXcf;
typedef Matrix<std::complex<float>, 1, Dynamic> RowVectorXcf;
typedef Matrix<std::complex<float>, 2, Dynamic> Matrix2Xcf;
typedef Matrix<std::complex<float>, Dynamic, 2> MatrixX2cf;
typedef Matrix<std::complex<float>, 3, Dynamic> Matrix3Xcf;
typedef Matrix<std::complex<float>, Dynamic, 3> MatrixX3cf;
typedef Matrix<std::complex<float>, 4, Dynamic> Matrix4Xcf;
typedef Matrix<std::complex<float>, Dynamic, 4> MatrixX4cf;
typedef Matrix<std::complex<double>, 2, 2> Matrix2cd;
typedef Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef Matrix<std::complex<double>, 1, 2> RowVector2cd;
typedef Matrix<std::complex<double>, 3, 3> Matrix3cd;
typedef Matrix<std::complex<double>, 3, 1> Vector3cd;
typedef Matrix<std::complex<double>, 1, 3> RowVector3cd;
typedef Matrix<std::complex<double>, 4, 4> Matrix4cd;
typedef Matrix<std::complex<double>, 4, 1> Vector4cd;
typedef Matrix<std::complex<double>, 1, 4> RowVector4cd;
typedef Matrix<std::complex<double>, Dynamic, Dynamic> MatrixXcd;
typedef Matrix<std::complex<double>, Dynamic, 1> VectorXcd;
typedef Matrix<std::complex<double>, 1, Dynamic> RowVectorXcd;
typedef Matrix<std::complex<double>, 2, Dynamic> Matrix2Xcd;
typedef Matrix<std::complex<double>, Dynamic, 2> MatrixX2cd;
typedef Matrix<std::complex<double>, 3, Dynamic> Matrix3Xcd;
typedef Matrix<std::complex<double>, Dynamic, 3> MatrixX3cd;
typedef Matrix<std::complex<double>, 4, Dynamic> Matrix4Xcd;
typedef Matrix<std::complex<double>, Dynamic, 4> MatrixX4cd;

// # 540 "/usr/include/eigen3/Eigen/src/Core/Matrix.h"
template <typename Type>
using Matrix2 = Matrix<Type, 2, 2>;
template <typename Type>
using Vector2 = Matrix<Type, 2, 1>;
template <typename Type>
using RowVector2 = Matrix<Type, 1, 2>;
template <typename Type>
using Matrix3 = Matrix<Type, 3, 3>;
template <typename Type>
using Vector3 = Matrix<Type, 3, 1>;
template <typename Type>
using RowVector3 = Matrix<Type, 1, 3>;
template <typename Type>
using Matrix4 = Matrix<Type, 4, 4>;
template <typename Type>
using Vector4 = Matrix<Type, 4, 1>;
template <typename Type>
using RowVector4 = Matrix<Type, 1, 4>;
template <typename Type>
using MatrixX = Matrix<Type, Dynamic, Dynamic>;
template <typename Type>
using VectorX = Matrix<Type, Dynamic, 1>;
template <typename Type>
using RowVectorX = Matrix<Type, 1, Dynamic>;
template <typename Type>
using Matrix2X = Matrix<Type, 2, Dynamic>;
template <typename Type>
using MatrixX2 = Matrix<Type, Dynamic, 2>;
template <typename Type>
using Matrix3X = Matrix<Type, 3, Dynamic>;
template <typename Type>
using MatrixX3 = Matrix<Type, Dynamic, 3>;
template <typename Type>
using Matrix4X = Matrix<Type, 4, Dynamic>;
template <typename Type>
using MatrixX4 = Matrix<Type, Dynamic, 4>;

# 352 "/usr/include/eigen3/Eigen/src/Core/Array.h"
typedef Array<int, 2, 2> Array22i;
typedef Array<int, 2, 1> Array2i;
typedef Array<int, 3, 3> Array33i;
typedef Array<int, 3, 1> Array3i;
typedef Array<int, 4, 4> Array44i;
typedef Array<int, 4, 1> Array4i;
typedef Array<int, Dynamic, Dynamic> ArrayXXi;
typedef Array<int, Dynamic, 1> ArrayXi;
typedef Array<int, 2, Dynamic> Array2Xi;
typedef Array<int, Dynamic, 2> ArrayX2i;
typedef Array<int, 3, Dynamic> Array3Xi;
typedef Array<int, Dynamic, 3> ArrayX3i;
typedef Array<int, 4, Dynamic> Array4Xi;
typedef Array<int, Dynamic, 4> ArrayX4i;
typedef Array<float, 2, 2> Array22f;
typedef Array<float, 2, 1> Array2f;
typedef Array<float, 3, 3> Array33f;
typedef Array<float, 3, 1> Array3f;
typedef Array<float, 4, 4> Array44f;
typedef Array<float, 4, 1> Array4f;
typedef Array<float, Dynamic, Dynamic> ArrayXXf;
typedef Array<float, Dynamic, 1> ArrayXf;
typedef Array<float, 2, Dynamic> Array2Xf;
typedef Array<float, Dynamic, 2> ArrayX2f;
typedef Array<float, 3, Dynamic> Array3Xf;
typedef Array<float, Dynamic, 3> ArrayX3f;
typedef Array<float, 4, Dynamic> Array4Xf;
typedef Array<float, Dynamic, 4> ArrayX4f;
typedef Array<double, 2, 2> Array22d;
typedef Array<double, 2, 1> Array2d;
typedef Array<double, 3, 3> Array33d;
typedef Array<double, 3, 1> Array3d;
typedef Array<double, 4, 4> Array44d;
typedef Array<double, 4, 1> Array4d;
typedef Array<double, Dynamic, Dynamic> ArrayXXd;
typedef Array<double, Dynamic, 1> ArrayXd;
typedef Array<double, 2, Dynamic> Array2Xd;
typedef Array<double, Dynamic, 2> ArrayX2d;
typedef Array<double, 3, Dynamic> Array3Xd;
typedef Array<double, Dynamic, 3> ArrayX3d;
typedef Array<double, 4, Dynamic> Array4Xd;
typedef Array<double, Dynamic, 4> ArrayX4d;
typedef Array<std::complex<float>, 2, 2> Array22cf;
typedef Array<std::complex<float>, 2, 1> Array2cf;
typedef Array<std::complex<float>, 3, 3> Array33cf;
typedef Array<std::complex<float>, 3, 1> Array3cf;
typedef Array<std::complex<float>, 4, 4> Array44cf;
typedef Array<std::complex<float>, 4, 1> Array4cf;
typedef Array<std::complex<float>, Dynamic, Dynamic> ArrayXXcf;
typedef Array<std::complex<float>, Dynamic, 1> ArrayXcf;
typedef Array<std::complex<float>, 2, Dynamic> Array2Xcf;
typedef Array<std::complex<float>, Dynamic, 2> ArrayX2cf;
typedef Array<std::complex<float>, 3, Dynamic> Array3Xcf;
typedef Array<std::complex<float>, Dynamic, 3> ArrayX3cf;
typedef Array<std::complex<float>, 4, Dynamic> Array4Xcf;
typedef Array<std::complex<float>, Dynamic, 4> ArrayX4cf;
typedef Array<std::complex<double>, 2, 2> Array22cd;
typedef Array<std::complex<double>, 2, 1> Array2cd;
typedef Array<std::complex<double>, 3, 3> Array33cd;
typedef Array<std::complex<double>, 3, 1> Array3cd;
typedef Array<std::complex<double>, 4, 4> Array44cd;
typedef Array<std::complex<double>, 4, 1> Array4cd;
typedef Array<std::complex<double>, Dynamic, Dynamic> ArrayXXcd;
typedef Array<std::complex<double>, Dynamic, 1> ArrayXcd;
typedef Array<std::complex<double>, 2, Dynamic> Array2Xcd;
typedef Array<std::complex<double>, Dynamic, 2> ArrayX2cd;
typedef Array<std::complex<double>, 3, Dynamic> Array3Xcd;
typedef Array<std::complex<double>, Dynamic, 3> ArrayX3cd;
typedef Array<std::complex<double>, 4, Dynamic> Array4Xcd;
typedef Array<std::complex<double>, Dynamic, 4> ArrayX4cd;

# 41 "/usr/include/eigen3/Eigen/src/Geometry/AngleAxis.h" 3
typedef AngleAxis<float> AngleAxisf;

typedef AngleAxis<double> AngleAxisd;

# 518 "/usr/include/eigen3/Eigen/src/Geometry/Transform.h" 3
typedef Transform<float, 2, Isometry> Isometry2f;
typedef Transform<float, 3, Isometry> Isometry3f;
typedef Transform<double, 2, Isometry> Isometry2d;
typedef Transform<double, 3, Isometry> Isometry3d;
typedef Transform<float, 2, Affine> Affine2f;
typedef Transform<float, 3, Affine> Affine3f;
typedef Transform<double, 2, Affine> Affine2d;
typedef Transform<double, 3, Affine> Affine3d;
typedef Transform<float, 2, AffineCompact> AffineCompact2f;
typedef Transform<float, 3, AffineCompact> AffineCompact3f;
typedef Transform<double, 2, AffineCompact> AffineCompact2d;
typedef Transform<double, 3, AffineCompact> AffineCompact3d;
typedef Transform<float, 2, Projective> Projective2f;
typedef Transform<float, 3, Projective> Projective3f;
typedef Transform<double, 2, Projective> Projective2d;
typedef Transform<double, 3, Projective> Projective3d;

# 29 "/usr/include/eigen3/Eigen/src/Geometry/Translation.h" 3

typedef Translation<float, 2> Translation2f;
typedef Translation<double, 2> Translation2d;
typedef Translation<float, 3> Translation3f;
typedef Translation<double, 3> Translation3d;

# 477 "/usr/include/eigen3/Eigen/src/Geometry/AlignedBox.h" 3
typedef AlignedBox<int, 1> AlignedBox1i;
typedef AlignedBox<int, 2> AlignedBox2i;
typedef AlignedBox<int, 3> AlignedBox3i;
typedef AlignedBox<int, 4> AlignedBox4i;
typedef AlignedBox<int, Dynamic> AlignedBoxXi;
typedef AlignedBox<float, 1> AlignedBox1f;
typedef AlignedBox<float, 2> AlignedBox2f;
typedef AlignedBox<float, 3> AlignedBox3f;
typedef AlignedBox<float, 4> AlignedBox4f;
typedef AlignedBox<float, Dynamic> AlignedBoxXf;
typedef AlignedBox<double, 1> AlignedBox1d;
typedef AlignedBox<double, 2> AlignedBox2d;
typedef AlignedBox<double, 3> AlignedBox3d;
typedef AlignedBox<double, 4> AlignedBox4d;
typedef AlignedBox<double, Dynamic> AlignedBoxXd;

# 32 "/usr/include/eigen3/Eigen/src/Geometry/Rotation2D.h" 3
typedef Rotation2D<float> Rotation2Df;
typedef Rotation2D<double> Rotation2Dd;

bool isApprox(const Rotation2D& other,
              const typename NumTraits<Scalar>::Real& prec = NumTraits<Scalar>::dummy_precision()) const {
    return internal::isApprox(m_angle, other.m_angle, prec);
}

inline Rotation2D slerp(const Scalar& t, const Rotation2D& other) const {
    Scalar dist = Rotation2D(other.m_angle - m_angle).smallestAngle();
    return Rotation2D(m_angle + dist * t);
}

 \ingroup Geometry_Module
Eigen/src/Geometry/Quaternion.h

// single precision quaternion type
typedef Quaternion<float> Quaternionf;
// double precision quaternion type
typedef Quaternion<double> Quaterniond;
*/
