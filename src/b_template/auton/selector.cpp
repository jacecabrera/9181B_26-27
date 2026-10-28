#include "b_template/auton/selector.hpp"
#include "b_template/auton/auton_registry.hpp"
#include "liblvgl/lvgl.h"
#include <cstdio>
#include <iterator>

AutonConfig selected_auton;

// Content panels for each tab
static lv_obj_t * panel_red;
static lv_obj_t * panel_blue;
static lv_obj_t * panel_skills;

// Routine display labels for each tab
static lv_obj_t * red_display_lbl;
static lv_obj_t * blue_display_lbl;
static lv_obj_t * skills_display_lbl;

// Bottom status label
static lv_obj_t * status_label;

// Selection indices
static int red_index = 0;
static int blue_index = 0;
static int skills_index = 0;

// Forward declarations
static void update_status_display(void);
static void tab_switch_event_cb(lv_event_t * e);
static void arrow_event_cb(lv_event_t * e);

// Helper to grab currently selected info from the registry
static const AutonRoutineInfo& get_current_info() {
    if (selected_auton.alliance == Alliance::RED) {
        return RED_ROUTINES[red_index];
    } else if (selected_auton.alliance == Alliance::BLUE) {
        return BLUE_ROUTINES[blue_index];
    } else {
        return SKILLS_ROUTINES[skills_index];
    }
}

