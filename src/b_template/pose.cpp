#include "b_template/util/pose.hpp"
#include <cmath>

namespace b_template {

    Pose::Pose(double x, double y, double theta)
        : x(x), y(y), theta(theta) {}

    double Pose::distance_to(const Pose& target) const {
        //finding differences from current axes to target axis (delta)
        double dx = target.x - this->x; //'this->x' refers to the x variable in its respective struct (in this case it's our 'pose' struct)
        double dy = target.y - this->y;
        return std::hypot(dx, dy); //hypot is safer for rounding in the event of overflow/underflow I guess
    }

    double Pose::angle_to(const Pose& target) const {
        double dx = target.x - this->x;
        double dy = target.y - this->y;

        // FIXED: atan2 expects (y, x), so dy must go first, then dx
        return std::atan2(dy, dx); // returns absolute angle in radians from current to target
    }

    //method to rotate header
    //using my knowledge of the unit circle
    Pose Pose::rotate(double angle) const {
        double c = std::cos(angle); // find cosine of provided angle param
        double s = std::sin(angle); // find sine of provided angle param
    
        // 2d rotation equations
        double rotated_x = (x * c) - (y * s);
        double rotated_y = (x * s) + (y * c);

        // return new pose with the rotated position and updated heading
        return Pose(rotated_x, rotated_y, this->theta + angle);
    }
}