//
// Created by Felix Wehnert on 23.01.2017.
//

#include "Quadcopter.hpp"

#include <thread>
#include <mutex>

#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <boost/asio/io_service.hpp>

#include "UDPServer.hpp"
#include "Motor.hpp"
#include "Log.hpp"

Quadcopter::Quadcopter() {

    std::cout << "Quad initialised" << std::endl;

}


void Quadcopter::startIO() {

    this->imu.loadCalibration();
    this->imu.enable();
    this->imu.measureOffsets();


    std::cout << "Start IO server" << std::endl;
    boost::asio::io_service io_service;

    this->sharedServer.reset(new UDPServer(io_service, 9001));

    if(this->sharedServer == nullptr) {
        std::cout << "Shared server is null" << std::endl;
    }

    //Motor::aim();

    std::thread stabilize_thread([&] { this->stabilize(); });
    std::thread io_thread([&] { io_service.run(); });

    stabilize_thread.detach();
    io_thread.join();

    std::cout << "IO Crashed?" << std::endl;
}


void Quadcopter::startStabilize() {

    this->pids[Input::roll].reset();
    this->pids[Input::pitch].reset();

    this->SHOULD_STABILIZE = true;
}


void Quadcopter::onMessage(UDPServer *server, std::string message) {

    this->inputs[Input::roll] = 1;
    std::vector <std::string> parts;
    boost::split(parts, message, boost::is_any_of(";"), boost::token_compress_on);

    std::string command = parts[0];

    if (command == "T") {

        std::cout << std::stod(parts[1]) << " " << std::stod(parts[2]);
        this->inputs[Input::yaw] = std::stod(parts[1]);
        this->inputs[Input::throttle] = std::stod(parts[2]);

    } else if (command == "S") {

        std::cout << std::stod(parts[1]) << " " << std::stod(parts[2]);
        this->inputs[Input::roll] = std::stod(parts[1]);
        this->inputs[Input::pitch] = std::stod(parts[2]);

    } else if (command == "STOP") {
        Motor::stop();
    } else if (command == "TR") {

        std::cout << std::stod(parts[1]) << " " << std::stod(parts[2]);
        this->trim[Trim::X] = std::stod(parts[1]);
        this->trim[Trim::Y] = std::stod(parts[2]);

    } else if (command == "CONNECT") {
        server->ackConnect();
        this->startStabilize();
    } else if (command == "IMURESET") {
        this->imuReset();
    } else if(command == "PIDRESET") {
        this->pids[Input::roll].reset();
        this->pids[Input::pitch].reset();
    } else if(command == "PIDVALUES") {
        this->pids[Input::roll].values(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3]));
        this->pids[Input::pitch].values(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3]));
    }
    std::cout.flush();
}

void Quadcopter::imuReset() {

}

void Quadcopter::stabilize() {


    // The quaternion that can convert a vector in body coordinates
    // to ground coordinates when it its changed to a matrix.
    quaternion rotation = quaternion::Identity();

    int start = millis(); // truncate 64-bit return value
    while (1) {

        int last_start = start;
        start = millis();
        double dt = (start - last_start) / 1000.0;
        if (dt < 0) { throw std::runtime_error("Time went backwards."); }

        vector angular_velocity = this->imu.readGyro();
        vector acceleration = this->imu.readAccel();
        vector magnetic_field = this->imu.readMag();

        fuse(rotation, dt, angular_velocity, acceleration, magnetic_field);

        //OLD: vector euler = rotation.toRotationMatrix().eulerAngles(2, 1, 0) * (180 / M_PI);
        std::vector<double> euler = myEulerAngles(rotation.toRotationMatrix());

        euler[0] *= (180 / M_PI); // PITCH
        euler[1] *= (180 / M_PI); // ROLL
        euler[2] *= (180 / M_PI); // YAW

        // Pitch is upside down. Change it.
        if (euler[0] > 0) {
            euler[0] -= 180;
        } else {
            euler[0] += 180;
        }

        this->sharedServer->sendEuler(euler);

        double targetRoll = 0, targetPitch = 0, targetYaw = 0;

        double pitchAdjust = this->pids[Input::pitch].compute(euler[0], targetPitch, dt);
        double rollAdjust  = this->pids[Input::roll].compute(euler[1], targetRoll, dt);
        double yawAdjust   = 0; //this->pids[Input::yaw]  .compute(euler[2], targetYaw,   dt);

        double frontLeftSpeed  = this->inputs[Input::throttle] - pitchAdjust + rollAdjust + yawAdjust;
        double frontRightSpeed = this->inputs[Input::throttle] - pitchAdjust - rollAdjust - yawAdjust;
        double backLeftSpeed   = this->inputs[Input::throttle] + pitchAdjust + rollAdjust - yawAdjust;
        double backRightSpeed  = this->inputs[Input::throttle] + pitchAdjust - rollAdjust + yawAdjust;

        if (this->SHOULD_STABILIZE) {

            //Motor::setSpeeds(frontLeftSpeed, frontRightSpeed, backLeftSpeed, backRightSpeed);

            Log::log("p\tr\ty");
            Log::log("%.2f\t%.2f\t%.2f", euler[0], euler[1], euler[2]);
            Log::log("%.2f\t%.2f\t%.2f", pitchAdjust, rollAdjust, yawAdjust);
            Log::log("%.2f, %.2f, %.2f %.2f", frontLeftSpeed, frontRightSpeed, backLeftSpeed, backRightSpeed);

            std::cout << std::endl;

        }


        //Log::log("%d ms", millis()-start);
        // Ensure that each iteration of the loop takes at least x ms.
        // TODO: Use std::thread::sleep_*
        while (millis() - start < 30) {
            usleep(1000);
        }
    }

}

