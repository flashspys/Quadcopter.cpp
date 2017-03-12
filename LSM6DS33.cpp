//
// Created by Felix Wehnert on 24.01.2017.
//

#include <iostream>
#include <bitset>
#include "LSM6DS33.hpp"

using std::cout;
using std::endl;

/**
 *
 *
    std::bitset<16> a( x);
    std::bitset<16> b( y);
    std::bitset<16> c( z);

    std::cout << std::endl << a << "\t" << std::dec << x << std::endl << b << "\t" << std::dec << y << std::endl << c << "\t" << std::dec << z << std::endl;

 *
 */

LSM6DS33::LSM6DS33() {
    cout << "detected LSM6DS33 0x69 == 0x" << std::hex << device.readWhoAmI() << std::endl;

    device.writeByte(LSM6DS33::CTRL1, 0b01001100); // 104 Hz, 8G accelerometer, 400Hz Anti-aliasing filter
    device.writeByte(LSM6DS33::CTRL2, 0b01001100); // Gyro @ 104Hz / 2000dps
    device.writeByte(LSM6DS33::CTRL3, 0b00000100); // nothing special

}

std::vector<int> LSM6DS33::readAccel() {
    std::vector<byte> buffer = this->device.readBytes(0x28, 6);

    int16_t x = (buffer[1] << 8 | buffer[0]);
    int16_t y = (buffer[3] << 8 | buffer[2]);
    int16_t z = (buffer[5] << 8 | buffer[4]);

    // Eliminate static noise
    y += 20;

    return {x,y,z};

}

std::vector<int> LSM6DS33::readGyro() {

    std::vector<byte> buffer = this->device.readBytes(0x22, 6);

    int16_t x = (buffer[1] << 8 | buffer[0]);
    int16_t y = (buffer[3] << 8 | buffer[2]);
    int16_t z = (buffer[5] << 8 | buffer[4]);

    // Eliminate static noise
    x -= 25;
    y += 43;
    z += 11;

    return {x,y,z};

}

int LSM6DS33::readTemp() {
    auto buffer = this->device.readBytes(0x20, 2);
    return buffer[1] << 8 | buffer[0];
}