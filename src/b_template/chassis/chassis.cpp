#include "b_template/chassis/chassis.hpp"
#include <cmath>

namespace b_template {
namespace chassis {

Chassis::Chassis(pros::MotorGroup& left_motors, pros::MotorGroup& right_motors, pros::IMU& imu)
    : left_motors(left_motors), right_motors(right_motors), imu(imu) {}

void Chassis::drive_distance(double target_dist, utils::PIDF drive_pidf, double timeout_ms) {
    drive_pidf.set_target(target_dist);
    left_motors.tare_position();
    right_motors.tare_position();

    uint32_t start_time = pros::millis();

    while (true) {
        //avg pos of left and right sides
        double current_pos = (left_motors.get_position() + right_motors.get_position()) / 2.0;
        double output = drive_pidf.compute(current_pos);

        left_motors.move_voltage(output);
        right_motors.move_voltage(output);

        if (std::fabs(drive_pidf.get_error()) < 10.0 || (pros::millis() - start_time) > timeout_ms) {
            break;
        }

        pros::delay(10);
    }

    left_motors.move_voltage(0);
    right_motors.move_voltage(0);
}

void Chassis::turn_to_angle(double target_heading, utils::PIDF turn_pidf, double timeout_ms) {
    turn_pidf.set_target(target_heading);
    uint32_t start_time = pros::millis();

    while (true) {
        double current_heading = imu.get_heading();
        
        // Compute output using heading feedback
        double output = turn_pidf.compute(current_heading);

        left_motors.move_voltage(output);
        right_motors.move_voltage(-output);

        if (std::fabs(turn_pidf.get_error()) < 1.5 || (pros::millis() - start_time) > timeout_ms) {
            break;
        }

        pros::delay(10);
    }

    left_motors.move_voltage(0);
    right_motors.move_voltage(0);
}

} // namespace chassis
} // namespace b_template