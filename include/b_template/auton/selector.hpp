#pragma once

#include "auton_registry.hpp" // Pull in Alliance and AutonRoutine from the registry

struct AutonConfig {
    Alliance alliance;
    AutonRoutine routine;
};

extern AutonConfig selected_auton;

void auton_selector_init(void);