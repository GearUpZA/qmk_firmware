#pragma once

#include <stdint.h>
#include <stdbool.h>  // Missing for bool type
#include "quantum.h"  // Missing for QMK functions
#include "drivers/display/ili9341.h"  // Missing display definitions

// UI States
typedef enum {
    UI_STATE_MAIN_MENU,
    UI_STATE_COMBO_EDIT,
    UI_STATE_KEY_SELECT,
    UI_STATE_SAVE_CONFIRM
} ui_state_t;

// UI Elements
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    const char* text;
} button_t;

void init_config_ui(void);
void draw_config_ui(void);
void process_touch_input(uint16_t x, uint16_t y);
void update_config_display(void);
void exit_config_mode(void);  // Missing declaration
bool is_config_mode(void);    // Missing declaration
