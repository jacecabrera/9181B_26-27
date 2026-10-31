#include "b_template/subsystem/claw.hpp"

namespace b_template {
namespace subsystem {

// Constructor implementation
Claw::Claw(int port, bool reverse) : claw_motor(reverse ? -port : port) {}

void Claw::spin(int voltage) {
    claw_motor.move(voltage);
}

void Claw::stop() {
    claw_motor.move(0);
}

} // namespace subsystem
} // namespace b_template