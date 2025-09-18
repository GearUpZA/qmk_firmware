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

#include "input_scanner.h"
#include "analog.h"

// Global scanner state
static input_scanner_state_t scanner_state = {0};

void input_scanner_init(void) {
    // Initialize configuration with defaults
    scanner_state.config.scan_interval_ms = DEFAULT_SCAN_INTERVAL_MS;
    scanner_state.config.debounce_time_ms = DEFAULT_DEBOUNCE_TIME_MS;
    scanner_state.config.joystick_debounce_ms = DEFAULT_JOYSTICK_DEBOUNCE_MS;
    scanner_state.config.enable_analog_smoothing = true;
    scanner_state.config.smoothing_factor = DEFAULT_SMOOTHING_FACTOR;
    
    // Clear all states
    memset(&scanner_state.button_matrix, 0, sizeof(button_matrix_t));
    memset(&scanner_state.joystick1, 0, sizeof(joystick_state_t));
    memset(&scanner_state.joystick2, 0, sizeof(joystick_state_t));
    memset(scanner_state.tracked_inputs, 0, sizeof(scanner_state.tracked_inputs));
    
    scanner_state.num_tracked_inputs = 0;
    scanner_state.last_scan_time = timer_read32();
    scanner_state.initialized = true;
}

void input_scanner_task(void) {
    if (!scanner_state.initialized) {
        return;
    }
    
    uint32_t current_time = timer_read32();
    
    // Check if it's time to scan
    if (timer_elapsed32(scanner_state.last_scan_time) >= scanner_state.config.scan_interval_ms) {
        scan_button_matrix();
        scan_joysticks();
        
        // Update tracked inputs
        for (uint8_t i = 0; i < scanner_state.num_tracked_inputs; i++) {
            debounce_input(&scanner_state.tracked_inputs[i]);
        }
        
        scanner_state.last_scan_time = current_time;
    }
}

void scan_button_matrix(void) {
    uint32_t current_time = timer_read32();
    
    // Save previous state
    memcpy(scanner_state.button_matrix.previous_state, 
           scanner_state.button_matrix.current_state, 
           sizeof(scanner_state.button_matrix.current_state));
    
    // Scan all buttons
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            // Read button state (assuming direct pins)
            bool current_state = !readPin(direct_pins[row][col]);
            bool previous_state = scanner_state.button_matrix.previous_state[row][col];
            
            scanner_state.button_matrix.current_state[row][col] = current_state;
            
            // Handle state changes
            if (current_state && !previous_state) {
                // Button just pressed
                scanner_state.button_matrix.press_times[row][col] = current_time;
                scanner_state.button_matrix.debounce_state[row][col] = false;
            } else if (!current_state && previous_state) {
                // Button just released
                scanner_state.button_matrix.release_times[row][col] = current_time;
            }
            
            // Handle debouncing
            if (current_state && !scanner_state.button_matrix.debounce_state[row][col]) {
                uint32_t press_duration = current_time - scanner_state.button_matrix.press_times[row][col];
                if (press_duration >= scanner_state.config.debounce_time_ms) {
                    scanner_state.button_matrix.debounce_state[row][col] = true;
                }
            }
        }
    }
}

void scan_joysticks(void) {
    uint32_t current_time = timer_read32();
    
    // Scan joystick 1
    scan_single_joystick(&scanner_state.joystick1, 1, current_time);
    
    // Scan joystick 2
    scan_single_joystick(&scanner_state.joystick2, 2, current_time);
}

