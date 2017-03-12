//
// Created by Felix Wehnert on 06.02.2017.
//

#ifndef QUADROCOPTER_PID_HPP
#define QUADROCOPTER_PID_HPP


class PID {

public:
	PID(double p, double i, double d);
	PID() {
		this->p = this->i = this->d = 0;
	}

	double compute(double input, double target, double dt);

private:
	double p;
	double i;
	double d;
	double i_error = 0;
	double lastError = 0;
};


#endif //QUADROCOPTER_PID_HPP
