#include "b_template/subsystem/claw.hpp"
#include <algorithm>
#include <cmath>

namespace b_template {
namespace subsystem {

Claw::Claw(int port, bool reverse) : claw_motor(reverse ? -port : port) {}

void Claw::init() {
    claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void Claw::spin(int power) {
    int dir = (power > 0) - (power < 0);
    if (dir != last_dir) stall_ticks = 0;
    last_dir = dir;

    if (dir != 0 && std::fabs(claw_motor.get_actual_velocity()) < 5.0) {
        if (++stall_ticks > stall_limit) power = std::clamp(power, -stall_power, stall_power);
    } else {
        stall_ticks = 0;
    }
    claw_motor.move(power);
}

void Claw::stop() {
    stall_ticks = 0;
    last_dir = 0;
    claw_motor.brake();
}

} // namespace subsystem
} // namespace b_template