static void scan_single_joystick(joystick_state_t* joy, uint8_t joystick_id, uint32_t current_time) {
    // Save previous values
    joy->previous_x = joy->x_value;
    joy->previous_y = joy->y_value;
    joy->previous_digital_left = joy->digital_left;
    joy->previous_digital_right = joy->digital_right;
    joy->previous_digital_up = joy->digital_up;
    joy->previous_digital_down = joy->digital_down;
    
    // Read analog values
    pin_t x_pin = (joystick_id == 1) ? ANALOG_JOYSTICK_X_AXIS_PIN : ANALOG_JOYSTICK2_X_AXIS_PIN;
    pin_t y_pin = (joystick_id == 1) ? ANALOG_JOYSTICK_Y_AXIS_PIN : ANALOG_JOYSTICK2_Y_AXIS_PIN;
    
    int16_t raw_x = analogReadPin(x_pin) - 512;  // Center around 0
    int16_t raw_y = analogReadPin(y_pin) - 512;
    
    // Apply smoothing if enabled
    if (scanner_state.config.enable_analog_smoothing) {
        joy->x_value = smooth_analog_value(raw_x, joy->previous_x, scanner_state.config.smoothing_factor);
        joy->y_value = smooth_analog_value(raw_y, joy->previous_y, scanner_state.config.smoothing_factor);
    } else {
        joy->x_value = raw_x;
        joy->y_value = raw_y;
    }
    
    // Apply deadzone
    joy->in_deadzone = apply_deadzone(&joy->x_value, &joy->y_value, ANALOG_JOYSTICK_DEADZONE);
    
    // Read digital inputs
    pin_t left_pin = (joystick_id == 1) ? JOYSTICK1_DIGITAL_X_PIN : JOYSTICK2_DIGITAL_X_PIN;
    pin_t right_pin = left_pin;  // Same pin, different logic
    pin_t up_pin = (joystick_id == 1) ? JOYSTICK1_DIGITAL_Y_PIN : JOYSTICK2_DIGITAL_Y_PIN;
    pin_t down_pin = up_pin;     // Same pin, different logic
    
    // Convert analog to digital directions
    joy->digital_left = (joy->x_value < -ANALOG_JOYSTICK_DEADZONE);
    joy->digital_right = (joy->x_value > ANALOG_JOYSTICK_DEADZONE);
    joy->digital_up = (joy->y_value > ANALOG_JOYSTICK_DEADZONE);
    joy->digital_down = (joy->y_value < -ANALOG_JOYSTICK_DEADZONE);
    
    // Update movement time
    if (joy->x_value != joy->previous_x || joy->y_value != joy->previous_y) {
        joy->last_movement_time = current_time;
    }
}

bool is_button_pressed(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return false;
    return scanner_state.button_matrix.current_state[row][col] && 
           scanner_state.button_matrix.debounce_state[row][col];
}

bool is_button_just_pressed(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return false;
    return scanner_state.button_matrix.current_state[row][col] && 
           !scanner_state.button_matrix.previous_state[row][col] &&
           scanner_state.button_matrix.debounce_state[row][col];
}

bool is_button_just_released(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return false;
    return !scanner_state.button_matrix.current_state[row][col] && 
           scanner_state.button_matrix.previous_state[row][col];
}

uint32_t get_button_press_duration(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return 0;
    if (!scanner_state.button_matrix.current_state[row][col]) return 0;
    
    return timer_elapsed32(scanner_state.button_matrix.press_times[row][col]);
}

joystick_state_t* get_joystick_state(uint8_t joystick_id) {
    if (joystick_id == 1) return &scanner_state.joystick1;
    if (joystick_id == 2) return &scanner_state.joystick2;
    return NULL;
}

bool is_joystick_digital_pressed(uint8_t joystick_id, uint8_t direction) {
    joystick_state_t* joy = get_joystick_state(joystick_id);
    if (!joy) return false;
    
    switch (direction) {
        case JOY_DIR_LEFT:  return joy->digital_left;
        case JOY_DIR_RIGHT: return joy->digital_right;
        case JOY_DIR_UP:    return joy->digital_up;
        case JOY_DIR_DOWN:  return joy->digital_down;
        default: return false;
    }
}

bool is_joystick_digital_just_pressed(uint8_t joystick_id, uint8_t direction) {
    joystick_state_t* joy = get_joystick_state(joystick_id);
    if (!joy) return false;
    
    bool current, previous;
    switch (direction) {
        case JOY_DIR_LEFT:
            current = joy->digital_left;
            previous = joy->previous_digital_left;
            break;
        case JOY_DIR_RIGHT:
            current = joy->digital_right;
            previous = joy->previous_digital_right;
            break;
        case JOY_DIR_UP:
            current = joy->digital_up;
            previous = joy->previous_digital_up;
            break;
        case JOY_DIR_DOWN:
            current = joy->digital_down;
            previous = joy->previous_digital_down;
            break;
        default:
            return false;
    }
    
    return current && !previous;
}

