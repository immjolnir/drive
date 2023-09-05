#include <gtest/gtest.h>

/*
Rotations in Three-Dimensions: Euler Angles and Rotation Matrices
- https://danceswithcode.net/engineeringnotes/rotations_in_3d/rotations_in_3d_part1.html

This paper describes a commonly used set of Tait-Bryan Euler angles, shows how to convert from Euler angles to a
rotation matrix and back, how to rotate objects in both the forward and reverse direction, and how to concatenate
multiple rotations into a single rotation matrix.

Anyone dealing with three dimensional rotations will need to be familiar with both Euler angles and rotation matrices.
Euler angels are useful for describing 3D rotations in a way that is understandable to humans, and are therefore
commonly seen in user interfaces. Rotation matrices, on the other hand, are the representation of choice when it comes
to implementing efficient rotations in software.

Unfortunately, converting back and forth between Euler angles and rotation matrices is a perennial source of confusion.
The reason is not that the math is particularly complicated. The reason is there are dozens of mutually exclusive ways
to define Euler angles. Different authors are likely to use different conventions, often without clearly stating the
underlying assumptions. This makes it difficult to combine equations and code from more than one source.

In this paper we will present one single (and very common) definition of Euler angles, and show how to use them.

To summarize, we will employ a Tait-Bryan Euler angle convention using active, intrinsic rotations around the axes in
the order z-y-x. We will call the rotation angles yaw, pitch and roll respectively. This is a common convention, and
most people find it the easiest to visualize. For example, the first two rotations (yaw and pitch) are identical to the
azimuth and elevation angles used in directing artillery pieces and in surveying; to the pan and tilt angles used to
specify the aiming direction of a camera; and to the longitude and latitude coordinates used in navigation.

- Converting a Rotation Matrix to Euler Angles

- Converting Euler Angles to a Rotation Matrix

- Interpolating Between Rotations, and Averaging Rotations
  - For some applications, it may be necessary to either interpolate between orientations, or to compute the average of
a number of rotations.

  - It is possible (thought tedious) to interpolate between orientations using rotation matrices. The algorithm to do so
is called “Spherical Linear Interpolation” (SLERP), a description of which may be found here
(http://en.wikipedia.org/wiki/Slerp). It is also possible to compute the average of a set of rotation matrices, although
once again the algorithm is tedious and somewhat ad hoc.

  - It is also possible to compute the average of a set of rotation matrices, although once again the algorithm is
tedious and somewhat ad hoc. The basic technique is:
    - Sum the X column vectors, and normalize to unit length. (The X column vector is the three elements of the first
column, treated as a vector). This produces the X column in the final average matrix.
    - Sum the Y column vectors and normalize to unit length. This and the X vector define the X,Y plane.
    - Compute the final average Y column vector by finding the unit vector that is perpendicular to the final X vector
and in the X,Y plane.
    - Compute the final average Z vector as the cross product of the final X and Y vectors.
    - Make sure all three final column vectors in the average matrix are unit vectors and mutually orthogonal.

Although the above algorithms work, they are not recommended. If you need to do either interpolation or averaging of
rotations, quaternions are the standard solution. Though somewhat intimidating at first, quaternions are a third
technique for representing rotations (after Euler angles and rotation matrices), and provide an elegant and
computationally efficient way to rotate points and concatenate, average, and interpolate rotations.

- Quaternions

Rotation Matrix Properties
  - The following are properties of all rotation matrices:
  - The magnitude (sum of squares) of the elements in any row or column is 1. That is, each row and each column is a
unit vector.
  - The unit vectors in rows 1, 2 and 3 define the x, y and z axes of the rotated coordinate system in the original
world space.
  - The unit vectors in columns 1, 2, and 3 define the x, y, and z axes of the world coordinate system in the rotated
coordinate space.

This implies that the following tests are necessary and sufficient to prove a 3x3 matrix is a valid rotation matrix:

  - The magnitude of all rows and all columns must be equal to 1.0
  - The cross product of row 1 and row 2 must equal row 3
  - The cross product of column 1 and column 2 must equal column 3

Part 2

- Tait-Bryan variant of Euler Angles
- Yaw-pitch-roll rotation order, rotating around the z, y and x axes respectively
- Intrinsic rotation
- Active (otherwise known as alibi) rotation
- Right-handed coordinate system with right-handed rotations

*/

#include <array>  // std::array
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

