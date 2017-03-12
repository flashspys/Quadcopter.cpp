//
// Created by Felix Wehnert on 24.01.2017.
//

#ifndef QUADROCOPTER_LIS3MDL_HPP
#define QUADROCOPTER_LIS3MDL_HPP


#include "I2CDevice.hpp"

class LIS3MDL {

    static const unsigned int addr = 0x1E;
    static const unsigned int whoAmI = 0x0F;
    static const unsigned int ctrlReg1 = 0x20;
    static const unsigned int ctrlReg2 = 0x21;
    static const unsigned int ctrlReg3 = 0x22;
    static const unsigned int ctrlReg4 = 0x23;
    static const unsigned int ctrlReg5 = 0x24;


    I2CDevice device = I2CDevice(LIS3MDL::addr);

public:
    LIS3MDL();

    std::vector<int> readMagno();
};


#endif //QUADROCOPTER_LIS3MDL_HPP
