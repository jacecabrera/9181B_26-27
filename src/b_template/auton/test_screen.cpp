#include "lvgl/lvgl.h"
#include <cstdio>

// Button click event callback to test input handling & UI updates
static void test_btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        static int click_count = 0;
        click_count++;

        // Update button label text dynamically
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Clicked: %d", click_count);
        
        std::printf("[Simulator] Test button clicked %d times\n", click_count);
    }
}

void load_test_screen(void) {
    // Get the default active screen (automatically sized to your 480x240 V5 window configuration)
    lv_obj_t * scr = lv_disp_get_scr_act(NULL);

    // 1. Add a title label
    lv_obj_t * title_label = lv_label_create(scr);
    lv_label_set_text(title_label, "lvgl brain sim test");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 15);

    // 2. Add an interactive button to test mouse/touch event mapping
    lv_obj_t * test_btn = lv_btn_create(scr);
    lv_obj_set_size(test_btn, 180, 50);
    lv_obj_align(test_btn, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(test_btn, test_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 3. Add label inside the button
    lv_obj_t * btn_label = lv_label_create(test_btn);
    lv_label_set_text(btn_label, "Click Me!");
    lv_obj_center(btn_label);
}