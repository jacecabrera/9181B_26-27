#include "globals.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::MotorGroup left_mg({4, 3, 1});
pros::MotorGroup right_mg({-6, -10, -8}); 