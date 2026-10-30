#pragma once
#include "main.h"
#include "b_template/subsystem/lift.hpp"
#include "b_template/subsystem/claw.hpp"
#include "b_template/chassis/heading_hold.hpp"

// Global objects

extern pros::Controller master;
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;
extern pros::Imu imu;

// Subsystem & Chassis Globals
extern HeadingHoldSystem heading_hold;
extern b_template::subsystem::Lift lift;
extern b_template::subsystem::Claw claw;

const double jog_speed = 15.0; // Ticks per 10ms loop