int16_t get_joystick_analog_x(uint8_t joystick_id) {
    joystick_state_t* joy = get_joystick_state(joystick_id);
    return joy ? joy->x_value : 0;
}

int16_t get_joystick_analog_y(uint8_t joystick_id) {
    joystick_state_t* joy = get_joystick_state(joystick_id);
    return joy ? joy->y_value : 0;
}

bool is_joystick_in_deadzone(uint8_t joystick_id) {
    joystick_state_t* joy = get_joystick_state(joystick_id);
    return joy ? joy->in_deadzone : true;
}

uint8_t track_input(input_type_t type, uint8_t id) {
    if (scanner_state.num_tracked_inputs >= MAX_TRACKED_INPUTS) {
        return 255; // Invalid ID
    }
    
    uint8_t tracked_id = scanner_state.num_tracked_inputs++;
    input_state_t* input = &scanner_state.tracked_inputs[tracked_id];
    
    input->id = id;
    input->type = type;
    input->active = false;
    input->previous = false;
    input->press_time = 0;
    input->release_time = 0;
    input->duration = 0;
    input->debounced = false;
    
    return tracked_id;
}

void untrack_input(uint8_t tracked_id) {
    if (tracked_id >= scanner_state.num_tracked_inputs) return;
    
    // Shift remaining inputs down
    for (uint8_t i = tracked_id; i < scanner_state.num_tracked_inputs - 1; i++) {
        scanner_state.tracked_inputs[i] = scanner_state.tracked_inputs[i + 1];
    }
    
    scanner_state.num_tracked_inputs--;
}

input_state_t* get_tracked_input(uint8_t tracked_id) {
    if (tracked_id >= scanner_state.num_tracked_inputs) return NULL;
    return &scanner_state.tracked_inputs[tracked_id];
}

void clear_all_tracked_inputs(void) {
    scanner_state.num_tracked_inputs = 0;
    memset(scanner_state.tracked_inputs, 0, sizeof(scanner_state.tracked_inputs));
}

void debounce_input(input_state_t* input) {
    if (!input) return;
    
    uint32_t current_time = timer_read32();
    
    // Update previous state
    input->previous = input->active;
    
    // Check current state based on input type
    switch (input->type) {
        case INPUT_TYPE_BUTTON:
            input->active = is_button_pressed(input->id / MATRIX_COLS, input->id % MATRIX_COLS);
            break;
        case INPUT_TYPE_JOYSTICK_DIGITAL:
            // Implement joystick digital tracking
            break;
        default:
            break;
    }
    
    // Handle state changes
    if (input->active && !input->previous) {
        input->press_time = current_time;
        input->debounced = false;
    } else if (!input->active && input->previous) {
        input->release_time = current_time;
        input->duration = input->release_time - input->press_time;
    }
    
    // Handle debouncing
    if (input->active && !input->debounced) {
        if (timer_elapsed32(input->press_time) >= scanner_state.config.debounce_time_ms) {
            input->debounced = true;
        }
    }
}

int16_t smooth_analog_value(int16_t current, int16_t previous, uint8_t factor) {
    if (factor == 0 || factor > 8) return current;
    
    // Simple exponential moving average
    return (previous * (factor - 1) + current) / factor;
}

bool apply_deadzone(int16_t* x, int16_t* y, uint16_t deadzone) {
    int32_t magnitude_squared = (*x) * (*x) + (*y) * (*y);
    int32_t deadzone_squared = deadzone * deadzone;
    
    if (magnitude_squared <= deadzone_squared) {
        *x = 0;
        *y = 0;
        return true;  // In deadzone
    }
    
    return false;  // Outside deadzone
}

void input_scanner_config_set(const scanner_config_t* config) {
    if (config) {
        scanner_state.config = *config;
    }
}

void input_scanner_config_get(scanner_config_t* config) {
    if (config) {
        *config = scanner_state.config;
    }
}

// Static function declaration (add to top of file)
static void scan_single_joystick(joystick_state_t* joy, uint8_t joystick_id, uint32_t current_time);

// Missing direct_pins definition (add after includes)
static const pin_t direct_pins[MATRIX_ROWS][MATRIX_COLS] = {
    { GP0, GP1, GP2, GP3 },
    { GP4, GP5, GP6, GP7 }
};