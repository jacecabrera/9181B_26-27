#pragma once

#include "api.h"

//all measurements are denoted in inches (usually)
namespace b_template {

    namespace config {
        //wheel, drive dimensions
        inline constexpr double WHEEL_DIAMETER  = 2.75;
        inline constexpr double GEAR_RATIO = 36/48 // driven_gear/driving_gear aka. output/input
        inline constexpr double TRACK_WIDTH = nullptr // distance between the midpoints of the left and right side wheels

        //odom
        inline constexpr double X_ODOM_DIAMETER = nullptr
        inline constexpr double X_ODOM_CENTER_OFFSET = nullptr // distance from the center of the odom wheel to the center of the bot as a whole (from a top down view)
        inline constexpr double Y_ODOM_DIAMETER = nullptr
        inline constexpr double Y_ODOM_CENTER_OFFSET = nullptr

        //pid constants
        inline constexpr double LATERAL_kP
        inline constexpr double LATERAL_kI
        inline constexpr double LATERAL_kD

        inline constexpr double ANGULAR_kP
        inline constexpr double ANGULAR_kI
        inline constexpr double ANGULAR_kD
    }

}