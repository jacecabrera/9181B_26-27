/**
 * @file lv_conf.h
 * Configuration file for LVGL v8 (Simulator)
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*=========================
 * COLOR SETTINGS
 *================         */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

/*=========================
 * MEMORY SETTINGS
 *=========================*/
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (64U * 1024U)   /* 64KB RAM for simulator */

/*=========================
 * HAL SETTINGS
 *=========================*/
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "SDL2/SDL.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (SDL_GetTicks())

/*=========================
 * FEATURE USAGE
 *=========================*/
#define LV_USE_ANIMATION 1
#define LV_USE_SHADOW 1
#define LV_USE_BLEND_MODES 1
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BTN 1
#define LV_USE_BTNMATRIX 1
#define LV_USE_CHECKBOX 1
#define LV_USE_DROPDOWN 1
#define LV_USE_IMAGE 1
#define LV_USE_LABEL 1
#define LV_USE_LINE 1
#define LV_USE_ROLLER 1
#define LV_USE_SLIDER 1
#define LV_USE_SWITCH 1
#define LV_USE_TEXTAREA 1
#define LV_USE_TABLE 1

#endif /*LV_CONF_H*/
