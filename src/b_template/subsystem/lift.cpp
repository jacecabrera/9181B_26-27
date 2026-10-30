#include "b_template/subsystem/lift.hpp"
#include <algorithm>

namespace b_template {
namespace subsystem {

Lift::Lift()
    : lift_motors({20, -11}), // Your dual lift motor ports (reverse one if needed)
      // kP = 0.8, kI = 0.001, kD = 0.25, kF = 0.0, kG = 1200.0 (Gravity mV compensation)
      lift_pidf(6.0, 0.001, 0.25, 0.0, 1200.0, 30.0, 5000.0),
      current_target(0.0) 
{
    lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    current_target = lift_motors.get_position();
    lift_pidf.set_target(current_target);
}

void Lift::jog(double delta) {
    current_target += delta;
    
    // Clamp between physical bounds
    if (current_target > max_height) current_target = max_height;
    if (current_target < min_height) current_target = min_height;
    
    lift_pidf.set_target(current_target);
}

void Lift::set_target(double target) {
    current_target = std::clamp(target, min_height, max_height);
    lift_pidf.set_target(current_target);
}

void Lift::update() {
    double current_pos = lift_motors.get_position();
    double output = lift_pidf.compute(current_pos);
    
    lift_motors.move_voltage(output);
}

double Lift::get_position() const { return lift_motors.get_position(); }
double Lift::get_target() const { return current_target; }

} // namespace subsystem
} // namespace b_template