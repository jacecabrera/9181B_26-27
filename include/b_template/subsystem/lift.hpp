#ifndef B_TEMPLATE_SUBSYSTEM_LIFT_HPP
#define B_TEMPLATE_SUBSYSTEM_LIFT_HPP

#include "main.h"

namespace b_template {
namespace subsystem {

class Lift {
public:
    Lift();

    void move(int voltage);
    void stop();
    double get_position() const;

private:
    pros::MotorGroup lift_motors;
};

} // namespace subsystem
} // namespace b_template

#endif // B_TEMPLATE_SUBSYSTEM_LIFT_HPP