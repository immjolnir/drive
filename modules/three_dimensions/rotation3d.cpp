#include "rotation3d.h"

#include <iostream>
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
#include <iomanip>

bool RotationMatrix::operator==(const RotationMatrix& other) {
    bool res = (r11 == other.r11);
    res = res && (r12 == other.r12);
    res = res && (r13 == other.r13);
    res = res && (r21 == other.r21);
    res = res && (r22 == other.r22);
    res = res && (r23 == other.r23);
    res = res && (r31 == other.r31);
    res = res && (r32 == other.r32);
    res = res && (r33 == other.r33);

    return res;
}

void RotationMatrix::print() {
    using namespace std;
    cout << std::left << setw(8) << r11 << std::left << setw(22) << r12 << std::left << setw(22) << r13 << endl;
    cout << std::left << setw(8) << r21 << std::left << setw(22) << r22 << std::left << setw(22) << r23 << endl;
    cout << std::left << setw(8) << r31 << std::left << setw(22) << r32 << std::left << setw(22) << r33 << endl;
}

RotationMatrix yawPitchRollToMatrix(double yaw,    // Yaw   angle (radians)
                                    double pitch,  // Pitch angle (radians)
                                    double roll)   // Roll  angle (radians)
{
    // Precompute sines and cosines of Euler angles
    double su = std::sin(roll);
    double cu = std::cos(roll);
    double sv = std::sin(pitch);
    double cv = std::cos(pitch);
    double sw = std::sin(yaw);
    double cw = std::cos(yaw);

    // Create and populate RotationMatrix
    RotationMatrix A;
    A.r11 = cv * cw;
    A.r12 = su * sv * cw - cu * sw;
    A.r13 = su * sw + cu * sv * cw;
    A.r21 = cv * sw;
    A.r22 = cu * cw + su * sv * sw;
    A.r23 = cu * sv * sw - su * cw;
    A.r31 = -sv;
    A.r32 = su * cv;
    A.r33 = cu * cv;
    return A;
}

std::array<double, 3> MatrixToYawPitchRoll(const RotationMatrix& A) {
    std::array<double, 3> angle;

    angle[1] = -std::asin(A.r31);  // Pitch
    /*
    However, for the special case where the pitch angle (v) = +/-90°, the system enters a state called “gimbal lock.”
    Equation 3b is still valid, but equations 3a and 3c are undefined. This condition must be tested for and handled as
    follows: If pitch angle v = −90°, then r31 will equal 1, and;

        u + w = atan2(− r12,− r13)(eq 4a)

    If pitch angle v = +90°, then r31 will equal −1, and;
        u − w = atan2(r12,r13)

    In practice, it is usual to set one of the angles (u) or (w) to zero and solve for the other.

    This code will return values of roll and yaw between −π and +π radians, and pitch angles between −π/2 and +π/2
    radians.
    */

    // Gymbal lock: pitch = -90
    if (A.r31 == 1) {
        angle[0] = 0.0;                         // yaw = 0
        angle[2] = std::atan2(-A.r12, -A.r13);  // Roll
        std::cout << "Gimbal lock: pitch = -90" << std::endl;
    }

    // Gymbal lock: pitch = 90
    else if (A.r31 == -1) {
        angle[0] = 0.0;                       // yaw = 0
        angle[2] = std::atan2(A.r12, A.r13);  // Roll
        std::cout << "Gimbal lock: pitch = 90" << std::endl;
    }
    // General solution
    else {
        angle[0] = std::atan2(A.r21, A.r11);
        angle[2] = std::atan2(A.r32, A.r33);
        std::cout << "No gimbal lock" << std::endl;
    }
    return angle;  // Euler angles in order yaw, pitch, roll
}

/*
    Rotating a Point using a Rotation Matrix
*/
std::array<double, 3> rotatePoint(const RotationMatrix& A,
                                  const std::array<double, 3>& p_in)  // Input coords in order (x,y,z)
{
    std::array<double, 3> p_out;
    p_out[0] = A.r11 * p_in[0] + A.r12 * p_in[1] + A.r13 * p_in[2];
    p_out[1] = A.r21 * p_in[0] + A.r22 * p_in[1] + A.r23 * p_in[2];
    p_out[2] = A.r31 * p_in[0] + A.r32 * p_in[1] + A.r33 * p_in[2];
    return p_out;  // Output coords in order (x,y,z)
}

RotationMatrix transposeMatrix(const RotationMatrix& A) {
    RotationMatrix B;
    B.r11 = A.r11;
    B.r12 = A.r21;
    B.r13 = A.r31;
    B.r21 = A.r12;
    B.r22 = A.r22;
    B.r23 = A.r32;
    B.r31 = A.r13;
    B.r32 = A.r23;
    B.r33 = A.r33;
    return B;
}

RotationMatrix multiplyMatrices(const RotationMatrix& A,  // First rotation
                                const RotationMatrix& B)  // Second rotation
{
    RotationMatrix C;
    C.r11 = A.r11 * B.r11 + A.r12 * B.r21 + A.r13 * B.r31;
    C.r12 = A.r11 * B.r12 + A.r12 * B.r22 + A.r13 * B.r32;
    C.r13 = A.r11 * B.r13 + A.r12 * B.r23 + A.r13 * B.r33;
    C.r21 = A.r21 * B.r11 + A.r22 * B.r21 + A.r23 * B.r31;
    C.r22 = A.r21 * B.r12 + A.r22 * B.r22 + A.r23 * B.r32;
    C.r23 = A.r21 * B.r13 + A.r22 * B.r23 + A.r23 * B.r33;
    C.r31 = A.r31 * B.r11 + A.r32 * B.r21 + A.r33 * B.r31;
    C.r32 = A.r31 * B.r12 + A.r32 * B.r22 + A.r33 * B.r32;
    C.r33 = A.r31 * B.r13 + A.r32 * B.r23 + A.r33 * B.r33;
    return C;  // Returns combined rotation (C=AB)
}