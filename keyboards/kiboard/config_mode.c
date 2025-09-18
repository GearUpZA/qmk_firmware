/* Copyright 2025 GearUpZA <you@example.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config_mode.h"
#include "quantum.h"
#include "action_layer.h"
#include <stdint.h>
#include <stdbool.h>

// QMK function declarations
uint8_t eeprom_read_byte(const uint8_t *addr);
void eeprom_update_byte(uint8_t *addr, uint8_t value);
uint32_t timer_read32(void);
uint32_t timer_elapsed32(uint32_t last);

// Layer definitions (must match keymap.c)
enum layers {
    _BASE = 0,
    _DIGITAL,
    _GAMING,
    _CONFIG
};

// Config mode flag in EEPROM
#define CONFIG_MODE_ADDR 100
#define CONFIG_MODE_MAGIC 0x42

// Display state management
static display_mode_t current_display_mode = DISPLAY_MODE_STATUS;
static uint32_t last_display_update = 0;
static uint32_t display_timeout = 30000; // 30 seconds before dimming
static bool display_initialized = false;

// Profile information (updated for layer-based system)
static const profile_info_t profile_configs[4] = {
    {"Base Mode", "Analog", "Mouse", 0},
    {"Digital Mode", "WASD", "Arrows", 1},
    {"Gaming Mode", "Gaming", "Hotkeys", 2},
    {"Config Mode", "Config", "Setup", 3}
};

// Helper function to get profile name (prevents unused variable warning)
static const char* get_profile_name(uint8_t profile_id) {
    if (profile_id < 4) {
        return profile_configs[profile_id].name;
    }
    return "Unknown";
}

bool is_config_mode(void) {
    return eeprom_read_byte((uint8_t*)CONFIG_MODE_ADDR) == CONFIG_MODE_MAGIC;
}

void enter_config_mode(void) {
    eeprom_update_byte((uint8_t*)CONFIG_MODE_ADDR, CONFIG_MODE_MAGIC);
}

void exit_config_mode(void) {
    eeprom_update_byte((uint8_t*)CONFIG_MODE_ADDR, 0x00);
}

void init_config_mode(void) {
    // Initialize display for persistent operation
    init_persistent_display();
}

void init_persistent_display(void) {
    // Initialize display hardware
    // This should be called during keyboard initialization
    display_initialized = true;
    current_display_mode = DISPLAY_MODE_STATUS;
    last_display_update = timer_read32();

    // Show initial status
    show_profile_status();
}

void update_display_mode(display_mode_t mode) {
    if (current_display_mode != mode) {
        current_display_mode = mode;
        last_display_update = timer_read32();

        switch (mode) {
            case DISPLAY_MODE_STATUS:
                show_profile_status();
                break;
            case DISPLAY_MODE_PROFILE_SELECT:
                show_profile_selector();
                break;
            case DISPLAY_MODE_CONFIG:
                // Enter full configuration mode
                enter_config_mode();
                break;
            case DISPLAY_MODE_OFF:
                // Turn off display for power saving
                break;
        }
    }
}

display_mode_t get_display_mode(void) {
    return current_display_mode;
}

void show_profile_status(void) {
    if (!display_initialized) return;

    // Get current active layer (simplified - just use default)
    uint8_t current_layer = 0; // Default to BASE layer

    // Ensure layer index is valid
    if (current_layer >= 4) current_layer = 0;

    // Use current_layer for profile info
    const char* profile_name = get_profile_name(current_layer);
    (void)profile_name; // Suppress unused variable warning

    // Clear display and show profile info
    // display_clear();

    // Title
    // display_text(10, 10, "KiBoard - Active Profile", COLOR_WHITE);

    // Current profile name
    // display_text(10, 40, profile_configs[current_layer].name, COLOR_CYAN);

    // Layer modes
    // char status_line[64];
    // snprintf(status_line, sizeof(status_line), "Left: %s", profile_configs[current_layer].joy1_mode);
    // display_text(10, 70, status_line, COLOR_GREEN);

    // snprintf(status_line, sizeof(status_line), "Right: %s", profile_configs[current_layer].joy2_mode);
    // display_text(10, 100, status_line, COLOR_GREEN);

    // Instructions
    // display_text(10, 140, "Touch screen to change profile", COLOR_YELLOW);
    // display_text(10, 170, "Hold A+B+C+D for full config", COLOR_GRAY);

    // Placeholder display update call
    // display_update();
}

void show_profile_selector(void) {
    if (!display_initialized) return;

    // Clear display
    // display_clear();

    // Title
    // display_text(10, 10, "Select Profile", COLOR_WHITE);

    // Profile options
    for (int i = 0; i < 4; i++) {
        uint16_t y_pos = TOUCH_ZONE_PROFILE_1_Y + (i * 50);

        // For simplicity, assume current layer 0 is active
        // In real implementation, you'd check the actual layer state
        bool is_current = (i == 0);

        // Use variables (prevents unused variable warnings)
        (void)y_pos;
        (void)is_current;

        // Draw profile button
        // uint16_t color = is_current ? COLOR_GREEN : COLOR_WHITE;
        // display_rectangle(10, y_pos, TOUCH_ZONE_WIDTH, TOUCH_ZONE_HEIGHT, color);
        // display_text(20, y_pos + 10, profile_configs[i].name, COLOR_BLACK);

        // Show layer modes
        // char mode_text[32];
        // snprintf(mode_text, sizeof(mode_text), "%s | %s",
        //          profile_configs[i].joy1_mode, profile_configs[i].joy2_mode);
        // display_text(20, y_pos + 25, mode_text, COLOR_BLACK);
    }

    // Configuration button
    // display_rectangle(TOUCH_ZONE_CONFIG_X, TOUCH_ZONE_CONFIG_Y,
    //                   TOUCH_ZONE_CONFIG_W, TOUCH_ZONE_CONFIG_H, COLOR_RED);
    // display_text(TOUCH_ZONE_CONFIG_X + 10, TOUCH_ZONE_CONFIG_Y + 10, "Config", COLOR_WHITE);

    // display_update();
}

void handle_display_touch(uint16_t x, uint16_t y) {
    last_display_update = timer_read32();

    switch (current_display_mode) {
        case DISPLAY_MODE_STATUS:
            // Any touch in status mode goes to profile selector
            update_display_mode(DISPLAY_MODE_PROFILE_SELECT);
            break;

        case DISPLAY_MODE_PROFILE_SELECT:
            // Check if touch is in profile selection area
            if (x >= 10 && x <= (10 + TOUCH_ZONE_WIDTH)) {
                // Determine which profile was touched
                if (y >= TOUCH_ZONE_PROFILE_1_Y && y <= (TOUCH_ZONE_PROFILE_1_Y + TOUCH_ZONE_HEIGHT)) {
                    // Profile 0: Base Layer
                    // TODO: Switch to base layer when QMK layer functions are available
                    update_display_mode(DISPLAY_MODE_STATUS);
                }
                else if (y >= TOUCH_ZONE_PROFILE_2_Y && y <= (TOUCH_ZONE_PROFILE_2_Y + TOUCH_ZONE_HEIGHT)) {
                    // Profile 1: Digital Layer
                    // TODO: Switch to digital layer when QMK layer functions are available
                    update_display_mode(DISPLAY_MODE_STATUS);
                }
                else if (y >= TOUCH_ZONE_PROFILE_3_Y && y <= (TOUCH_ZONE_PROFILE_3_Y + TOUCH_ZONE_HEIGHT)) {
                    // Profile 2: Gaming Layer
                    // TODO: Switch to gaming layer when QMK layer functions are available
                    update_display_mode(DISPLAY_MODE_STATUS);
                }
                else if (y >= TOUCH_ZONE_PROFILE_4_Y && y <= (TOUCH_ZONE_PROFILE_4_Y + TOUCH_ZONE_HEIGHT)) {
                    // Profile 3: Config Layer
                    // TODO: Switch to config layer when QMK layer functions are available
                    update_display_mode(DISPLAY_MODE_STATUS);
                }
            }
            // Check if touch is in config button area
            else if (x >= TOUCH_ZONE_CONFIG_X && x <= (TOUCH_ZONE_CONFIG_X + TOUCH_ZONE_CONFIG_W) &&
                     y >= TOUCH_ZONE_CONFIG_Y && y <= (TOUCH_ZONE_CONFIG_Y + TOUCH_ZONE_CONFIG_H)) {
                // Enter full configuration mode
                update_display_mode(DISPLAY_MODE_CONFIG);
            }
            else {
                // Touch outside selection areas - return to status
                update_display_mode(DISPLAY_MODE_STATUS);
            }
            break;

        case DISPLAY_MODE_CONFIG:
            // Handle full configuration mode touches
            // This would be handled by the existing config interface
            break;

        case DISPLAY_MODE_OFF:
            // Any touch wakes up the display
            update_display_mode(DISPLAY_MODE_STATUS);
            break;
    }
}

void update_profile_display(void) {
    // Check for display timeout
    if (timer_elapsed32(last_display_update) > display_timeout) {
        if (current_display_mode != DISPLAY_MODE_OFF) {
            update_display_mode(DISPLAY_MODE_OFF);
        }
        return;
    }

    // Update current display mode if needed
    switch (current_display_mode) {
        case DISPLAY_MODE_STATUS:
            // Refresh status display periodically
            if (timer_elapsed32(last_display_update) > 5000) { // Every 5 seconds
                show_profile_status();
                last_display_update = timer_read32();
            }
            break;
        case DISPLAY_MODE_PROFILE_SELECT:
        case DISPLAY_MODE_CONFIG:
        case DISPLAY_MODE_OFF:
            // These modes don't need periodic updates
            break;
    }
}

void cycle_display_mode(void) {
    switch (current_display_mode) {
        case DISPLAY_MODE_OFF:
            update_display_mode(DISPLAY_MODE_STATUS);
            break;
        case DISPLAY_MODE_STATUS:
            update_display_mode(DISPLAY_MODE_PROFILE_SELECT);
            break;
        case DISPLAY_MODE_PROFILE_SELECT:
            update_display_mode(DISPLAY_MODE_STATUS);
            break;
        case DISPLAY_MODE_CONFIG:
            update_display_mode(DISPLAY_MODE_STATUS);
            break;
    }
}
