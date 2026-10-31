#ifndef B_TEMPLATE_SUBSYSTEM_CLAW_HPP
#define B_TEMPLATE_SUBSYSTEM_CLAW_HPP

#include "main.h"

namespace b_template {
namespace subsystem {

class Claw {
private:
    pros::v5::Motor claw_motor;
    int stall_ticks = 0;
    int last_dir = 0;
    static constexpr int stall_power = 40;
    static constexpr int stall_limit = 25;   // 25 loops * 10 ms

public:
    Claw(int port, bool reverse = false);
    void init();               // call once from initialize()
    void spin(int power);      // -127..127
    void stop();
};

} // namespace subsystem
} // namespace b_template

#endif