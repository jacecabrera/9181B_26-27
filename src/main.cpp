#include "main.h"
#include "b_template/auton/selector.hpp"
#include "globals.hpp"
#include "b_template/chassis/heading_hold.hpp"
#include <cstdlib>
#include <algorithm>

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    auton_selector_init();

    left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // Blocks until calibration finishes (about 2 s). Returns PROS_ERR on failure.
    if (imu.reset(true) == PROS_ERR) {
        pros::lcd::set_text(2, "IMU FAILED, check IMU port and restart");
    }

    auton_selector_init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    switch (selected_auton.routine) {
        case AutonRoutine::RED_GOAL_RUSH:
            // Run Red Goal Rush path
            break;
        case AutonRoutine::RED_SAFE_WP:
            // Run Red Safe Win Point path
            break;
        case AutonRoutine::BLUE_GOAL_RUSH:
            // Run Blue Goal Rush path
            break;
        case AutonRoutine::BLUE_SAFE_WP:
            // Run Blue Safe Win Point path
            break;
        case AutonRoutine::SKILLS_FULL:
            // Run Full 60s Skills path
            break;
        default:
            // Default fallback action
            break;
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    heading_hold.reset();

    while (true) {
        // Run heading hold update and replace all old arcade/tank drive loops
        heading_hold.update(master, left_mg, right_mg, imu);
        
        pros::delay(10);
    }
}