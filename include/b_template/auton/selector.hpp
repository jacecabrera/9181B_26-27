#pragma once

#include <string>

enum class Alliance { RED, BLUE, SKILLS };

enum class AutonRoutine {
    // Red Routines
    RED_GOAL_RUSH,
    RED_SAFE_WP,
    RED_ELIMS,
    RED_DO_NOTHING,
    // Blue Routines
    BLUE_GOAL_RUSH,
    BLUE_SAFE_WP,
    BLUE_ELIMS,
    BLUE_DO_NOTHING,
    // Skills Routes
    SKILLS_FULL,
    SKILLS_HALF,
    SKILLS_SPEEDRUN,
    NONE
};

struct AutonConfig {
    Alliance alliance = Alliance::RED;
    AutonRoutine routine = AutonRoutine::RED_GOAL_RUSH;
};

extern AutonConfig selected_auton;

void auton_selector_init(void);