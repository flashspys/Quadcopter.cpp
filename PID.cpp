//
// Created by Felix Wehnert on 06.02.2017.
//

#include "PID.hpp"
#include <iostream>
PID::PID(double p, double i, double d) : p(p), i(i), d(d) {

}

double PID::compute(double input, double target, double dt) {
	double error, p_out, i_out, d_out;

	error = target - input;
	p_out = this->p * error;

	this->i_error += error * dt;

    std::cout << this->i_error << std::endl;

    i_out = this->i * this->i_error;

	d_out = this->d * ( (error - this->lastError) / dt );

	this->lastError = error;

	return (p_out + i_out + d_out) / 100.0;
}

void PID::reset() {
    this->i_error = 0;
    this->lastError = 0;
}

void PID::values(double p, double i, double d) {
    this->p = p;
    this->i = i;
    this->d = d;
}