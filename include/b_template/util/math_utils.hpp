#pragma once
#include <numbers>
#include <cmath>

//std::atan2(y, x) : 'computes the arctangent (inverse tangent) of a quotient y / x'
//std:hypot(x, y) : 'computes the square root of the sum of the squares of its arguments'

namespace b_template {
    namespace math {
        //conversion utilites

        inline constexpr double deg_to_rad(double deg) {
            //multiply the inputted degree by (pi/180)
            return deg * (std::numbers::pi / 180.0); // 180.0 instead of 180 to avoid integer truncation (to be safe)
        }

        //opposite of top function
        inline constexpr double rad_to_deg(double rad) {
            return rad * (180.0 / std::numbers::pi);
        }

        //conversions between rotations and distance travelled


        //constexpr is used because there are no "dynamically changing(?)" variables; able to calculate during compile
        inline constexpr double encoder_to_dist(double encoder_deg, double wheel_diameter, double gear_ratio = 1.0) {
            double circumference = std::numbers::pi * wheel_diameter;
            return (encoder_deg / 360.0) * gear_ratio  * circumference;
        }

        inline constexpr double dist_to_encoder(double wheel_inch, double wheel_diameter, double gear_ratio = 1.0) {
            double circumference = std::numbers::pi * wheel_diameter;
            return (wheel_inch / circumference) * (360.0 / gear_ratio);
        }

        //ips = inches-per-second
        inline constexpr double ips_to_rpm(double ips, double wheel_diameter, double gear_ratio = 1.0) {
            double circumference = std::numbers::pi * wheel_diameter;
            return (ips / circumference) * 60.0 / gear_ratio;
        }

        //coordinate system conversion (polar<->cartesian)
        //void functions because the function isnt supposed to formally return anything

        inline void cartesian_to_polar(double x, double y, double& out_r, double& out_theta) {
            out_r = std::hypot(x, y);
            out_theta = std::atan2(y, x);
        }

        inline void polar_to_cartesian(double r, double theta, double& out_x, double& out_y) {
            out_x = r * std::cos(theta);
            out_y = r * std::sin(theta);
        }
    }
}