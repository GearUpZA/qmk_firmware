/* Copyright 2025 Custom Keyboards
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

#pragma once

#include "quantum.h"

// Use QMK's built-in combo system
// Combo definitions using QMK's enum system
enum combo_events {
    COMBO_PROFILE_1,
    COMBO_PROFILE_2,
    COMBO_PROFILE_3,
    COMBO_PROFILE_4,
    COMBO_JOY1_TOGGLE,
    COMBO_JOY2_TOGGLE,
    COMBO_CALIBRATE,
    COMBO_RESET_SETTINGS,
    COMBO_LAYER_1,
    COMBO_LAYER_2,
    COMBO_LAYER_3,
    COMBO_EMERGENCY_RESET,
    COMBO_TOUCH_TOGGLE,
    COMBO_DEBUG_MODE,
    COMBO_FACTORY_RESET,
    COMBO_LENGTH
};

// Matrix position macros for combo definitions
#define K00 0,0
#define K01 0,1
#define K02 0,2
#define K03 0,3
#define K10 1,0
#define K11 1,1
#define K12 1,2
#define K13 1,3

// Convert matrix position to keycode for combos
#define KEY_AT(row, col) ((row) * MATRIX_COLS + (col))

// Function declarations for QMK combo integration
void combo_handler_init(void);
void combo_handler_reset(void);

// QMK combo system callbacks (these are called by QMK)
void process_combo_event(uint16_t combo_index, bool pressed);
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo);
bool get_combo_must_tap(uint16_t combo_index, combo_t *combo);
bool get_combo_must_hold(uint16_t combo_index, combo_t *combo);

// Custom action handlers
void combo_switch_profile(uint8_t profile_id);
void combo_toggle_joystick_mode(uint8_t joystick_id);
void combo_start_calibration(void);
void combo_emergency_reset(void);
void combo_factory_reset(void);
void combo_toggle_debug_mode(void);
void combo_switch_layer(uint8_t layer);

// Utility functions
bool combo_handler_is_enabled(void);
void combo_handler_enable(void);
void combo_handler_disable(void);

// External declarations for combo arrays (defined in implementation)
extern const uint16_t PROGMEM combo_profile_1[];
extern const uint16_t PROGMEM combo_profile_2[];
extern const uint16_t PROGMEM combo_profile_3[];
extern const uint16_t PROGMEM combo_profile_4[];
extern const uint16_t PROGMEM combo_joy1_toggle[];
extern const uint16_t PROGMEM combo_joy2_toggle[];
extern const uint16_t PROGMEM combo_calibrate[];
extern const uint16_t PROGMEM combo_reset_settings[];
extern const uint16_t PROGMEM combo_layer_1[];
extern const uint16_t PROGMEM combo_layer_2[];
extern const uint16_t PROGMEM combo_layer_3[];
extern const uint16_t PROGMEM combo_emergency_reset[];
extern const uint16_t PROGMEM combo_touch_toggle[];
extern const uint16_t PROGMEM combo_debug_mode[];
extern const uint16_t PROGMEM combo_factory_reset[];

// QMK combo array declaration (defined in implementation)
extern combo_t key_combos[];

#endif // COMBO_HANDLER_H
