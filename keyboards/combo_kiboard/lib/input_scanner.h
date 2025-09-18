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

// Maximum number of simultaneous inputs to track
#define MAX_TRACKED_INPUTS 16

// Input types
typedef enum {
    INPUT_TYPE_BUTTON,
    INPUT_TYPE_JOYSTICK_DIGITAL,
    INPUT_TYPE_JOYSTICK_ANALOG,
    INPUT_TYPE_TOUCH
} input_type_t;

// Input state structure
typedef struct {
    uint8_t id;                    // Unique input ID
    input_type_t type;             // Type of input
    bool active;                   // Current state
    bool previous;                 // Previous state
    uint32_t press_time;           // Time when pressed
    uint32_t release_time;         // Time when released
    uint16_t duration;             // How long pressed
    bool debounced;                // Has passed debounce time
} input_state_t;

// Button matrix state
typedef struct {
    bool current_state[MATRIX_ROWS][MATRIX_COLS];
    bool previous_state[MATRIX_ROWS][MATRIX_COLS];
    uint32_t press_times[MATRIX_ROWS][MATRIX_COLS];
    uint32_t release_times[MATRIX_ROWS][MATRIX_COLS];
    bool debounce_state[MATRIX_ROWS][MATRIX_COLS];
} button_matrix_t;

// Joystick input state
typedef struct {
    int16_t x_value;
    int16_t y_value;
    int16_t previous_x;
    int16_t previous_y;
    bool digital_left;
    bool digital_right;
    bool digital_up;
    bool digital_down;
    bool previous_digital_left;
    bool previous_digital_right;
    bool previous_digital_up;
    bool previous_digital_down;
    uint32_t last_movement_time;
    bool in_deadzone;
} joystick_state_t;

// Scanner configuration
typedef struct {
    uint16_t scan_interval_ms;     // How often to scan inputs
    uint16_t debounce_time_ms;     // Debounce time for buttons
    uint16_t joystick_debounce_ms; // Debounce time for joystick digital
    bool enable_analog_smoothing;  // Enable smoothing for analog inputs
    uint8_t smoothing_factor;      // Smoothing factor (1-8)
} scanner_config_t;

// Global scanner state
typedef struct {
    button_matrix_t button_matrix;
    joystick_state_t joystick1;
    joystick_state_t joystick2;
    input_state_t tracked_inputs[MAX_TRACKED_INPUTS];
    uint8_t num_tracked_inputs;
    scanner_config_t config;
    uint32_t last_scan_time;
    bool initialized;
} input_scanner_state_t;

// Function declarations
void input_scanner_init(void);
void input_scanner_task(void);
void input_scanner_config_set(const scanner_config_t* config);
void input_scanner_config_get(scanner_config_t* config);

// Button scanning functions
void scan_button_matrix(void);
bool is_button_pressed(uint8_t row, uint8_t col);
bool is_button_just_pressed(uint8_t row, uint8_t col);
bool is_button_just_released(uint8_t row, uint8_t col);
uint32_t get_button_press_duration(uint8_t row, uint8_t col);

// Joystick scanning functions
void scan_joysticks(void);
joystick_state_t* get_joystick_state(uint8_t joystick_id);
bool is_joystick_digital_pressed(uint8_t joystick_id, uint8_t direction);
bool is_joystick_digital_just_pressed(uint8_t joystick_id, uint8_t direction);
int16_t get_joystick_analog_x(uint8_t joystick_id);
int16_t get_joystick_analog_y(uint8_t joystick_id);
bool is_joystick_in_deadzone(uint8_t joystick_id);

// Input tracking functions
uint8_t track_input(input_type_t type, uint8_t id);
void untrack_input(uint8_t tracked_id);
input_state_t* get_tracked_input(uint8_t tracked_id);
void clear_all_tracked_inputs(void);

// Utility functions
void debounce_input(input_state_t* input);
int16_t smooth_analog_value(int16_t current, int16_t previous, uint8_t factor);
bool apply_deadzone(int16_t* x, int16_t* y, uint16_t deadzone);

// Direction constants for joystick digital
#define JOY_DIR_LEFT  0
#define JOY_DIR_RIGHT 1
#define JOY_DIR_UP    2
#define JOY_DIR_DOWN  3

// Default configuration values
#define DEFAULT_SCAN_INTERVAL_MS      5
#define DEFAULT_DEBOUNCE_TIME_MS      5
#define DEFAULT_JOYSTICK_DEBOUNCE_MS  10
#define DEFAULT_SMOOTHING_FACTOR      4