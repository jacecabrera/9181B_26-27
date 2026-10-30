#include "b_template/util/pidf.hpp"
#include <cmath>

namespace b_template {
namespace utils {

PIDF::PIDF(double kP, double kI, double kD, double kF, double kG, double starti, double max_i)
    : kP(kP), kI(kI), kD(kD), kF(kF), kG(kG), starti(starti), max_i(max_i) {}

void PIDF::set_target(double target) {
    this->target = target;
    //reset();
}

double PIDF::compute(double current, double target_vel) {
    error = target - current;

    //integral accumulation logic
    if (starti == 0.0 || std::fabs(error) < starti) {
        integral += error;
    } else {
        //integral = 0.0;
    }

    if (max_i > 0.0) {
        if (integral > max_i) integral = max_i;
        if (integral < -max_i) integral = -max_i;
    }

    //derivative calculation
    derivative = error - prev_error;
    prev_error = error;

    //feedback terms (PID)
    double feedback = (error * kP) + (integral * kI) + (derivative * kD);

    //feedforward terms (Velocity kF + Gravity kG)
    double feedforward = (target_vel * kF) + kG;

    return feedback + feedforward;
}

void PIDF::reset() {
    error = 0.0;
    prev_error = 0.0;
    integral = 0.0;
    derivative = 0.0;
}

void PIDF::set_constants(double kP, double kI, double kD, double kF, double kG, double starti, double max_i) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
    this->kF = kF;
    this->kG = kG;
    this->starti = starti;
    this->max_i = max_i;
}

double PIDF::get_target() const { return target; }
double PIDF::get_error() const { return error; }

} //namespace utils
} //namespace b_template