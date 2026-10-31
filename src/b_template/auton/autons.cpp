#include "b_template/auton/autons.hpp"
#include "b_template/auton/auton_registry.hpp"
#include "main.h"
#include "globals.hpp"
#include "config.hpp"

namespace {
using namespace b_template;

utils::PIDF drive_pid() { return utils::PIDF(config::LATERAL_kP, config::LATERAL_kI, config::LATERAL_kD, 0.0, 0.0, 200.0, 2000.0); }
utils::PIDF turn_pid()  { return utils::PIDF(config::ANGULAR_kP, config::ANGULAR_kI, config::ANGULAR_kD, 0.0, 0.0, 10.0, 2000.0); }

void wait_for_lift(std::uint32_t timeout_ms = 1500) {
    std::uint32_t t0 = pros::millis();
    while (!lift.at_target() && pros::millis() - t0 < timeout_ms) pros::delay(10);
}
void run_red_elims(void) {
}

void run_red_do_nothing(void) {
}

// --- Blue Routine Implementations ---
void run_blue_goal_rush(void) {
}

void run_blue_safe_wp(void) {
}

void run_blue_elims(void) {
}

void run_blue_do_nothing(void) {
}

void run_skills_full(void) {
}

void run_skills_half(void) {
}

void run_skills_speedrun(void) {
}

//main selector
void run_selected_auton(void) {
    switch (selected_auton.routine) {
        // Red cases
        case AutonRoutine::RED_GOAL_RUSH:
            run_red_goal_rush();
            break;
        case AutonRoutine::RED_SAFE_WP:
            run_red_safe_wp();
            break;
        case AutonRoutine::RED_ELIMS:
            run_red_elims();
            break;
        case AutonRoutine::RED_DO_NOTHING:
            run_red_do_nothing();
            break;

        // Blue cases
        case AutonRoutine::BLUE_GOAL_RUSH:
            run_blue_goal_rush();
            break;
        case AutonRoutine::BLUE_SAFE_WP:
            run_blue_safe_wp();
            break;
        case AutonRoutine::BLUE_ELIMS:
            run_blue_elims();
            break;
        case AutonRoutine::BLUE_DO_NOTHING:
            run_blue_do_nothing();
            break;

        // Skills cases
        case AutonRoutine::SKILLS_FULL:
            run_skills_full();
            break;
        case AutonRoutine::SKILLS_HALF:
            run_skills_half();
            break;
        case AutonRoutine::SKILLS_SPEEDRUN:
            run_skills_speedrun();
            break;

        default:
            // Fallback safety action if an unassigned state happens
            break;
    }
}