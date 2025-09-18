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

#pragma once

#include <stdint.h>
#include <stdbool.h>

// Joystick mode enumeration
typedef enum {
    JOY_MODE_ANALOG,    // Analog output for camera/mouse control
    JOY_MODE_DIGITAL    // Digital hat switch for button presses
} joystick_mode_t;

// Custom keycodes for joystick directions
enum joystick_keycodes {
    JOY1_UP = 0x7E00,    // Use custom range to avoid conflicts
    JOY1_DOWN,
    JOY1_LEFT,
    JOY1_RIGHT,
    JOY2_UP,
    JOY2_DOWN,
    JOY2_LEFT,
    JOY2_RIGHT,
    PROFILE_CYCLE
};

// Function declarations
void analog_init(void);
void analog_task(void);
int16_t analog_read_joy(uint8_t pin);
void set_joystick_mode(uint8_t joystick, joystick_mode_t mode);
joystick_mode_t get_joystick_mode(uint8_t joystick);
void cycle_joystick_profiles(void);
