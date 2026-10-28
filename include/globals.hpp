#pragma once
#include "main.h"
#include "b_template/chassis/heading_hold.hpp"

/// Global objects
extern pros::Controller master;

extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;
extern pros::Imu imu;

extern HeadingHoldSystem heading_hold;