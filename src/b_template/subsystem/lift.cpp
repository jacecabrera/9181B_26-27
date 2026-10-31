#include "b_template/subsystem/lift.hpp"
#include <algorithm>
#include <cmath>
#include <mutex>

namespace b_template {
namespace subsystem {

Lift::Lift()
    : lift_motors({20, -11}, pros::MotorGearset::green),   // set to YOUR cartridge
      lift_pidf(20.0, 0.3, 40.0, 0.0, 1200.0, 40.0, 3000.0) {}   // starting points, tune

void Lift::init() {
    std::lock_guard<pros::Mutex> lock(mtx);
    lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift_motors.tare_position_all();
    current_target = 0.0;
    lift_pidf.reset();
    lift_pidf.set_target(current_target);

    if (!task_) {
        task_ = std::make_unique<pros::Task>([this] {
            while (true) {
                update();
                pros::delay(10);
            }
        });
    }
}

double Lift::position() const {
    double sum = 0.0;
    int n = 0;
    for (double p : lift_motors.get_position_all()) {
        if (std::isfinite(p)) { sum += p; ++n; }
    }
    return n ? sum / n : std::nan("");
}

void Lift::jog(double delta) {
    double pos = position();
    if (!std::isfinite(pos)) return;
    std::lock_guard<pros::Mutex> lock(mtx);
    current_target = std::clamp(current_target + delta, pos - max_lead, pos + max_lead);
    current_target = std::clamp(current_target, min_height, max_height);
    lift_pidf.set_target(current_target);
}

void Lift::hold() {
    double pos = position();
    if (!std::isfinite(pos)) return;
    std::lock_guard<pros::Mutex> lock(mtx);
    current_target = std::clamp(pos, min_height, max_height);
    lift_pidf.set_target(current_target);
}

void Lift::set_target(double target) {
    std::lock_guard<pros::Mutex> lock(mtx);
    current_target = std::clamp(target, min_height, max_height);
    lift_pidf.reset();
    lift_pidf.set_target(current_target);
}

void Lift::update() {
    double pos = position();
    std::lock_guard<pros::Mutex> lock(mtx);

    if (!std::isfinite(pos)) {
        lift_motors.move_voltage(0);
        return;
    }

    double out = lift_pidf.compute(pos);

    if (current_target <= min_height + rest_band && pos <= min_height + rest_band) {
        out = 0.0;
        lift_pidf.reset();
    }

    out = std::clamp(out, -12000.0, 12000.0);
    lift_motors.move_voltage(static_cast<std::int32_t>(out));
}

double Lift::get_position() const { return position(); }
double Lift::get_target() const {
    std::lock_guard<pros::Mutex> lock(mtx);
    return current_target;
}
bool Lift::at_target(double tolerance) const {
    return std::fabs(get_target() - position()) < tolerance;
}

} // namespace subsystem
} // namespace b_template