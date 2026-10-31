#include "b_template/subsystem/lift.hpp"
#include <cmath>

namespace b_template {
namespace subsystem {

Lift::Lift()
    : lift_motors({20, -11}, pros::MotorGearset::green) // Port 11 reversed so motors don't fight
{
    lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
}

void Lift::move(int voltage) {
    lift_motors.move(voltage);
}

void Lift::stop() {
    lift_motors.move(0); // Brake mode HOLD takes over here automatically
}

double Lift::get_position() const {
    double sum = 0.0;
    int n = 0;
    for (double p : lift_motors.get_position_all()) {
        if (std::isfinite(p)) { sum += p; ++n; }
    }
    return n ? sum / n : std::nan("");
}

} // namespace subsystem
} // namespace b_template