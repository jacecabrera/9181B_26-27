#include "globals.hpp"

// --- Controller ---
pros::Controller master(pros::E_CONTROLLER_MASTER);

// --- Drivetrain Motor Groups & Sensors ---
// (Adjust port numbers to match your actual robot wiring)
pros::MotorGroup left_mg({1, -2, 3}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({-4, 5, -6}, pros::MotorGearset::blue);
pros::Imu imu(10);

// --- Subsystems & Chassis Control ---
HeadingHoldSystem heading_hold;
b_template::subsystem::Lift lift;
b_template::subsystem::Claw claw(7); // Claw plugged into port 7
b_template::chassis::Chassis drive_chassis(left_mg, right_mg, imu);