#ifndef _IMU_H
#define _IMU_H

#include "vector.hpp"

class IMU {
public:
    // Scaled readings
    virtual vector readMag() = 0;  // In body coords, scaled to -1..1 range
    virtual vector readAccel() = 0;  // In body coords, with units = g
    virtual vector readGyro() = 0; // In body coords, with units = rad/sec
    void read(){ readAccel(); readMag(); readGyro(); }

    virtual void measureOffsets() = 0;
    virtual void enable() = 0;
    virtual void loadCalibration() = 0;

    vector gyro_offset;
    int_vector mag_min, mag_max;

    int_vector raw_m, raw_a, raw_g;
};

#endif
