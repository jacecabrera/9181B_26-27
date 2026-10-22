#ifndef B_TEMPLATE_CHASSIS_CHASSIS_HPP
#define B_TEMPLATE_CHASSIS_CHASSIS_HPP

#include "b_template/utils/pidf.hpp"
#include "main.h"

namespace b_template {
namespace chassis {

class Chassis {
public:
    Chassis(pros::MotorGroup& left_motors, pros::MotorGroup& right_motors, pros::IMU& imu);

    /**
     * @brief Drive straight for a specified distance in encoder ticks/inches.
     */
    void drive_distance(double target_dist, utils::PIDF drive_pidf, double timeout_ms = 3000);

    /**
     * @brief Turn to an absolute heading using the IMU.
     */
    void turn_to_angle(double target_heading, utils::PIDF turn_pidf, double timeout_ms = 2000);

private:
    pros::MotorGroup& left_motors;
    pros::MotorGroup& right_motors;
    pros::IMU& imu;
};

} // namespace chassis
} // namespace b_template

#endif // B_TEMPLATE_CHASSIS_CHASSIS_HPP