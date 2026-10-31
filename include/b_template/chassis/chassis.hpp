#ifndef B_TEMPLATE_CHASSIS_CHASSIS_HPP
#define B_TEMPLATE_CHASSIS_CHASSIS_HPP

#include "b_template/util/pidf.hpp"
#include "main.h"

namespace b_template {
namespace chassis {

class Chassis {
public:
    Chassis(pros::MotorGroup& left_motors, pros::MotorGroup& right_motors, pros::IMU& imu);

    /** target_inches is in INCHES. */
    void drive_distance(double target_inches, utils::PIDF drive_pidf, double timeout_ms = 3000);

    /** Heading in degrees, clockwise, 0-360. Takes the short way round. */
    void turn_to_angle(double target_heading, utils::PIDF turn_pidf, double timeout_ms = 2000);

private:
    double avg_position(pros::MotorGroup& g) const;

    pros::MotorGroup& left_motors;
    pros::MotorGroup& right_motors;
    pros::IMU& imu;
};

} // namespace chassis
} // namespace b_template

#endif