#pragma once
#include "api.h"

// All measurements are denoted in inches (usually)
namespace b_template {
    namespace config {
        // Wheel and drive dimensions
        inline constexpr double WHEEL_DIAMETER = 2.75;
        inline constexpr double GEAR_RATIO = 36.0 / 48.0; // driven_gear/driving_gear aka. output/input
        inline constexpr double TRACK_WIDTH = 0.0;        // distance between the midpoints of the left and right side wheels

        // Odom
        inline constexpr double X_ODOM_DIAMETER = 0.0;
        inline constexpr double X_ODOM_CENTER_OFFSET = 0.0; // distance from the center of the odom wheel to the center of the bot as a whole (from a top down view)
        inline constexpr double Y_ODOM_DIAMETER = 0.0;
        inline constexpr double Y_ODOM_CENTER_OFFSET = 0.0;

        // PID constants
        inline constexpr double LATERAL_kP = 0.0;
        inline constexpr double LATERAL_kI = 0.0;
        inline constexpr double LATERAL_kD = 0.0;
        
        inline constexpr double ANGULAR_kP = 0.0;
        inline constexpr double ANGULAR_kI = 0.0;
        inline constexpr double ANGULAR_kD = 0.0;
    }

    static_assert(config::WHEEL_DIAMETER > 0.0 && config::GEAR_RATIO > 0.0,
              "set WHEEL_DIAMETER and GEAR_RATIO in config.hpp");
}

//Changed nullptr to 0.0 because nullptr is not a valid double value and contexpr is not initialized.