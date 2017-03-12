//
// Created by Felix Wehnert on 10.02.2017.
//

#ifndef QUADROCOPTER_IMUCALCULATION_HPP
#define QUADROCOPTER_IMUCALCULATION_HPP

#include <time.h>
#include <ctime>
#include <sys/time.h>
#include <cmath>
#include "MinIMU9.hpp"

#define FLOAT_FORMAT std::fixed

std::ostream & operator << (std::ostream & os, const vector & vector);
std::ostream & operator << (std::ostream & os, const matrix & matrix);
std::ostream & operator << (std::ostream & os, const quaternion & quat);
int millis();

std::vector<double> myEulerAngles(const matrix &rm);
matrix rotationFromCompass(const vector &acceleration, const vector &magnetic_field);

void rotate(quaternion &rotation, const vector &w, double dt);
void fuse(quaternion &rotation, float dt, const vector &angular_velocity, const vector &acceleration, const vector &magnetic_field);

#endif //QUADROCOPTER_IMUCALCULATION_HPP
