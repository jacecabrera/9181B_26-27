#include "main.h"

#include "b_template/auton/selector.hpp"
#include "b_template/auton/autons.hpp"

#include "globals.hpp"
#include "b_template/subsystem/lift.hpp"
#include "b_template/chassis/heading_hold.hpp"
#include "b_template/subsystem/claw.hpp"
#include <cstdlib>
#include <algorithm>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    /** 
    auton_selector_init();

    left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // Blocks until calibration finishes (about 2 s). Returns PROS_ERR on failure.
    if (imu.reset(true) == PROS_ERR) {
        pros::lcd::set_text(2, "IMU FAILED, check IMU port and restart");
    }
    */
    auton_selector_init();               // first, so the screen isn't blank for 2 s
    left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //claw.init();
    //lift.init();                         // robot must start with the lift fully down
    if (imu.reset(true) == PROS_ERR) {   // blocks ~2 s, keep the robot still
        master.rumble("---");
        master.set_text(0, 0, "IMU FAILED");
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}
//* */
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
        left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    run_selected_auton();

    /** 
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
    */
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
    left_mg.tare_position_all();

            left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    while (true) {
        std::printf("left motor0 pos: %.1f\n", left_mg.get_position());
        heading_hold.update(master, left_mg, right_mg, imu);

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            claw.spin(127);
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            claw.spin(-127);
        } else {
            claw.stop();
        }

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            lift.move(127);  // Move up
        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            lift.move(-127); // Move down
        } 
        else {
            lift.stop();     // Brake hold locks position
        }
        // no lift.update(): the lift task runs it

        pros::delay(10);
    }
}