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
int16_t analog_read_joy(uint8_t pin);
void set_joystick_mode(uint8_t joystick, joystick_mode_t mode);
joystick_mode_t get_joystick_mode(uint8_t joystick);
void cycle_joystick_profiles(void);
