#pragma once
#include "globals.hpp"

class Claw {
private:
    pros::v5::Motor claw_motor;

public:
    Claw(int port, bool reverse = false);
    void spin(int voltage);
    void stop();
};

extern Claw claw;