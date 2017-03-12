#ifndef _MINIMU9_H
#define _MINIMU9_H

#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DS33.hpp"

class MinIMU9 : public IMU {
public:
    MinIMU9();

    LIS3MDL compass;
    LSM6DS33 gyro;

    virtual vector readAccel();
    virtual vector readMag();
    virtual vector readGyro();

    virtual void enable();
    virtual void loadCalibration();
    virtual void measureOffsets();
};

#endif
