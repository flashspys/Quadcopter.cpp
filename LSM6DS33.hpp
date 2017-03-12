//
// Created by Felix Wehnert on 24.01.2017.
//

#ifndef QUADROCOPTER_LSM6DS33_HPP
#define QUADROCOPTER_LSM6DS33_HPP

#include "I2CDevice.hpp"

class LSM6DS33 {

    static const unsigned int addr = 0x6B;

    static const unsigned int CTRL1 = 0x10;
    static const unsigned int CTRL2 = 0x11;
    static const unsigned int CTRL3 = 0x12;
    static const unsigned int CTRL4 = 0x13;
    static const unsigned int CTRL5 = 0x14;
    static const unsigned int CTRL6 = 0x15;
    static const unsigned int CTRL7 = 0x16;
    static const unsigned int CTRL8 = 0x17;
    static const unsigned int CTRL9 = 0x18;
    static const unsigned int CTRL10 = 0x19;

    I2CDevice device = I2CDevice(LSM6DS33::addr);
public:
    LSM6DS33();

    std::vector<int> readAccel();

    std::vector<int> readGyro();

    int readTemp();
};


#endif //QUADROCOPTER_LSM6DS33_HPP
