#include "b_template/chassis/chassis.hpp"
#include <cmath>

namespace b_template {
namespace chassis {

Chassis::Chassis(pros::MotorGroup& left_motors, pros::MotorGroup& right_motors, pros::IMU& imu, 
                 utils::PIDF drive_pidf, utils::PIDF turn_pidf)
    : left_motors(left_motors), right_motors(right_motors), imu(imu),
      drive_pid(drive_pidf), turn_pid(turn_pidf) {}

void Chassis::drive_distance(double target_dist, double timeout_ms) {
    drive_pid.set_target(target_dist);
    left_motors.tare_position();
    right_motors.tare_position();

    uint32_t start_time = pros::millis();
    int settle_count = 0;

    while (true) {
        double current_pos = (left_motors.get_position() + right_motors.get_position()) / 2.0;
        double output = drive_pid.compute(current_pos);

        // Clamp output to max motor voltage (12000 mV)
        if (output > 12000.0) output = 12000.0;
        if (output < -12000.0) output = -12000.0;

        left_motors.move_voltage(output);
        right_motors.move_voltage(output);

        // Settling logic: must stay within error threshold for 15 consecutive loops (~150ms)
        if (std::fabs(drive_pid.get_error()) < 10.0) {
            settle_count++;
            if (settle_count > 15 || (pros::millis() - start_time) > timeout_ms) {
                break;
            }
        } else {
            settle_count = 0;
            if ((pros::millis() - start_time) > timeout_ms) {
                break;
            }
        }

        pros::delay(10);
    }

    left_motors.move_voltage(0);
    right_motors.move_voltage(0);
}

void Chassis::turn_to_angle(double target_heading, double timeout_ms) {
    turn_pid.set_target(target_heading);
    uint32_t start_time = pros::millis();
    int settle_count = 0;

    while (true) {
        double current_heading = imu.get_heading();
        
        // Compute raw PID output based on heading error
        double output = turn_pid.compute(current_heading);

        // FIX: Normalize angle error across 360-degree boundary for shortest path
        double error = target_heading - current_heading;
        while (error > 180.0) error -= 360.0;
        while (error < -180.0) error += 360.0;

        // Clamp output voltage
        if (output > 12000.0) output = 12000.0;
        if (output < -12000.0) output = -12000.0;

        left_motors.move_voltage(output);
        right_motors.move_voltage(-output);

        // Settling check with error normalization
        if (std::fabs(error) < 1.5) {
            settle_count++;
            if (settle_count > 15 || (pros::millis() - start_time) > timeout_ms) {
                break;
            }
        } else {
            settle_count = 0;
            if ((pros::millis() - start_time) > timeout_ms) {
                break;
            }
        }

        pros::delay(10);
    }

    left_motors.move_voltage(0);
    right_motors.move_voltage(0);
}

} // namespace chassis
} // namespace b_template