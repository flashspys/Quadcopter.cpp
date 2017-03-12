//
// Created by Felix Wehnert on 23.01.2017.
//

#ifndef QUADROCOPTER_QUADCOPTER_HPP
#define QUADROCOPTER_QUADCOPTER_HPP


#include <map>
#include <memory>
#include <iostream>

#include "PID.hpp"
#include "IMUCalculation.hpp"

class UDPServer;

enum Input {throttle, yaw, roll, pitch};
enum Trim {X, Y};

class Quadcopter {

private:
    std::map<Input, double> inputs = {{Input::throttle, 0}, {Input::pitch, 0}, {Input::roll, 0}, {Input::yaw, 0}};
    std::map<Trim, double> trim = {{Trim::X, 0}, {Trim::Y, 0}};
	std::map<Input, PID> pids = { {Input::roll, PID(1,0,0)}, {Input::pitch, PID(1,0,0)}, {Input::yaw, PID(1,0,0)} };

    std::shared_ptr<UDPServer> sharedServer;
    MinIMU9 imu;

    bool SHOULD_STABILIZE = false;

    Quadcopter();
    void imuReset();


public:
    static Quadcopter& sharedInstance() {
        static Quadcopter instance;
        return instance;
    }
    void onMessage(UDPServer* server, std::string message);

    void startStabilize();
    void startIO();
    void stabilize();


};


#endif //QUADROCOPTER_QUADCOPTER_HPP
