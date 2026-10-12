#include "globals.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// ports and signs are placeholders. Confirm at practice.

pros::MotorGroup left_mg({1, 2, 3});   // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({-4, -5, -6});   // Creates a motor group with forwards port 5 and reversed ports 4 & 6