using namespace std;

/*
Rotation Matrices(3x3 rotation matrix)

    | r11 r12 r13 |
R = | r21 r22 r23 |
    | r31 r32 r33 |
*/
struct RotationMatrix {
    double r11 = 0;  // First row
    double r12 = 0;
    double r13 = 0;
    double r21 = 0;  // Second row
    double r22 = 0;
    double r23 = 0;
    double r31 = 0;  // Third row
    double r32 = 0;
    double r33 = 0;

    // comma-initializer
    // using the comma-initializer is also allowed
    //   m << 1.0,2.0, 3.0,4.0;
    /* inserts a scalar value in the target matrix */
    // RotationMatrix& operator,(const double& s) {
    //      return *this;
    // }

    // comparasion
    // overload == operator
    bool operator==(const RotationMatrix& other) {
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

    void print() {
        cout << std::left << setw(8) << r11 << std::left << setw(22) << r12 << std::left << setw(22) << r13 << endl;
        cout << std::left << setw(8) << r21 << std::left << setw(22) << r22 << std::left << setw(22) << r23 << endl;
        cout << std::left << setw(8) << r31 << std::left << setw(22) << r32 << std::left << setw(22) << r33 << endl;
        // clang-format on
    }
};

/*
    Converting Euler Angles to Rotation Matrices

    (u, v, w) are the three Euler angles (roll, pitch, yaw)(in radians), corresponding to rotations around the x, y
   and z axes
*/
// Creating a Rotation Matrix from Euler Angles
static RotationMatrix yawPitchRollToMatrix(double yaw,    // Yaw   angle (radians)
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

/*
    Converting a Rotation Matrix to Euler Angles
    Extracting Euler Angles from a Rotation Matrix

*/

static std::array<double, 3> MatrixToYawPitchRoll(const RotationMatrix& A) {
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
static std::array<double, 3> rotatePoint(const RotationMatrix& A,
                                         const std::array<double, 3>& p_in)  // Input coords in order (x,y,z)
{
    std::array<double, 3> p_out;
    p_out[0] = A.r11 * p_in[0] + A.r12 * p_in[1] + A.r13 * p_in[2];
    p_out[1] = A.r21 * p_in[0] + A.r22 * p_in[1] + A.r23 * p_in[2];
    p_out[2] = A.r31 * p_in[0] + A.r32 * p_in[1] + A.r33 * p_in[2];
    return p_out;  // Output coords in order (x,y,z)
}

/*
Inverse Rotations
For rotation matrices, the transpose of a matrix equals its inverse. Therefore if R_f is a forward rotation matrix, the
reverse rotation R)r can be obtained by transposing the rows and columns of R_f.

- Forward rotation matrix:

          | r11 r12 r13 |
    R_f = | r21 r22 r23 |
          | r31 r32 r33 |

- Reverse rotation matrix:
          | r11 r21 r31 |
    R_f = | r12 r22 r32 |
          | r13 r23 r33 |
*/

// Inverting a 3x3 Rotation Matrix

static RotationMatrix transposeMatrix(const RotationMatrix& A) {
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

/*
    Combining Multiple Rotations

    Multiple rotations can be combined into a single rotation by multiplying the respective rotation matrices together.
    If A is an initial rotation, and B is a subsequent rotation, then the cumulative rotation C.

     C = A*B
*/

// Concatenating Rotations using Matrix Multiplication
static RotationMatrix multiplyMatrices(const RotationMatrix& A,  // First rotation
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

// Part 1: EulerAngles(Roll, Pitch, Yaw) <-> Rotation Matrices
TEST(Rotations, EulerAngles_and_Rotation_Matrices) {
    double roll = M_PI_4;
    double pitch = 0;
    double yaw = 0;
    auto rotation = yawPitchRollToMatrix(yaw, pitch, roll);
    rotation.print();
    RotationMatrix expect;
    expect.r11 = 1;
    expect.r12 = 0;
    expect.r13 = 0;
    expect.r21 = 0;
    expect.r22 = 0.707107;
    expect.r23 = -0.707107;
    expect.r31 = 0;
    expect.r32 = 0.707107;
    expect.r33 = 0.707107;
    expect.print();

    // EXPECT_EQ(expect, rotation);
}

// Part 2: EulerAngles(Roll, Pitch, Yaw) <-> Quaternions
TEST(ThreeDimensions, Example) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}
