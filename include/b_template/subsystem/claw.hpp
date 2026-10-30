#ifndef B_TEMPLATE_SUBSYSTEM_CLAW_HPP
#define B_TEMPLATE_SUBSYSTEM_CLAW_HPP

#include "main.h"

namespace b_template {
namespace subsystem {

class Claw {
private:
    pros::v5::Motor claw_motor;

public:
    Claw(int port, bool reverse = false);
    void spin(int voltage);
    void stop();
};

} // namespace subsystem
} // namespace b_template

#endif // B_TEMPLATE_SUBSYSTEM_CLAW_HPP