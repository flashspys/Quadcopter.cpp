//
// Created by Felix Wehnert on 23.01.2017.
//

#include <iostream>
#include "Motor.hpp"
#include <thread>
#include "pigpio.h"

double checkBounces(double speed) {
    return speed < 0.0 ? 0 : (speed > 1 ? 1 : speed);
}

using namespace Motor;

void Motor::aim() {
    //Execute pigs aim command on command line
    gpioInitialise();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    system("pigs s 21 900 s 19 900 s 26 900 s 20 900");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Motor::stop() {
    Motor::setSpeeds(Motor::ESC_AIM_PULSEWIDTH, Motor::ESC_AIM_PULSEWIDTH, Motor::ESC_AIM_PULSEWIDTH, Motor::ESC_AIM_PULSEWIDTH);
}

void Motor::setSpeeds(double frontLeft, double frontRight, double backLeft, double backRight) {

    gpioServo(Motor::pin1, (unsigned int) (checkBounces(frontLeft)  * (ESC_MAX_SPEED_PULSEWIDTH - ESC_MIN_SPEED_PULSEWIDTH) + ESC_MIN_SPEED_PULSEWIDTH));
    gpioServo(Motor::pin2, (unsigned int) (checkBounces(frontRight) * (ESC_MAX_SPEED_PULSEWIDTH - ESC_MIN_SPEED_PULSEWIDTH) + ESC_MIN_SPEED_PULSEWIDTH));
    gpioServo(Motor::pin3, (unsigned int) (checkBounces(backLeft)   * (ESC_MAX_SPEED_PULSEWIDTH - ESC_MIN_SPEED_PULSEWIDTH) + ESC_MIN_SPEED_PULSEWIDTH));
    gpioServo(Motor::pin4, (unsigned int) (checkBounces(backRight)  * (ESC_MAX_SPEED_PULSEWIDTH - ESC_MIN_SPEED_PULSEWIDTH) + ESC_MIN_SPEED_PULSEWIDTH));

}
