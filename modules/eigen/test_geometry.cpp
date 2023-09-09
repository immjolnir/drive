#include "precomp.h"

#include <gtest/gtest.h>

#include <cmath>
#include <iostream>

using namespace std;
using namespace Eigen;

/*
Eigen 几何模块的使用方法
*/
TEST(geometry, predefined_matrices) {
    /*
    Vector3f::UnitX() // 1 0 0
    Vector3f::UnitY() // 0 1 0
    Vector3f::UnitZ() // 0 0 1
    Vector4f::Unit(i) // What's this?
    */
    cout << Eigen::Vector3f::UnitX() << endl;
    cout << Eigen::Vector3f::UnitY() << endl;
    cout << Eigen::Vector3f::UnitZ() << endl;
    cout << Eigen::Vector3f::Unit(0) << endl;

    // typedef Eigen::Matrix<Scalar, 3, 1> Vector3;
    // Eigen::Vector3 v0 = Eigen::Vector3::Random();
    // std::cout << "v0=\n" << v0 << std::endl;
}

TEST(geometry, rotation) {
    // Eigen/Geometry 模块提供了各种旋转和平移的表示
    // 3D 旋转矩阵直接使用 Matrix3d 或 Matrix3f
    Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();

    // 旋转向量使用 AngleAxis, 它底层不直接是Matrix，但运算可以当作矩阵（因为重载了运算符）
    Eigen::AngleAxisd rotation_vector(M_PI / 4, Eigen::Vector3d(0, 0, 1));  // 沿 Z 轴旋转 45 度
    cout.precision(3);
    cout << "rotation matrix =\n" << rotation_vector.matrix() << endl;  // 用 matrix()转换成矩阵

    // 也可以直接赋值
    rotation_matrix = rotation_vector.toRotationMatrix();
    cout << "rotation_matrix=\n" << rotation_matrix << endl;
}

// TODO use my eigen.
// https://builtin.com/data-science/vector-norms
// https://machinelearningmastery.com/vector-norms-machine-learning/#:~:text=The%20length%20of%20the%20vector,vector's%20magnitude%20or%20the%20norm.
// https://askubuntu.com/questions/1408784/apps-crash-randomly-on-newly-installed-ubuntu-22-04

class GeometryTestCase : public testing::Test {
  public:
    // Now that we has precompiled header, we can use eigen's template instantiations.

    // typedef Matrix<double, 3, 3> Matrix3d;
    // typedef Matrix<double, 4, 4> Matrix4d;
    // typedef Matrix<double, 2, 1> Vector2d;
    // typedef Matrix<double, 3, 1> Vector3d;
    // typedef Matrix<double, 4, 1> Vector4d;
    // typedef Quaternion<double> Quaternionxd;
    // typedef AngleAxis<double> AngleAxisx;
    // typedef DiagonalMatrix<double, 3> AlignedScaling3;
    // typedef Translation<double, 2> Translation2;
    // typedef Translation<double, 3> Translation3;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(GeometryTestCase, geo_transformations) {
    Matrix3d actual_rot;
    // rotation matrix conversion
    actual_rot =
      AngleAxisd(0.1, Vector3d::UnitX()) * AngleAxisd(0.2, Vector3d::UnitY()) * AngleAxisd(0.3, Vector3d::UnitZ());

    cout << actual_rot << endl;

    Matrix3d expected_rot =
      AngleAxisd(0.1, Vector3d(1, 0, 0)).toRotationMatrix() *
      (AngleAxisd(0.2, Vector3d(0, 1, 0)).toRotationMatrix() * (AngleAxisd(0.3, Vector3d(0, 0, 1)).toRotationMatrix()));
    cout << expected_rot << endl;
}

/*
   The length of the vector is referred to as the vector norm or the vector's magnitude.

   The length of a vector is a nonnegative number that describes the extent of the vector in space, and is sometimes
   referred to as the vector's magnitude or the norm.
 */
TEST_F(GeometryTestCase, vector_norm) {
    Vector3d v0 = Vector3d::Random();
    while (v0.norm() < 1e-5f) {
        cout << "regenerate the vector for its norm is too small: v0=\n" << v0 << std::endl;
        v0 = Vector3d::Random();
    }
    cout << "norm=" << v0.norm() << endl;
    cout << "v0=\n" << v0 << endl;
}
