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

#include "combo_kiboard.h"
#include "analog.h"
#include "joystick.h"
#include "pointing_device.h"
#include "lib/profile_manager.h"
#include "lib/touch_interface.h"

// Global state
kiboard_state_t kiboard_state = {
    .joystick1 = {
        .analog_mode = true,
        .x_center = 512,
        .y_center = 512,
        .deadzone = ANALOG_JOYSTICK_DEADZONE,
        .sensitivity = ANALOG_JOYSTICK_SENSITIVITY
    },
    .joystick2 = {
        .analog_mode = true,
        .x_center = 512,
        .y_center = 512,
        .deadzone = ANALOG_JOYSTICK_DEADZONE,
        .sensitivity = ANALOG_JOYSTICK_SENSITIVITY
    },
    .touchscreen_enabled = true,
    .combo_enabled = true,
    .current_profile = 0,
    .debug_mode = false
};

void keyboard_post_init_kb(void) {
    // Initialize profile manager (uses QMK's eeconfig)
    profile_manager_init();

    // Initialize touchscreen
    touch_interface_init();

    // Apply current profile settings
    profile_manager_apply_current_profile();

    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    // Update joysticks using QMK's joystick API
    update_joysticks();

    // Handle touchscreen if enabled
    if (kiboard_state.touchscreen_enabled) {
        touch_interface_task();
    }

    // Profile manager tasks
    profile_manager_task();

    housekeeping_task_user();
}

void kiboard_task(void) {
    static uint32_t last_mode_check = 0;

    // Check mode switches every 50ms
    if (timer_elapsed32(last_mode_check) > 50) {
        // Check joystick mode switches
        static bool joy1_last_state = true;
        static bool joy2_last_state = true;

        bool joy1_current = readPin(JOYSTICK1_MODE_PIN);
        bool joy2_current = readPin(JOYSTICK2_MODE_PIN);

        // Detect falling edge (button press)
        if (joy1_last_state && !joy1_current) {
            joystick_toggle_mode(1);
        }
        if (joy2_last_state && !joy2_current) {
            joystick_toggle_mode(2);
        }

        joy1_last_state = joy1_current;
        joy2_last_state = joy2_current;
        last_mode_check = timer_read32();
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Process custom keycodes
    if (!process_record_kiboard(keycode, record)) {
        return false;
    }

    // Pass to combo handler
    if (kiboard_state.combo_enabled) {
        combo_handler_process_record(keycode, record);
    }

    return process_record_user(keycode, record);
}

bool process_record_kiboard(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CK_PROF1:
            if (record->event.pressed) {
                profile_manager_switch_profile(0);
                kiboard_state.current_profile = 0;
            }
            return false;

        case CK_PROF2:
            if (record->event.pressed) {
                profile_manager_switch_profile(1);
                kiboard_state.current_profile = 1;
            }
            return false;

        case CK_PROF3:
            if (record->event.pressed) {
                profile_manager_switch_profile(2);
                kiboard_state.current_profile = 2;
            }
            return false;

        case CK_PROF4:
            if (record->event.pressed) {
                profile_manager_switch_profile(3);
                kiboard_state.current_profile = 3;
            }
            return false;

        case CK_JOY1_MODE:
            if (record->event.pressed) {
                joystick_toggle_mode(1);
            }
            return false;

        case CK_JOY2_MODE:
            if (record->event.pressed) {
                joystick_toggle_mode(2);
            }
            return false;

        case CK_TOUCH_CAL:
            if (record->event.pressed) {
                touchscreen_calibrate();
            }
            return false;

        case CK_COMBO_TOG:
            if (record->event.pressed) {
                kiboard_state.combo_enabled = !kiboard_state.combo_enabled;
                if (kiboard_state.debug_mode) {
                    kiboard_debug_print(kiboard_state.combo_enabled ? "Combos ON" : "Combos OFF");
                }
            }
            return false;
    }

    return true;
}

void joystick_init(void) {
    // Initialize analog pins
    setPinInputHigh(ANALOG_JOYSTICK_X_AXIS_PIN);
    setPinInputHigh(ANALOG_JOYSTICK_Y_AXIS_PIN);
    setPinInputHigh(ANALOG_JOYSTICK2_X_AXIS_PIN);
    setPinInputHigh(ANALOG_JOYSTICK2_Y_AXIS_PIN);

    // Initialize digital pins
    setPinInputHigh(JOYSTICK1_DIGITAL_X_PIN);
    setPinInputHigh(JOYSTICK1_DIGITAL_Y_PIN);
    setPinInputHigh(JOYSTICK2_DIGITAL_X_PIN);
    setPinInputHigh(JOYSTICK2_DIGITAL_Y_PIN);
}

