#pragma once

#include <span>

enum class Alliance {
    RED,
    BLUE,
    SKILLS
};

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
    
    // Skills Routines
    SKILLS_FULL,
    SKILLS_HALF,
    SKILLS_SPEEDRUN
};

struct AutonRoutineInfo {
    AutonRoutine routine;
    const char* name;
};

inline constexpr AutonRoutineInfo RED_ROUTINES[] = {
    { AutonRoutine::RED_GOAL_RUSH, "Red: Goal Rush" },
    { AutonRoutine::RED_SAFE_WP,   "Red: Safe Win Point" },
    { AutonRoutine::RED_ELIMS,     "Red: Elims Push" },
    { AutonRoutine::RED_DO_NOTHING,"Red: Do Nothing" }
};

inline constexpr AutonRoutineInfo BLUE_ROUTINES[] = {
    { AutonRoutine::BLUE_GOAL_RUSH, "Blue: Goal Rush" },
    { AutonRoutine::BLUE_SAFE_WP,   "Blue: Safe Win Point" },
    { AutonRoutine::BLUE_ELIMS,     "Blue: Elims Push" },
    { AutonRoutine::BLUE_DO_NOTHING,"Blue: Do Nothing" }
};

inline constexpr AutonRoutineInfo SKILLS_ROUTINES[] = {
    { AutonRoutine::SKILLS_FULL,    "Skills: Full 60s Run" },
    { AutonRoutine::SKILLS_HALF,    "Skills: Half Route" },
    { AutonRoutine::SKILLS_SPEEDRUN,"Skills: Speedrun Route" }
};