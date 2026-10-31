#include "b_template/chassis/chassis.hpp"
#include "b_template/util/math_utils.hpp"
#include "config.hpp"
#include <algorithm>
#include <cmath>

namespace b_template {
namespace chassis {

namespace {
    double wrap180(double a) {
        while (a > 180.0)  a -= 360.0;
        while (a < -180.0) a += 360.0;
        return a;
    }
    constexpr double MAX_MV = 12000.0;
}

Chassis::Chassis(pros::MotorGroup& left_motors, pros::MotorGroup& right_motors, pros::IMU& imu)
    : left_motors(left_motors), right_motors(right_motors), imu(imu) {}

double Chassis::avg_position(pros::MotorGroup& g) const {
    double sum = 0.0; int n = 0;
    for (double p : g.get_position_all()) if (std::isfinite(p)) { sum += p; ++n; }
    return n ? sum / n : std::nan("");
}

void Chassis::drive_distance(double target_inches, utils::PIDF drive_pidf, double timeout_ms) {
    const double target_deg = math::dist_to_encoder(target_inches, config::WHEEL_DIAMETER, config::GEAR_RATIO);
    drive_pidf.reset();
    drive_pidf.set_target(target_deg);
    left_motors.tare_position_all();
    right_motors.tare_position_all();

    std::uint32_t start = pros::millis();
    int settled_ms = 0;

    while (pros::millis() - start < timeout_ms) {
        double pos = (avg_position(left_motors) + avg_position(right_motors)) / 2.0;
        if (!std::isfinite(pos)) break;

        double out = std::clamp(drive_pidf.compute(pos), -MAX_MV, MAX_MV);
        left_motors.move_voltage(static_cast<std::int32_t>(out));
        right_motors.move_voltage(static_cast<std::int32_t>(out));

        settled_ms = (std::fabs(drive_pidf.get_error()) < 15.0) ? settled_ms + 10 : 0;
        if (settled_ms >= 60) break;
        pros::delay(10);
    }
    //left_motors.move_voltage(0);
    //right_motors.move_voltage(0);
left_motors.brake();
right_motors.brake();
}

void Chassis::turn_to_angle(double target_heading, utils::PIDF turn_pidf, double timeout_ms) {
    turn_pidf.reset();
    std::uint32_t start = pros::millis();
    int settled_ms = 0;

    while (pros::millis() - start < timeout_ms) {
        double heading = imu.get_heading();
        if (!std::isfinite(heading)) break;

        double err = wrap180(target_heading - heading);
        double out = std::clamp(turn_pidf.compute_error(err), -MAX_MV, MAX_MV);
        left_motors.move_voltage(static_cast<std::int32_t>(out));
        right_motors.move_voltage(static_cast<std::int32_t>(-out));

        settled_ms = (std::fabs(err) < 1.5) ? settled_ms + 10 : 0;
        if (settled_ms >= 60) break;
        pros::delay(10);
    }
    left_motors.move_voltage(0);
    right_motors.move_voltage(0);
}

} // namespace chassis
} // namespace b_template