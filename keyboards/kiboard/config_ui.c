#include "config_ui.h"
#include "drivers/display/ili9341.h"
#include "eeprom.h"

static ui_state_t current_state = UI_STATE_MAIN_MENU;
static uint8_t selected_combo = 0;

// Define UI elements
static const button_t main_menu_buttons[] = {
    {10, 10, 220, 40, "Edit Combos"},
    {10, 60, 220, 40, "Save Config"},
    {10, 110, 220, 40, "Exit Config"}
};

void init_config_ui(void) {
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, COLOR_BLACK);
    draw_config_ui();
}

void draw_config_ui(void) {
    switch (current_state) {
        case UI_STATE_MAIN_MENU:
            for (int i = 0; i < 3; i++) {
                const button_t* btn = &main_menu_buttons[i];
                ili9341_fill_rect(btn->x, btn->y, btn->width, btn->height, COLOR_BLUE);
                ili9341_draw_string(btn->x + 5, btn->y + 15, btn->text, COLOR_WHITE);
            }
            break;

        case UI_STATE_COMBO_EDIT:
            // Draw combo editing interface
            ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, COLOR_BLACK);
            ili9341_draw_string(10, 10, "Select Combo to Edit:", COLOR_WHITE);
            // Draw combo list
            break;

        // ... Add other UI states ...
    }
}

void process_touch_input(uint16_t x, uint16_t y) {
    switch (current_state) {
        case UI_STATE_MAIN_MENU:
            for (int i = 0; i < 3; i++) {
                const button_t* btn = &main_menu_buttons[i];
                if (x >= btn->x && x < btn->x + btn->width &&
                    y >= btn->y && y < btn->y + btn->height) {
                    switch (i) {
                        case 0:
                            current_state = UI_STATE_COMBO_EDIT;
                            break;
                        case 1:
                            current_state = UI_STATE_SAVE_CONFIRM;
                            break;
                        case 2:
                            exit_config_mode();
                            break;
                    }
                    draw_config_ui();
                    break;
                }
            }
            break;

        // ... Add other state handlers ...
    }
}

void update_config_display(void) {
    // Handle any animated elements or status updates
    draw_config_ui();
}

// Missing state transition validation
bool validate_state_transition(ui_state_t new_state) {
    switch (current_state) {
        case UI_STATE_MAIN_MENU:
            return (new_state == UI_STATE_COMBO_EDIT ||
                   new_state == UI_STATE_SAVE_CONFIRM);
        // Add other state validations
        default:
            return false;
    }
}
