//
// Created by Felix Wehnert on 23.01.2017.
//

#ifndef QUADROCOPTER_MOTOR_HPP
#define QUADROCOPTER_MOTOR_HPP


namespace Motor {

    const int pin1 = 26;
    const int pin2 = 19;
    const int pin3 = 21;
    const int pin4 = 20;

    static const int ESC_AIM_PULSEWIDTH = 900;
    static const int ESC_MIN_SPEED_PULSEWIDTH = 1060;
    static const int ESC_MAX_SPEED_PULSEWIDTH = 1860;

    void aim();
    void stop();
    void setSpeeds(double frontLeft, double frontRight, double backLeft, double backRight);
};


#endif //QUADROCOPTER_MOTOR_HPP
