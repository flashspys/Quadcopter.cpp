//
// Created by Felix Wehnert on 24.01.2017.
//

#ifndef QUADROCOPTER_I2CWRAPPER_HPP
#define QUADROCOPTER_I2CWRAPPER_HPP

#include <vector>

typedef unsigned char byte;

class I2CDevice {

public:

    int i2chandler = 0;

    I2CDevice(unsigned int addr);
    ~I2CDevice();

    void writeBytes(unsigned int reg, std::vector<byte> bytes);
    void writeByte(unsigned int reg, byte b);

    std::vector<byte> readBytes(unsigned int reg, unsigned int length);
    byte readByte(unsigned int reg);

    int readWhoAmI();
};


#endif //QUADROCOPTER_I2CWRAPPER_HPP
