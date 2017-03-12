//
// Created by Felix Wehnert on 24.01.2017.
//

#include "I2CDevice.hpp"
#include <iostream>

using std::cout;
using std::endl;

#define QUADRO_USE_LINUX_I2C_COMMUNICATION 1

#ifdef __APPLE__
// -- -- - -- - - -- -- - STUB - -- - - -- - - --- - - - --
#include "pigpio.h"
I2CDevice::I2CDevice(unsigned int addr) { this->i2chandler = 0; }
I2CDevice::~I2CDevice() { }
void I2CDevice::writeBytes(unsigned int reg, std::vector<byte> bytes) { }
void I2CDevice::writeByte(unsigned int reg, byte b) { }
std::vector<byte> I2CDevice::readBytes(unsigned int reg, unsigned int length) {
    std::vector<byte> buffer = {0};
    return buffer;
}
byte I2CDevice::readByte(unsigned int reg) { return 0; }
// ---   - -- - - - - - - - - END STUB -- -- - - - - - - - - -  --
#elif QUADRO_USE_LINUX_I2C_COMMUNICATION

#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


I2CDevice::I2CDevice(unsigned int addr)
{
    i2chandler = open("/dev/i2c-1", O_RDWR);
    if (i2chandler == -1)
    {
        cout << "Failed to open I2C device " << endl;
    }

    int result = ioctl(i2chandler, I2C_SLAVE, addr);
    if (result == -1)
    {
        cout << "Failed to set address" << endl;
    }

}

I2CDevice::~I2CDevice()
{
    close(i2chandler);
}

void I2CDevice::writeBytes(unsigned int reg, std::vector<byte> bytes) {
     cout << "writeBytes does nothing atm." << endl;
}

void I2CDevice::writeByte(unsigned int command, byte data)
{
    int result = i2c_smbus_write_byte_data(i2chandler, command, data);
    if (result == -1)
    {
         cout << "Failed to write byte to I2C" << endl;
    }
}

byte I2CDevice::readByte(unsigned int command)
{
    int result = i2c_smbus_read_byte_data(i2chandler, command);
    if (result == -1)
    {
         cout << "Failed to read byte from I2C" << endl;
    }
    return (byte) result;
}


std::vector<byte> I2CDevice::readBytes(unsigned int command, unsigned int length)
{

    std::vector<byte> buffer(length);
    int result = i2c_smbus_read_i2c_block_data(i2chandler, command, length, (unsigned char *)&buffer[0]);
    if (result != length)
    {
         cout << "Failed to read block from I2C" << endl;
    }
    return buffer;
}


#else
#include <pigpio.h>

I2CDevice::I2CDevice(unsigned int addr) {
    this->i2chandler = (unsigned int) i2cOpen(1, addr, 0);
}

I2CDevice::~I2CDevice() {
    i2cClose(this->i2chandler);
}

void I2CDevice::writeBytes(unsigned int reg, std::vector<byte> bytes) {
    i2cWriteI2CBlockData(this->i2chandler, reg, bytes.data(), (unsigned int) bytes.size());
}

void I2CDevice::writeByte(unsigned int reg, byte b) {

    std::vector<byte> data = {b};
    writeBytes(reg, data);

}

std::vector<byte> I2CDevice::readBytes(unsigned int reg, unsigned int length) {

    std::vector<byte> buffer(length);
    i2cReadI2CBlockData(this->i2chandler, reg, buffer.data(), length);
    return buffer;

}

byte I2CDevice::readByte(unsigned int reg) {
    return this->readBytes(reg, 1)[0];
}


#endif

int I2CDevice::readWhoAmI() {
    return (int) this->readByte(0x0f);
}
