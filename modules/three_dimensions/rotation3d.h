#pragma once

/*
Rotations in Three-Dimensions: Euler Angles and Rotation Matrices
- https://danceswithcode.net/engineeringnotes/rotations_in_3d/rotations_in_3d_part1.html

This paper describes a commonly used set of Tait-Bryan Euler angles, shows how to convert from Euler angles to a
rotation matrix and back, how to rotate objects in both the forward and reverse direction, and how to concatenate
multiple rotations into a single rotation matrix.

Anyone dealing with three dimensional rotations will need to be familiar with both Euler angles and rotation matrices.
Euler angles are useful for describing 3D rotations in a way that is understandable to humans, and are therefore
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
    bool operator==(const RotationMatrix& other);

    void print();
};

/*
    Converting Euler Angles to Rotation Matrices

    (u, v, w) are the three Euler angles (roll, pitch, yaw)(in radians), corresponding to rotations around the x, y
   and z axes
*/
// Creating a Rotation Matrix from Euler Angles
RotationMatrix yawPitchRollToMatrix(double yaw,    // Yaw   angle (radians)
                                    double pitch,  // Pitch angle (radians)
                                    double roll)   // Roll  angle (radians)
  ;

/*
    Converting a Rotation Matrix to Euler Angles
    Extracting Euler Angles from a Rotation Matrix

*/

std::array<double, 3> MatrixToYawPitchRoll(const RotationMatrix& A);

/*
    Rotating a Point using a Rotation Matrix
*/
std::array<double, 3> rotatePoint(const RotationMatrix& A,
                                  const std::array<double, 3>& p_in)  // Input coords in order (x,y,z)
  ;

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

RotationMatrix transposeMatrix(const RotationMatrix& A);

/*
    Combining Multiple Rotations

    Multiple rotations can be combined into a single rotation by multiplying the respective rotation matrices together.
    If A is an initial rotation, and B is a subsequent rotation, then the cumulative rotation C.

     C = A*B
*/

// Concatenating Rotations using Matrix Multiplication
RotationMatrix multiplyMatrices(const RotationMatrix& A,  // First rotation
                                const RotationMatrix& B)  // Second rotation
  ;