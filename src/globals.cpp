#include "globals.hpp"

// --- Controller ---
pros::Controller master(pros::E_CONTROLLER_MASTER);

// --- Drivetrain Motor Groups & Sensors ---
// (Adjust port numbers to match your actual robot wiring)// -12, 19
pros::MotorGroup left_mg({-6, -16, -12}, pros::MotorGearset::blue); //-12,-16, -6
pros::MotorGroup right_mg({8, 3, 19}, pros::MotorGearset::blue); //19, 8, 3
pros::Imu imu(13);

// --- Subsystems & Chassis Control ---
HeadingHoldSystem heading_hold;
b_template::subsystem::Lift lift;
b_template::subsystem::Claw claw(7); // Claw plugged into port 7
b_template::chassis::Chassis drive_chassis(left_mg, right_mg, imu);