void joystick_task(void) {
    static uint32_t last_update = 0;

    // Update joysticks every 10ms
    if (timer_elapsed32(last_update) > 10) {
        // Process joystick 1
        if (kiboard_state.joystick1.analog_mode) {
            uint16_t x = read_joystick_x(1);
            uint16_t y = read_joystick_y(1);

            // Send analog joystick data
            joystick_set_axis(0, x - 512);
            joystick_set_axis(1, y - 512);
        } else {
            // Process digital joystick inputs
            bool left = !readPin(JOYSTICK1_DIGITAL_X_PIN);
            bool right = readPin(JOYSTICK1_DIGITAL_X_PIN);
            bool up = !readPin(JOYSTICK1_DIGITAL_Y_PIN);
            bool down = readPin(JOYSTICK1_DIGITAL_Y_PIN);

            // Convert to key presses (implement as needed)
        }

        // Process joystick 2 similarly
        if (kiboard_state.joystick2.analog_mode) {
            uint16_t x = read_joystick_x(2);
            uint16_t y = read_joystick_y(2);

            joystick_set_axis(2, x - 512);
            joystick_set_axis(3, y - 512);
        }

        last_update = timer_read32();
    }
}

uint16_t read_joystick_x(uint8_t joystick_id) {
    pin_t pin = (joystick_id == 1) ? ANALOG_JOYSTICK_X_AXIS_PIN : ANALOG_JOYSTICK2_X_AXIS_PIN;
    return analogReadPin(pin);
}

uint16_t read_joystick_y(uint8_t joystick_id) {
    pin_t pin = (joystick_id == 1) ? ANALOG_JOYSTICK_Y_AXIS_PIN : ANALOG_JOYSTICK2_Y_AXIS_PIN;
    return analogReadPin(pin);
}

void joystick_toggle_mode(uint8_t joystick_id) {
    if (joystick_id == 1) {
        kiboard_state.joystick1.analog_mode = !kiboard_state.joystick1.analog_mode;
        if (kiboard_state.debug_mode) {
            kiboard_debug_print(kiboard_state.joystick1.analog_mode ? "Joy1: Analog" : "Joy1: Digital");
        }
    } else if (joystick_id == 2) {
        kiboard_state.joystick2.analog_mode = !kiboard_state.joystick2.analog_mode;
        if (kiboard_state.debug_mode) {
            kiboard_debug_print(kiboard_state.joystick2.analog_mode ? "Joy2: Analog" : "Joy2: Digital");
        }
    }

    // Save configuration
    profile_manager_save_current();
}

void touchscreen_init(void) {
    // Initialize SPI pins
    setPinOutput(TOUCHSCREEN_CS_PIN);
    setPinOutput(TOUCHSCREEN_RST_PIN);
    setPinInputHigh(TOUCHSCREEN_INT_PIN);

    // Reset touchscreen
    writePinLow(TOUCHSCREEN_RST_PIN);
    wait_ms(10);
    writePinHigh(TOUCHSCREEN_RST_PIN);
    wait_ms(50);
}

void touchscreen_task(void) {
    // Check for touch interrupt
    if (!readPin(TOUCHSCREEN_INT_PIN)) {
        touch_interface_handle_touch();
    }
}

bool touchscreen_calibrate(void) {
    if (kiboard_state.debug_mode) {
        kiboard_debug_print("Starting touchscreen calibration...");
    }

    return touch_interface_calibrate();
}

void factory_reset(void) {
    // Reset all settings to defaults
    kiboard_state.joystick1.analog_mode = true;
    kiboard_state.joystick2.analog_mode = true;
    kiboard_state.touchscreen_enabled = true;
    kiboard_state.combo_enabled = true;
    kiboard_state.current_profile = 0;

    // Reset profile manager
    profile_manager_factory_reset();

    // Reset combo handler
    combo_handler_reset();

    if (kiboard_state.debug_mode) {
        kiboard_debug_print("Factory reset complete");
    }
}

void emergency_reset(void) {
    // Emergency reset - disable all custom features
    kiboard_state.combo_enabled = false;
    kiboard_state.touchscreen_enabled = false;

    // Switch to basic profile
    kiboard_state.current_profile = 0;
    profile_manager_emergency_reset();

    if (kiboard_state.debug_mode) {
        kiboard_debug_print("Emergency reset activated");
    }
}

void kiboard_debug_print(const char* message) {
#ifdef CONSOLE_ENABLE
    uprintf("KIBOARD: %s\n", message);
#endif
}

void kiboard_error_handler(const char* error) {
    kiboard_debug_print(error);
    // Could add LED indication or other error handling here
}
