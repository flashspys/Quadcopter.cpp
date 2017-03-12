//
// Created by Felix Wehnert on 24.01.2017.
//

#include <iostream>
#include <bitset>
#include "LIS3MDL.hpp"

using std::cout;
using std::endl;

LIS3MDL::LIS3MDL() {

    cout << "LIS3MDL detected: 0x3D == 0x" << std::hex << this->device.readWhoAmI() << endl;

    this->device.writeByte(LIS3MDL::ctrlReg1, 0x78); // Ultra High Performance Mode 40 Hz
    this->device.writeByte(LIS3MDL::ctrlReg2, 0x00); // 4gauss
    this->device.writeByte(LIS3MDL::ctrlReg3, 0x00); // Single-conversion-Mode
    this->device.writeByte(LIS3MDL::ctrlReg4, 0x0C); // Z-axis operative mode selection: Ultra High Performance Mode*/
    this->device.writeByte(LIS3MDL::ctrlReg5, 0x00); //

}

std::vector<int> LIS3MDL::readMagno() {

    std::vector<byte> buffer = this->device.readBytes(0x80 | 0x28, 6);

    int16_t x = (buffer[1] << 8 | buffer[0]);
    int16_t y = (buffer[3] << 8 | buffer[2]);
    int16_t z = (buffer[5] << 8 | buffer[4]);

    return {x,y,z};

}
