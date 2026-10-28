#pragma once

#include "selector.hpp"

// Main dispatcher called from main.cpp
void run_selected_auton(void);

void run_red_goal_rush(void);
void run_red_safe_wp(void);
void run_red_elims(void);
void run_red_do_nothing(void);

void run_blue_goal_rush(void);
void run_blue_safe_wp(void);
void run_blue_elims(void);
void run_blue_do_nothing(void);

void run_skills_full(void);
void run_skills_half(void);
void run_skills_speedrun(void);