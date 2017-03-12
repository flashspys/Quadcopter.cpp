//
// Created by Felix Wehnert on 21.02.2017.
//

#include "IMUCalculation.hpp"


std::ostream & operator << (std::ostream & os, const vector & vector)
{
    return os << FLOAT_FORMAT << vector(0) << ' '
              << FLOAT_FORMAT << vector(1) << ' '
              << FLOAT_FORMAT << vector(2);
}

std::ostream & operator << (std::ostream & os, const matrix & matrix)
{
    return os << (vector)matrix.row(0) << ' '
              << (vector)matrix.row(1) << ' '
              << (vector)matrix.row(2);
}

std::ostream & operator << (std::ostream & os, const quaternion & quat)
{
    return os << FLOAT_FORMAT << quat.w() << ' '
              << FLOAT_FORMAT << quat.x() << ' '
              << FLOAT_FORMAT << quat.y() << ' '
              << FLOAT_FORMAT << quat.z();
}


int millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}



std::vector<double> myEulerAngles(const matrix &rm) {
/*  https://www.learnopencv.com/rotation-matrix-to-euler-angles/
 *     float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

    bool singular = sy < 1e-6; // If

    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vec3f(x, y, z);
 *
 */

    double sy = sqrt(rm(0,0) * rm(0,0) + rm(1,0) * rm(1,0));
    bool singular = sy < 1e-6;

    double x,y,z;
    if(!singular) {
        x = atan2( rm(2,1), rm(2,2));
        y = atan2(-rm(2,0), sy);
        z = atan2( rm(1,0), rm(0,0));
    } else {
        x = atan2(-rm(1,2), rm(1,1));
        y = atan2(-rm(2,0), sy);
        z = 0;
    }

    return {x,y,z};
}

matrix rotationFromCompass(const vector &acceleration, const vector &magnetic_field) {
    vector down = -acceleration;     // usually true
    vector east = down.cross(magnetic_field); // actually it's magnetic east
    vector north = east.cross(down);

    east.normalize();
    north.normalize();
    down.normalize();

    matrix r;
    r.row(0) = north;
    r.row(1) = east;
    r.row(2) = down;
    return r;
}

void rotate(quaternion &rotation, const vector &w, double dt) {
    // Multiply by first order approximation of the
    // quaternion representing this rotation.
    rotation *= quaternion(1, w(0) * dt / 2, w(1) * dt / 2, w(2) * dt / 2);
    rotation.normalize();
}
/*
void fuse(quaternion& rotation, double dt, const vector& angular_velocity,
                    const vector& acceleration, const vector& magnetic_field)
{
    rotate(rotation, angular_velocity, dt);
}
*/

void fuse(quaternion &rotation, float dt, const vector &angular_velocity,
          const vector &acceleration, const vector &magnetic_field) {
    vector correction = vector(0, 0, 0);

    if (abs(acceleration.norm() - 1) <= 0.3) {
        // The magnetidude of acceleration is close to 1 g, so
        // it might be pointing up and we can do drift correction.

        const float correction_strength = 1;

        matrix rotationCompass = rotationFromCompass(acceleration, magnetic_field);
        matrix rotationMatrix = rotation.toRotationMatrix();

        correction = (
                             rotationCompass.row(0).cross(rotationMatrix.row(0)) +
                             rotationCompass.row(1).cross(rotationMatrix.row(1)) +
                             rotationCompass.row(2).cross(rotationMatrix.row(2))
                     ) * correction_strength;

    }

    rotate(rotation, angular_velocity + correction, dt);
}


