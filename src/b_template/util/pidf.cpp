#include "b_template/util/pidf.hpp"
#include <cmath>
#include <algorithm>

namespace b_template {
namespace utils {

PIDF::PIDF(double kP, double kI, double kD, double kF, double kG, double starti, double max_i)
    : kP(kP), kI(kI), kD(kD), kF(kF), kG(kG), starti(starti), max_i(max_i) {}

void PIDF::set_target(double target) {
    this->target = target;
    //reset();
}

double PIDF::compute(double current, double target_vel) {
    return compute_error(target - current, target_vel);
}

double PIDF::compute_error(double err, double target_vel) {
    error = err;
    if (primed && ((error > 0.0) != (prev_error > 0.0))) integral = 0.0;

    if (kI == 0.0) {
        integral = 0.0;
    } else if (starti == 0.0 || std::fabs(error) < starti) {
        integral += error;
    } else {
        integral = 0.0;
    }
    if (max_i > 0.0) integral = std::clamp(integral, -max_i, max_i);

    derivative = primed ? (error - prev_error) : 0.0;
    prev_error = error;
    primed = true;

    double feedback = (error * kP) + (integral * kI) + (derivative * kD);
    double feedforward = (target_vel * kF) + kG;
    return feedback + feedforward;
}

void PIDF::reset() {
    error = 0.0;
    prev_error = 0.0;
    integral = 0.0;
    derivative = 0.0;
    primed = false;
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