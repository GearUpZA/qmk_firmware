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

#include "joystick.h"
#include <stdint.h>
#include <stdbool.h>

// Profile state
static uint8_t current_profile = 0;
static uint8_t joystick1_mode[4] = {JOY_MODE_ANALOG, JOY_MODE_DIGITAL, JOY_MODE_ANALOG, JOY_MODE_DIGITAL};
static uint8_t joystick2_mode[4] = {JOY_MODE_ANALOG, JOY_MODE_DIGITAL, JOY_MODE_DIGITAL, JOY_MODE_ANALOG};

// Timing for profile switching
static uint16_t last_profile_switch = 0;
#define PROFILE_SWITCH_DEBOUNCE 300

// Digital mode keystroke state tracking
static bool joy1_digital_state[4] = {false, false, false, false}; // up, down, left, right
static bool joy2_digital_state[4] = {false, false, false, false}; // up, down, left, right

// QMK function declarations (will be available when linked)
uint16_t timer_read(void);
uint16_t timer_elapsed(uint16_t last);
void register_code16(uint16_t keycode);
void unregister_code16(uint16_t keycode);
int16_t joystick_read_axis(uint8_t axis);
void joystick_set_axis(uint8_t axis, int16_t value);

// Forward declarations
void process_joystick_input(uint8_t joystick, int16_t x, int16_t y);
void process_digital_mode(uint8_t joystick, int16_t x, int16_t y);

void analog_init(void) {
    // Initialize timing
    last_profile_switch = timer_read();
}

void analog_task(void) {
    // Handle profile switching - read all 4 axes to determine if both joysticks are pressed
    int16_t x1 = joystick_read_axis(0);  // GP26
    int16_t y1 = joystick_read_axis(1);  // GP27
    int16_t x2 = joystick_read_axis(2);  // GP28
    int16_t y2 = joystick_read_axis(3);  // GP29

    // Check if both joysticks are pressed down (Y values significantly negative)
    bool joy1_pressed = (y1 < -200);  // Threshold for "pressed down"
    bool joy2_pressed = (y2 < -200);  // Threshold for "pressed down"

    if (joy1_pressed && joy2_pressed) {
        if (timer_elapsed(last_profile_switch) > PROFILE_SWITCH_DEBOUNCE) {
            cycle_joystick_profiles();
            last_profile_switch = timer_read();
        }
        return; // Don't process normal joystick input during profile switching
    }

    // Process joystick input based on current profile
    process_joystick_input(0, x1, y1);  // Joystick 1
    process_joystick_input(1, x2, y2);  // Joystick 2
}

void process_joystick_input(uint8_t joystick, int16_t x, int16_t y) {
    if (joystick >= 2) return;  // Only support 2 joysticks

    joystick_mode_t mode = (joystick == 0) ? joystick1_mode[current_profile] : joystick2_mode[current_profile];

    if (mode == JOY_MODE_ANALOG) {
        // Analog mode - use QMK's built-in joystick axes
        // The axes are automatically read by QMK's joystick_task()
        // We just need to ensure they're not overridden
        // QMK will handle sending the analog values to the host

    } else {
        // Digital mode - convert analog to digital hat switch
        process_digital_mode(joystick, x, y);
    }
}

void process_digital_mode(uint8_t joystick, int16_t x, int16_t y) {
    #define DIGITAL_THRESHOLD 200

    // Calculate current digital state
    bool new_state[4] = {
        y < -DIGITAL_THRESHOLD,  // up
        y > DIGITAL_THRESHOLD,   // down
        x < -DIGITAL_THRESHOLD,  // left
        x > DIGITAL_THRESHOLD    // right
    };

    // Get pointer to the state array for this joystick
    bool *old_state = (joystick == 0) ? joy1_digital_state : joy2_digital_state;

    // Define keycodes for each joystick and direction
    uint16_t keycodes[2][4] = {
        {JOY1_UP, JOY1_DOWN, JOY1_LEFT, JOY1_RIGHT},      // Joystick 1
        {JOY2_UP, JOY2_DOWN, JOY2_LEFT, JOY2_RIGHT}       // Joystick 2
    };

    // Process each direction
    for (int i = 0; i < 4; i++) {
        if (new_state[i] != old_state[i]) {
            if (new_state[i]) {
                // Direction activated
                register_code16(keycodes[joystick][i]);
            } else {
                // Direction deactivated
                unregister_code16(keycodes[joystick][i]);
            }
            old_state[i] = new_state[i];
        }
    }

    // Override the analog axes with neutral values in digital mode
    // This prevents analog output when in digital mode
    int axis_base = joystick * 2;  // 0 or 2
    joystick_set_axis(axis_base, 0);     // X axis
    joystick_set_axis(axis_base + 1, 0); // Y axis
}

void set_joystick_mode(uint8_t joystick, joystick_mode_t mode) {
    if (joystick == 0) {
        joystick1_mode[current_profile] = mode;
    } else if (joystick == 1) {
        joystick2_mode[current_profile] = mode;
    }
}

joystick_mode_t get_joystick_mode(uint8_t joystick) {
    if (joystick == 0) {
        return joystick1_mode[current_profile];
    } else if (joystick == 1) {
        return joystick2_mode[current_profile];
    }
    return JOY_MODE_ANALOG;
}

void cycle_joystick_profiles(void) {
    current_profile = (current_profile + 1) % 4;

    // Clear any active digital states when switching profiles
    for (int joy = 0; joy < 2; joy++) {
        bool *state = (joy == 0) ? joy1_digital_state : joy2_digital_state;
        uint16_t keycodes[2][4] = {
            {JOY1_UP, JOY1_DOWN, JOY1_LEFT, JOY1_RIGHT},
            {JOY2_UP, JOY2_DOWN, JOY2_LEFT, JOY2_RIGHT}
        };

        for (int i = 0; i < 4; i++) {
            if (state[i]) {
                unregister_code16(keycodes[joy][i]);
                state[i] = false;
            }
        }
    }
}

uint8_t get_current_profile(void) {
    return current_profile;
}

// Helper function to get profile name
const char* get_profile_name(uint8_t profile) {
    const char* names[4] = {
        "Analog/Analog",
        "Digital/Digital",
        "Analog/Digital",
        "Digital/Analog"
    };
    return (profile < 4) ? names[profile] : "Unknown";
}
