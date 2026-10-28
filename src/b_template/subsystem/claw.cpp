#include "b_template/subsystem/claw.hpp"

// Constructor with a default value for 'reverse' so 1-argument instantiation works
Claw::Claw(int port, bool reverse) : claw_motor(reverse ? -port : port) {}

void Claw::spin(int voltage) {
    claw_motor.move(voltage);
}

void Claw::stop() {
    claw_motor.move(0);
}

Claw claw(7);