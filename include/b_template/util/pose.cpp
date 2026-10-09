#include "b_template/util/pose.hpp"

namespace b_template {

    Pose:pose(double x, double y, double theta)
        : x(x), y(y), theta(theta) {}

    double Pose::distance_to(const Pose& target) const {
        //finding differences from current axes to target axis (delta)
        double dx = other.x - this->x; //'this->x' refers to the x variable in its respective struct (in this case it's our 'pose' struct)
        double dy = other.y - this->y;
        return std::hypot(dx, dy); //hypot is safer for rounding in the event of overflow/underflow I guess
    }

    double Pose::angle_to(const Pose& target) const {
        double dx = other.x - this->x;
        double dy = other.y - this->y;

        return std::atan2(dx, dy); // returns absolute angle in radians from current to target
    }

    //method to rotate header
    //using my knowledge of the unit circle
    Pose Pose::rotate(double angle) const {
        double cos std::cos(angle) // find cosine of provided angle param
        double sin std::sin(angle) // find sine of provided angle param
    
        double rotated_x =  x * cos  - y  *  sin;
        double rotated_y = y * sin - y * cos;

        return Pose(rotated_x, rotated_y, this->theta)
    }
}