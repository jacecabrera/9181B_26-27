#include "b_template/chassis/odom.hpp"
#include "pros/error.h"
#include <cmath>

namespace b_template {
namespace chassis {

namespace {
    constexpr double PI = 3.14159265358979323846;
    double to_rad(double deg) { return deg * PI / 180.0; }

    // Convert rotation sensor position to inches traveled by the wheel
    double wheel_inches(pros::Rotation& r, double diameter) {
        int32_t raw = r.get_position();
        if (raw == PROS_ERR) return std::nan("");
     return raw / 100.0 / 360.0 * PI * diameter;   // centidegrees -> inches
 }
}

odom::odom(pros::Rotation& vert, pros::Rotation& horiz, pros::Imu& imu,
           double vert_diameter, double horiz_diameter,
           double vert_offset, double horiz_offset)
    : vert_(vert), horiz_(horiz), imu_(imu),
      vert_d_(vert_diameter), horiz_d_(horiz_diameter),
      vert_offset_(vert_offset), horiz_offset_(horiz_offset) {}

// Start the odometry task, which continuously updates the robot's pose
void odom::start_task() {
    vert_.reset_position();
    horiz_.reset_position();
    prev_vert_pos_ = prev_horiz_pos_ = 0.0;
    prev_theta_ = to_rad(imu_.get_rotation());  //first reading of IMU
    current_pose_.theta = imu_.get_rotation();  //Initialize current pose with IMU reading

    task_ = std::make_unique<pros::Task>([this] {
        while (true) {
            update();
            pros::delay(10);
        }
    }
 );
}

void odom::update() {
    std::lock_guard<pros::Mutex> lock(pose_mutex_);

    double vert = wheel_inches(vert_, vert_d_);
    double horiz = wheel_inches(horiz_, horiz_d_);
    double heading_deg = imu_.get_rotation();

    // If readings are invalid, skip update
    if (std::isnan(vert) || std::isnan(horiz) ||
        std::isinf(heading_deg) || std::isnan(heading_deg)) return;
    double theta = to_rad(heading_deg);

    //movement since last update
    double d_vert = vert - prev_vert_pos_;
    double d_horiz = horiz - prev_horiz_pos_;
    double d_theta = theta - prev_theta_;
    prev_vert_pos_ = vert;
    prev_horiz_pos_ = horiz;
    prev_theta_ = theta;

    // local displacement, corrected for the arc the robot drove while turning
    double local_x, local_y;
    if (std::fabs(d_theta) < 1e-6) {
        local_x = d_horiz;
        local_y = d_vert;
    } else {
        double chord = 2.0 * std::sin(d_theta / 2.0);
        local_x = chord * (d_horiz / d_theta + horiz_offset_);
        local_y = chord * (d_vert / d_theta + vert_offset_);
    }

    // rotate local frame into field frame using the average heading
    double avg = theta - d_theta / 2.0;
    current_pose_.x += local_y * std::sin(avg) + local_x * std::cos(avg);
    current_pose_.y += local_y * std::cos(avg) - local_x * std::sin(avg);
    current_pose_.theta = heading_deg; 
    //Stores heading in degrees
}

pose odom::get_pose() {
    std::lock_guard<pros::Mutex> lock(pose_mutex_);
    return current_pose_;
}

void odom::set_pose(double x, double y, double theta_deg) {
    std::lock_guard<pros::Mutex> lock(pose_mutex_);
    current_pose_.x = x;
    current_pose_.y = y;
    current_pose_.theta = theta_deg;
    imu_.set_rotation(theta_deg);
    prev_theta_ = to_rad(theta_deg);
}

} // namespace chassis
} // namespace b_template