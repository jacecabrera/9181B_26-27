#ifndef B_TEMPLATE_CHASSIS_ODOM_HPP
#define B_TEMPLATE_CHASSIS_ODOM_HPP

#include "pros/imu.hpp"
#include "pros/rotation.hpp"
#include "pros/rtos.hpp"
#include <memory>
#include <mutex>

namespace b_template {
namespace chassis {

struct pose {
    double x = 0.0;      // inches
    double y = 0.0;      // inches
    double theta = 0.0;  // degrees, clockwise from y
};

class odom {
public:
    odom(pros::Rotation& vert, pros::Rotation& horiz, pros::Imu& imu,
         double vert_diameter, double horiz_diameter,
         double vert_offset, double horiz_offset);

    void start_task();
    void update();
    pose get_pose();
    void set_pose(double x, double y, double theta_deg);

private:
    pros::Rotation& vert_;
    pros::Rotation& horiz_;
    pros::Imu& imu_;

    // declare current pose and previous pose variables
    double vert_d_, horiz_d_;
    double vert_offset_, horiz_offset_;
    pose current_pose_;

    // declare previous position variables
    double prev_vert_pos_ = 0.0;
    double prev_horiz_pos_ = 0.0;
    double prev_theta_ = 0.0;

    pros::Mutex pose_mutex_;
    std::unique_ptr<pros::Task> task_;
};

} // namespace chassis
} // namespace b_template

#endif // B_TEMPLATE_CHASSIS_ODOM_HPP