#include "precomp.h"

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

/** \geometry_module \ingroup Geometry_Module
 *
 *
 * \returns the Euler-angles of the rotation matrix \c *this using the convention defined by the triplet (\a a0,\a a1,\a
 * a2)
 *
 * Each of the three parameters \a a0,\a a1,\a a2 represents the respective rotation axis as an integer in {0,1,2}.
 * For instance, in:
 * \code Vector3f ea = mat.eulerAngles(2, 0, 2); \endcode
 * "2" represents the z axis and "0" the x axis, etc. The returned angles are such that
 * we have the following equality:
 * \code
 * mat == AngleAxisf(ea[0], Vector3f::UnitZ())
 *      * AngleAxisf(ea[1], Vector3f::UnitX())
 *      * AngleAxisf(ea[2], Vector3f::UnitZ()); \endcode
 * This corresponds to the right-multiply conventions (with right hand side frames).
 *
 * The returned angles are in the ranges [0:pi]x[-pi:pi]x[-pi:pi].
 *
 * \sa class AngleAxis
 */

TEST(geometry_euler_angle, example) {
    Eigen::Matrix3d mat = Eigen::Matrix3d::Identity();
    cout << mat << endl;
    /*
    1 0 0
    0 1 0
    0 0 1
    */

    // Eigen::Vector3f ea = mat.eulerAngles(2, 0, 2);
    // cout << ea << endl;
}