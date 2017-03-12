#include "vector.hpp"
#include "MinIMU9.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <wordexp.h>

MinIMU9::MinIMU9()
  //compass(i2cDeviceName), gyro(i2cDeviceName)
{

}

void MinIMU9::enable()
{
    //compass.enable();
    //gyro.enable();
}

void MinIMU9::loadCalibration()
{
    wordexp_t expansion_result;
    wordexp("~/.minimu9-ahrs-cal", &expansion_result, 0);

    std::ifstream file(expansion_result.we_wordv[0]);
    if (file.fail())
    {
	    throw std::runtime_error("Failed to open calibration file ~/.minimu9-ahrs-cal");
    }

    file >> mag_min(0) >> mag_max(0) >> mag_min(1) >> mag_max(1) >> mag_min(2) >> mag_max(2);
    if (file.fail() || file.bad())
    {
        throw std::runtime_error("Failed to parse calibration file ~/.minimu9-ahrs-cal");
    }
}

void MinIMU9::measureOffsets()
{
    // LSM303 accelerometer: 8 g sensitivity.  3.8 mg/digit; 1 g = 256.
    // TODO: unify this with the other place in the code where we scale accelerometer readings.
    gyro_offset = vector::Zero();
    const int sampleCount = 32;
	std::vector<int> gyroData;
    for(int i = 0; i < sampleCount; i++)
    {
        gyroData = gyro.readGyro();
        gyro_offset += vector_from_ints(gyroData.data());
        usleep(20*1000);
    }
    gyro_offset /= sampleCount;
}

vector MinIMU9::readMag()
{
    std::vector<int> magnoData = compass.readMagno();
    IMU::raw_m = int_vector_from_ints(magnoData.data());
    
    vector v;
    v(0) = (float)(magnoData[0] - mag_min(0)) / (mag_max(0) - mag_min(0)) * 2 - 1;
    v(1) = (float)(magnoData[1] - mag_min(1)) / (mag_max(1) - mag_min(1)) * 2 - 1;
    v(2) = (float)(magnoData[2] - mag_min(2)) / (mag_max(2) - mag_min(2)) * 2 - 1;
    return v;
}

vector MinIMU9::readAccel()
{
    // Info about linear acceleration sensitivity from datasheets:
    // LSM303DLM: at FS = 8 g, 3.9 mg/digit (12-bit reading)
    // LSM303DLHC: at FS = 8 g, 4 mg/digit (12-bit reading probably an approximation)
    // LSM303DLH: at FS = 8 g, 3.9 mg/digit (12-bit reading)
    // LSM303D: at FS = 8 g, 0.244 mg/LSB (16-bit reading)
    const float accel_scale = 0.000244;

    std::vector<int> accelData = gyro.readAccel();
    IMU::raw_a = int_vector_from_ints(accelData.data());
    return vector_from_ints(accelData.data()) * accel_scale;
}

vector MinIMU9::readGyro()
{
    // Info about sensitivity from datasheets:
    // L3G4200D: at FS = 2000 dps, 70 mdps/digit
    // L3GD20: at FS = 2000 dps, 70 mdps/digit
    // L3GD20H: at FS = 2000 dps, 70 mdps/digit
    const double gyro_scale = 0.07 * 3.14159265 / 180;

	std::vector<int> gyroData = gyro.readGyro();
    IMU::raw_g = int_vector_from_ints(gyroData.data());
    return ( vector_from_ints(gyroData.data()) - gyro_offset ) * gyro_scale;
}