void auton_selector_init(void) {
    std::printf(">>> AUTON SELECTOR INIT CALLED <<<\n");
    lv_obj_t * scr = lv_screen_active();

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x101010), 0); // Dark grey
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    // 1. Top Tab Switcher Buttons (Red, Blue, Skills)
    lv_obj_t * btn_tab_red = lv_obj_create(scr);
    lv_obj_set_size(btn_tab_red, 140, 32);
    lv_obj_align(btn_tab_red, LV_ALIGN_TOP_LEFT, 15, 8);
    lv_obj_set_style_bg_color(btn_tab_red, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_add_flag(btn_tab_red, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(btn_tab_red, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(btn_tab_red, tab_switch_event_cb, LV_EVENT_CLICKED, (void*)0);
    lv_obj_t * lbl_red = lv_label_create(btn_tab_red);
    lv_label_set_text(lbl_red, "RED");
    lv_obj_center(lbl_red);

    lv_obj_t * btn_tab_blue = lv_obj_create(scr);  
    lv_obj_set_size(btn_tab_blue, 140, 32);
    lv_obj_align(btn_tab_blue, LV_ALIGN_TOP_LEFT, 170, 8);
    lv_obj_set_style_bg_color(btn_tab_blue, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_add_flag(btn_tab_blue, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(btn_tab_blue, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(btn_tab_blue, tab_switch_event_cb, LV_EVENT_CLICKED, (void*)1);
    lv_obj_t * lbl_blue = lv_label_create(btn_tab_blue);
    lv_label_set_text(lbl_blue, "BLUE");
    lv_obj_center(lbl_blue);

    lv_obj_t * btn_tab_skills = lv_obj_create(scr);
    lv_obj_set_size(btn_tab_skills, 140, 32);
    lv_obj_align(btn_tab_skills, LV_ALIGN_TOP_LEFT, 325, 8);
    lv_obj_set_style_bg_color(btn_tab_skills, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_add_flag(btn_tab_skills, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(btn_tab_skills, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(btn_tab_skills, tab_switch_event_cb, LV_EVENT_CLICKED, (void*)2);
    lv_obj_t * lbl_skills = lv_label_create(btn_tab_skills);
    lv_label_set_text(lbl_skills, "SKILLS");
    lv_obj_set_style_text_color(lbl_skills, lv_color_black(), 0);
    lv_obj_center(lbl_skills);

    // 2. Master Content Panels with Tab Background Hues
    panel_red = lv_obj_create(scr);
    lv_obj_set_size(panel_red, 450, 145);
    lv_obj_align(panel_red, LV_ALIGN_TOP_LEFT, 15, 45);
    lv_obj_remove_flag(panel_red, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(panel_red, lv_palette_lighten(LV_PALETTE_RED, 4), 0);
    lv_obj_set_style_bg_opa(panel_red, LV_OPA_30, 0);

    panel_blue = lv_obj_create(scr);
    lv_obj_set_size(panel_blue, 450, 145);
    lv_obj_align(panel_blue, LV_ALIGN_TOP_LEFT, 15, 45);
    lv_obj_remove_flag(panel_blue, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(panel_blue, lv_palette_lighten(LV_PALETTE_BLUE, 4), 0);
    lv_obj_set_style_bg_opa(panel_blue, LV_OPA_30, 0);
    lv_obj_add_flag(panel_blue, LV_OBJ_FLAG_HIDDEN);

    panel_skills = lv_obj_create(scr);
    lv_obj_set_size(panel_skills, 450, 145);
    lv_obj_align(panel_skills, LV_ALIGN_TOP_LEFT, 15, 45);
    lv_obj_remove_flag(panel_skills, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(panel_skills, lv_palette_lighten(LV_PALETTE_YELLOW, 4), 0);
    lv_obj_set_style_bg_opa(panel_skills, LV_OPA_30, 0);
    lv_obj_add_flag(panel_skills, LV_OBJ_FLAG_HIDDEN);

    // ==========================================
    // RED TAB CONTENT
    // ==========================================
    lv_obj_t * red_left_arrow = lv_obj_create(panel_red);
    lv_obj_set_size(red_left_arrow, 60, 90);
    lv_obj_align(red_left_arrow, LV_ALIGN_LEFT_MID, 5, 0);
    lv_obj_set_style_bg_color(red_left_arrow, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_add_flag(red_left_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(red_left_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)-1);
    lv_obj_t * r_l_lbl = lv_label_create(red_left_arrow);
    lv_label_set_text(r_l_lbl, "<");
    lv_obj_center(r_l_lbl);

    lv_obj_t * red_card = lv_obj_create(panel_red);
    lv_obj_set_size(red_card, 290, 90);
    lv_obj_align(red_card, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(red_card, lv_color_white(), 0);
    red_display_lbl = lv_label_create(red_card);
    lv_label_set_text(red_display_lbl, RED_ROUTINES[red_index].name);
    lv_obj_set_style_text_color(red_display_lbl, lv_color_black(), 0);
    lv_obj_center(red_display_lbl);

    lv_obj_t * red_right_arrow = lv_obj_create(panel_red);
    lv_obj_set_size(red_right_arrow, 60, 90);
    lv_obj_align(red_right_arrow, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_style_bg_color(red_right_arrow, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_add_flag(red_right_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(red_right_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)1);
    lv_obj_t * r_r_lbl = lv_label_create(red_right_arrow);
    lv_label_set_text(r_r_lbl, ">");
    lv_obj_center(r_r_lbl);

    // ==========================================
    // BLUE TAB CONTENT
    // ==========================================
    lv_obj_t * blue_left_arrow = lv_obj_create(panel_blue);
    lv_obj_set_size(blue_left_arrow, 60, 90);
    lv_obj_align(blue_left_arrow, LV_ALIGN_LEFT_MID, 5, 0);
    lv_obj_set_style_bg_color(blue_left_arrow, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_add_flag(blue_left_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(blue_left_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)-1);
    lv_obj_t * b_l_lbl = lv_label_create(blue_left_arrow);
    lv_label_set_text(b_l_lbl, "<");
    lv_obj_center(b_l_lbl);

    lv_obj_t * blue_card = lv_obj_create(panel_blue);
    lv_obj_set_size(blue_card, 290, 90);
    lv_obj_align(blue_card, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(blue_card, lv_color_white(), 0);
    blue_display_lbl = lv_label_create(blue_card);
    lv_label_set_text(blue_display_lbl, BLUE_ROUTINES[blue_index].name);
    lv_obj_set_style_text_color(blue_display_lbl, lv_color_black(), 0);
    lv_obj_center(blue_display_lbl);

    lv_obj_t * blue_right_arrow = lv_obj_create(panel_blue);
    lv_obj_set_size(blue_right_arrow, 60, 90);
    lv_obj_align(blue_right_arrow, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_style_bg_color(blue_right_arrow, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_add_flag(blue_right_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(blue_right_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)1);
    lv_obj_t * b_r_lbl = lv_label_create(blue_right_arrow);
    lv_label_set_text(b_r_lbl, ">");
    lv_obj_center(b_r_lbl);

    // ==========================================
    // SKILLS TAB CONTENT
    // ==========================================
    lv_obj_t * skills_left_arrow = lv_obj_create(panel_skills);
    lv_obj_set_size(skills_left_arrow, 60, 90);
    lv_obj_align(skills_left_arrow, LV_ALIGN_LEFT_MID, 5, 0);
    lv_obj_set_style_bg_color(skills_left_arrow, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_add_flag(skills_left_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(skills_left_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)-1);
    lv_obj_t * s_l_lbl = lv_label_create(skills_left_arrow);
    lv_label_set_text(s_l_lbl, "<");
    lv_obj_set_style_text_color(s_l_lbl, lv_color_black(), 0);
    lv_obj_center(s_l_lbl);

    lv_obj_t * skills_card = lv_obj_create(panel_skills);
    lv_obj_set_size(skills_card, 290, 90);
    lv_obj_align(skills_card, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(skills_card, lv_color_white(), 0);
    skills_display_lbl = lv_label_create(skills_card);
    lv_label_set_text(skills_display_lbl, SKILLS_ROUTINES[skills_index].name);
    lv_obj_set_style_text_color(skills_display_lbl, lv_color_black(), 0);
    lv_obj_center(skills_display_lbl);

    lv_obj_t * skills_right_arrow = lv_obj_create(panel_skills);
    lv_obj_set_size(skills_right_arrow, 60, 90);
    lv_obj_align(skills_right_arrow, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_style_bg_color(skills_right_arrow, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_add_flag(skills_right_arrow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(skills_right_arrow, arrow_event_cb, LV_EVENT_CLICKED, (void*)1);
    lv_obj_t * s_r_lbl = lv_label_create(skills_right_arrow);
    lv_label_set_text(s_r_lbl, ">");
    lv_obj_set_style_text_color(s_r_lbl, lv_color_black(), 0);
    lv_obj_center(s_r_lbl);

    // ==========================================
    // 3. BOTTOM STATUS BAR
    // ==========================================
    status_label = lv_label_create(scr);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -4);
    
    // Initialize default selected routine
    selected_auton.alliance = Alliance::RED;
    selected_auton.routine = RED_ROUTINES[red_index].routine;
    update_status_display();
}

static void update_status_display(void) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "Selected Auton: %s", get_current_info().name);
    lv_label_set_text(status_label, buf);
}

static void tab_switch_event_cb(lv_event_t * e) {
    int tab_index = (int)(intptr_t)lv_event_get_user_data(e);

    lv_obj_add_flag(panel_red, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(panel_blue, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(panel_skills, LV_OBJ_FLAG_HIDDEN);

    if (tab_index == 0) {
        lv_obj_remove_flag(panel_red, LV_OBJ_FLAG_HIDDEN);
        selected_auton.alliance = Alliance::RED;
        selected_auton.routine = RED_ROUTINES[red_index].routine;
    } else if (tab_index == 1) {
        lv_obj_remove_flag(panel_blue, LV_OBJ_FLAG_HIDDEN);
        selected_auton.alliance = Alliance::BLUE;
        selected_auton.routine = BLUE_ROUTINES[blue_index].routine;
    } else if (tab_index == 2) {
        lv_obj_remove_flag(panel_skills, LV_OBJ_FLAG_HIDDEN);
        selected_auton.alliance = Alliance::SKILLS;
        selected_auton.routine = SKILLS_ROUTINES[skills_index].routine;
    }
    update_status_display();
}

static void arrow_event_cb(lv_event_t * e) {
    int direction = (int)(intptr_t)lv_event_get_user_data(e);

    if (selected_auton.alliance == Alliance::RED) {
        int total = std::size(RED_ROUTINES);
        red_index = (red_index + direction + total) % total;
        lv_label_set_text(red_display_lbl, RED_ROUTINES[red_index].name);
        selected_auton.routine = RED_ROUTINES[red_index].routine;
    } else if (selected_auton.alliance == Alliance::BLUE) {
        int total = std::size(BLUE_ROUTINES);
        blue_index = (blue_index + direction + total) % total;
        lv_label_set_text(blue_display_lbl, BLUE_ROUTINES[blue_index].name);
        selected_auton.routine = BLUE_ROUTINES[blue_index].routine;
    } else if (selected_auton.alliance == Alliance::SKILLS) {
        int total = std::size(SKILLS_ROUTINES);
        skills_index = (skills_index + direction + total) % total;
        lv_label_set_text(skills_display_lbl, SKILLS_ROUTINES[skills_index].name);
        selected_auton.routine = SKILLS_ROUTINES[skills_index].routine;
    }

    update_status_display();
}