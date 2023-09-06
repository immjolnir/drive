#include <gtest/gtest.h>

#include "rotation3d.h"

// C++ math constants
// https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
// https://www.quantstart.com/articles/Mathematical-Constants-in-C/
#include <cmath>
// Note that it is not best practice within C++ to use #defines for mathematical constants! Instead, as an example, you
// should use const double pi = 3.14159265358979323846;. The #defines are a legacy feature of C.
#define _USE_MATH_DEFINES

/*
Mathematical Expression     C++ Symbol      Decimal Representation
pi                          M_PI            3.14159265358979323846
pi/2                        M_PI_2          1.57079632679489661923
pi/4                        M_PI_4          0.785398163397448309616
1/pi                        M_1_PI          0.318309886183790671538
2/pi                        M_2_PI          0.636619772367581343076
2/sqrt(pi)                  M_2_SQRTPI      1.12837916709551257390
sqrt(2)                     M_SQRT2         1.41421356237309504880
1/sqrt(2)                   M_SQRT1_2       0.707106781186547524401
e                           M_E             2.71828182845904523536
log_2(e)                    M_LOG2E         1.44269504088896340736
log_10(e)                   M_LOG10E        0.434294481903251827651
log_e(2)                    M_LN2           0.693147180559945309417
log_e(10)                   M_LN10          2.30258509299404568402
*/

// Part 1: EulerAngles(Roll, Pitch, Yaw) <-> Rotation Matrices
TEST(Rotations, yaw) {
    double yaw = M_PI_4;
    double pitch = 0;
    double roll = 0;
    auto rotation = yawPitchRollToMatrix(yaw, pitch, roll);
    rotation.print();
    /*
        0.707107     -0.707107             0
        0.707107     0.707107              0
        -0           0                     1
     */
    std::array<double, 3> euler_angles = MatrixToYawPitchRoll(rotation);
    EXPECT_DOUBLE_EQ(yaw, euler_angles[0]);
    EXPECT_DOUBLE_EQ(pitch, euler_angles[1]);
    EXPECT_DOUBLE_EQ(roll, euler_angles[2]);
}

TEST(Rotations, pitch) {
    double yaw = 0;
    double pitch = M_PI_4;
    double roll = 0;
    auto rotation = yawPitchRollToMatrix(yaw, pitch, roll);
    rotation.print();
    /*
        0.707107           0                     0.707107
        0                  1                     0
        -0.707107          0                     0.707107
     */
    std::array<double, 3> euler_angles = MatrixToYawPitchRoll(rotation);
    EXPECT_DOUBLE_EQ(yaw, euler_angles[0]);
    EXPECT_DOUBLE_EQ(pitch, euler_angles[1]);
    EXPECT_DOUBLE_EQ(roll, euler_angles[2]);
}

TEST(Rotations, roll) {
    double yaw = 0;
    double pitch = 0;
    double roll = M_PI_4;
    auto rotation = yawPitchRollToMatrix(yaw, pitch, roll);
    rotation.print();
    /*
        1       0                     0
        0       0.707107              -0.707107
        -0      0.707107              0.707107
    */
    std::array<double, 3> euler_angles = MatrixToYawPitchRoll(rotation);
    EXPECT_DOUBLE_EQ(yaw, euler_angles[0]);
    EXPECT_DOUBLE_EQ(pitch, euler_angles[1]);
    EXPECT_DOUBLE_EQ(roll, euler_angles[2]);
}

// Part 2: EulerAngles(Roll, Pitch, Yaw) <-> Quaternions
TEST(ThreeDimensions, Example) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);

    /*
    Eigen::Matrix4d radar_refine = Eigen::Matrix4d::Identity();
    Eigen::Quaterniond q(radar_calib_refine.calib_refine().qw(), radar_calib_refine.calib_refine().qx(),
                         radar_calib_refine.calib_refine().qy(), radar_calib_refine.calib_refine().qz());
    radar_refine.block<3, 3>(0, 0) = q.matrix();
    */
}

/*
        // calculate R_fix which fix the tr_radar_to_imu, rotate around camera's y axis
        Eigen::AngleAxisd pitch_angle(
                _radar_calib_refines_y_rot[_camera_radar_check_map[offsets.first]],
                Eigen::Vector3d::UnitY());
        Eigen::Quaternion<double> q = Eigen::AngleAxisd(0.0, Eigen::Vector3d::UnitZ()) *
                                      pitch_angle *
                                      Eigen::AngleAxisd(0.0, Eigen::Vector3d::UnitX());
        radar_refine.block<3, 3>(0, 0) = q.matrix();
*/

/*
        const Eigen::Matrix4d& refine_matrix = _radar_to_imu_refines[radar_topic];
        Eigen::Vector3d radar_position_imu =
                (refine_matrix * radar_detection_info.getPositionVector().homogeneous())
                        .hnormalized();
        Eigen::Vector3d radar_velocity_imu =
                refine_matrix.block<3, 3>(0, 0) * radar_detection_info.getVelocityVector();
        radar_detection_info.setPosition(radar_position_imu);
        radar_detection_info.setVelocity(radar_velocity_imu);
*/