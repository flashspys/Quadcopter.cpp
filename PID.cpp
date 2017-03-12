//
// Created by Felix Wehnert on 06.02.2017.
//

#include "PID.hpp"

PID::PID(double p, double i, double d) : p(p), i(i), d(d) {

}

double PID::compute(double input, double target, double dt) {
	double error, p_error, i_error, d_error;

	error = target - input;
	p_error = error;

	this->i_error += (error + this->lastError) * dt;
	i_error = this->i_error;
	d_error = (error - this->lastError) / dt;

	double p_output = this->p * p_error;
	double i_output = this->i * i_error;
	double d_output = this->d * d_error;
	this->lastError = error;

	return (p_output + i_output + d_output) / 100.0;
}


