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
#include "lib/input_scanner.h"
#include "lib/combo_handler.h"
#include "lib/profile_manager.h"
#include "lib/touch_interface.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    k00, k01, k02, k03, \
    k10, k11, k12, k13  \
) { \
    { k00, k01, k02, k03 }, \
    { k10, k11, k12, k13 }  \
}

// Joystick configuration structure
typedef struct {
    bool analog_mode;
    uint16_t x_center;
    uint16_t y_center;
    uint16_t deadzone;
    uint8_t sensitivity;
} joystick_config_t;

// Global state structure
typedef struct {
    joystick_config_t joystick1;
    joystick_config_t joystick2;
    bool touchscreen_enabled;
    bool combo_enabled;
    uint8_t current_profile;
    bool debug_mode;
} kiboard_state_t;

// Global state variable
extern kiboard_state_t kiboard_state;

// Function declarations
void kiboard_init(void);
void kiboard_task(void);
bool process_record_kiboard(uint16_t keycode, keyrecord_t *record);
void joystick_init(void);
void joystick_task(void);
void joystick_calibrate(uint8_t joystick_id);
void joystick_toggle_mode(uint8_t joystick_id);
uint16_t read_joystick_x(uint8_t joystick_id);
uint16_t read_joystick_y(uint8_t joystick_id);
void touchscreen_init(void);
void touchscreen_task(void);
bool touchscreen_calibrate(void);
void factory_reset(void);
void emergency_reset(void);

// Hardware abstraction functions
void set_pin_input_pullup(pin_t pin);
void set_pin_output(pin_t pin);
bool read_pin(pin_t pin);
void write_pin(pin_t pin, bool value);
uint16_t read_adc(pin_t pin);

// Utility functions
void kiboard_debug_print(const char* message);
void kiboard_error_handler(const char* error);