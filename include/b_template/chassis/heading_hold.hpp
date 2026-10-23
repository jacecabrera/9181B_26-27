#pragma once

#include "main.h"
#include "drive_accel.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>


/* Driver-control for driving straight
With the turn stick neutral and throttle applied, latches the IMU heading
and adjusts left/right motor power (PD) to hold it. Inactive while turning.
*/
// Set to false to disable telemetry printing during competition matches
constexpr bool HEADING_DEBUG = true;

struct PIDController {
    double kp;
    double kd;

    double calculate_p(double error) {
        return kp * error;
    }

    double calculate_d(double gyro_rate) {
        return kd * (-gyro_rate);
    }
};

// Tune PID constants in config.hpp
// Automatically catch errors 
struct HeadingHoldSystem {
    PIDController pid{2.0, 0.08};
    bool is_holding = false;
    double target_heading = 0.0;
    std::uint32_t last_turn_time = 0;   // Uses uint32_t to match pros::millis() return type
    std::uint32_t last_print_time = 0;
    bool imu_error_notified = false;

    // Ramped forward power, kept between update() calls so the ramp can build up
    double forward_command = 0.0;
    std::uint32_t previous_update_time_ms = 0;

// Reset current state and error
    void reset() {
        is_holding = false;
        imu_error_notified = false;
        forward_command = 0.0;
        previous_update_time_ms = pros::millis();
    }

    void update(pros::Controller& master, pros::MotorGroup& left_mg, pros::MotorGroup& right_mg, pros::Imu& imu) {

        int throttle = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // Reads vertical position of left joystick
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);    // Reads horizontal position of right joystick
        const int deadband = 5;
        const double error_deadband = 1.0;
        const std::uint32_t latch_delay_ms = 100; // Temporarily increase to ~600 when measuring spin-down on the floor

        auto drive_raw = [&](int lp, int rp) {
            left_mg.move(std::clamp(lp, -127, 127));
            right_mg.move(std::clamp(rp, -127, 127));
        };

        // Measures real time since the last update, then ramps the forward power.
        // Turn is converted with the same deadband but never ramped, so it has no delay.
        std::uint32_t now_ms = pros::millis();
        double seconds_since_last_update = (now_ms - previous_update_time_ms) / 1000.0;
        previous_update_time_ms = now_ms;
        forward_command = b_template::drive_accel::ramp_forward_command(
            throttle, forward_command, seconds_since_last_update, deadband);
        double turn_command = b_template::drive_accel::stick_to_command(turn, deadband);


        // Both sticks neutral: release hold and stop
        if (std::abs(throttle) <= deadband && std::abs(turn) <= deadband) {
            is_holding = false;
            drive_raw(std::lround(forward_command), std::lround(forward_command));
            return;
        }

        // Active turning: bypass hold, update last turn timestamp continuously
        if (std::abs(turn) > deadband) {
            is_holding = false;
            last_turn_time = pros::millis();
            drive_raw(std::lround(forward_command + turn_command), std::lround(forward_command - turn_command));
            return;
        }

        // Ensures IMU readings with fallback and single-fire error notification
        double current_heading = imu.get_heading();
        double gyro_rate = imu.get_gyro_rate().z;
        if (!std::isfinite(current_heading) || !std::isfinite(gyro_rate)) {
            if (!imu_error_notified) {
                master.rumble(".---");
                if constexpr (HEADING_DEBUG) {
                    std::printf("[HeadingHold] ERROR: invalid IMU read\n");
                }
                imu_error_notified = true;
            }
            drive_raw(std::lround(forward_command), std::lround(forward_command));
            return;
        }

        // Coasts for a short period after turning before latching the heading
        std::uint32_t current_time = pros::millis();
        if (!is_holding) {
            if constexpr (HEADING_DEBUG) {
                std::printf("[HeadingHold waiting] elapsed=%lu ms | rate=%.2f deg/s\n",
                            (unsigned long)(current_time - last_turn_time), gyro_rate);
            }

            if (current_time - last_turn_time >= latch_delay_ms) {
                target_heading = current_heading;
                is_holding = true;
            } else {
                drive_raw(std::lround(forward_command), std::lround(forward_command));
                return;
            }
        }

        // Error calculation with wrap
        double error = target_heading - current_heading;
        if (error > 180.0) error -= 360.0;
        if (error < -180.0) error += 360.0;

        // Proportional reacts to position error
        // Derivative reacts to gyro rate
        double p_correction = 0.0;
        if (std::abs(error) > error_deadband) {
            double adjusted_error = (error > 0.0) ? (error - error_deadband) : (error + error_deadband);
            p_correction = pid.calculate_p(adjusted_error);
        }
        double d_correction = pid.calculate_d(gyro_rate);
        double correction = p_correction + d_correction;

        // Proportional to throttle
        double max_correction = std::min(40.0, 0.3 * std::abs(forward_command));
        correction = std::clamp(correction, -max_correction, max_correction);

        
        // Mix powers
        int left_power = std::lround(forward_command + correction);
        int right_power = std::lround(forward_command - correction);

        // Combines throttle and steering correction into motor powers
        if (left_power > 127) {
            int excess = left_power - 127;
            left_power -= excess;
            right_power -= excess;
        } else if (left_power < -127) {
            int excess = -127 - left_power;
            left_power += excess;
            right_power += excess;
        }
        if (right_power > 127) {
            int excess = right_power - 127;
            left_power -= excess;
            right_power -= excess;
        } else if (right_power < -127) {
            int excess = -127 - right_power;
            left_power += excess;
            right_power += excess;
        }

        // Formats and prints component values every 200ms for tuning and debugging
        if constexpr (HEADING_DEBUG) {
            if (current_time - last_print_time >= 200) {
                std::printf("[HeadingHold] Target: %.2f | Curr: %.2f | Err: %.2f | Rate: %.2f | P: %.2f | D: %.2f | Tot: %.2f\n",
                            target_heading, current_heading, error, gyro_rate, p_correction, d_correction, correction);
                last_print_time = current_time;
            }
        }

        drive_raw(left_power, right_power);
    }
};

// Accessible global instance of the heading hold system
extern HeadingHoldSystem heading_hold;