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

#include "profile_manager.h"
#include "eeconfig.h"

// Use QMK's eeconfig user data area
typedef union {
    uint32_t raw;
    struct {
        uint8_t active_profile   :2;
        uint8_t joy1_analog_mode :1;
        uint8_t joy2_analog_mode :1;
        uint8_t touchscreen_enabled :1;
        uint8_t combo_enabled    :1;
        uint8_t joy1_sensitivity :4;
        uint8_t joy2_sensitivity :4;
        uint16_t joy1_deadzone   :8;
        uint16_t joy2_deadzone   :8;
        uint8_t reserved         :2;
    };
} user_config_t;

// Global state
static profile_manager_state_t manager_state = {0};
static user_config_t user_config;

// Default profiles
const profile_data_t default_gaming_profile = {
    .name = "Gaming",
    .enabled = true,
    .joystick1 = {.analog_mode = true, .x_center = 512, .y_center = 512, .deadzone = 50, .sensitivity = 8},
    .joystick2 = {.analog_mode = true, .x_center = 512, .y_center = 512, .deadzone = 50, .sensitivity = 8},
    .combo_config = {.enabled = true, .timeout_ms = 150, .strict_timing = false},
    .touchscreen_config = {.enabled = true, .sensitivity = 5},
    .layer_config = {.default_layer = 0}
};

void profile_manager_init(void) {
    // Read user config from QMK's eeconfig
    user_config.raw = eeconfig_read_user();

    // Initialize with defaults if invalid
    if (user_config.active_profile >= MAX_PROFILES) {
        user_config.active_profile = 0;
        user_config.joy1_analog_mode = true;
        user_config.joy2_analog_mode = true;
        user_config.touchscreen_enabled = true;
        user_config.combo_enabled = true;
        user_config.joy1_sensitivity = 8;
        user_config.joy2_sensitivity = 8;
        user_config.joy1_deadzone = 50;
        user_config.joy2_deadzone = 50;

        eeconfig_update_user(user_config.raw);
    }

    // Initialize default profile
    manager_state.profiles[0] = default_gaming_profile;
    manager_state.config.active_profile = user_config.active_profile;
    manager_state.config.num_profiles = 1;
    manager_state.initialized = true;

    // Apply current profile
    profile_manager_apply_current_profile();
}

bool profile_manager_switch_profile(uint8_t profile_id) {
    if (profile_id >= MAX_PROFILES) {
        return false;
    }

    user_config.active_profile = profile_id;
    manager_state.config.active_profile = profile_id;

    // Save to eeconfig
    eeconfig_update_user(user_config.raw);

    profile_manager_apply_current_profile();
    return true;
}

void profile_manager_apply_current_profile(void) {
    extern kiboard_state_t kiboard_state;

    // Apply settings from eeconfig
    kiboard_state.joystick1.analog_mode = user_config.joy1_analog_mode;
    kiboard_state.joystick2.analog_mode = user_config.joy2_analog_mode;
    kiboard_state.touchscreen_enabled = user_config.touchscreen_enabled;
    kiboard_state.combo_enabled = user_config.combo_enabled;
    kiboard_state.joystick1.sensitivity = user_config.joy1_sensitivity;
    kiboard_state.joystick2.sensitivity = user_config.joy2_sensitivity;
    kiboard_state.joystick1.deadzone = user_config.joy1_deadzone;
    kiboard_state.joystick2.deadzone = user_config.joy2_deadzone;

    // Enable/disable combos
    if (kiboard_state.combo_enabled) {
        combo_enable();
    } else {
        combo_disable();
    }
}

bool profile_manager_save_current(void) {
    extern kiboard_state_t kiboard_state;

    // Update config from current state
    user_config.joy1_analog_mode = kiboard_state.joystick1.analog_mode;
    user_config.joy2_analog_mode = kiboard_state.joystick2.analog_mode;
    user_config.touchscreen_enabled = kiboard_state.touchscreen_enabled;
    user_config.combo_enabled = kiboard_state.combo_enabled;
    user_config.joy1_sensitivity = kiboard_state.joystick1.sensitivity;
    user_config.joy2_sensitivity = kiboard_state.joystick2.sensitivity;
    user_config.joy1_deadzone = kiboard_state.joystick1.deadzone;
    user_config.joy2_deadzone = kiboard_state.joystick2.deadzone;

    // Save to eeconfig
    eeconfig_update_user(user_config.raw);

    manager_state.dirty = false;
    return true;
}

void profile_manager_factory_reset(void) {
    // Reset eeconfig user data
    user_config.raw = 0;
    user_config.active_profile = 0;
    user_config.joy1_analog_mode = true;
    user_config.joy2_analog_mode = true;
    user_config.touchscreen_enabled = true;
    user_config.combo_enabled = true;
    user_config.joy1_sensitivity = 8;
    user_config.joy2_sensitivity = 8;
    user_config.joy1_deadzone = 50;
    user_config.joy2_deadzone = 50;

    eeconfig_update_user(user_config.raw);

    // Reset profiles
    manager_state.profiles[0] = default_gaming_profile;
    manager_state.config.active_profile = 0;

    profile_manager_apply_current_profile();
}

// Simplified implementations for essential functions
uint8_t profile_manager_get_active_profile(void) {
    return user_config.active_profile;
}

profile_data_t* profile_manager_get_active_profile_data(void) {
    return &manager_state.profiles[user_config.active_profile];
}

void profile_manager_mark_dirty(void) {
    manager_state.dirty = true;
}

void profile_manager_task(void) {
    // Auto-save if dirty and some time has passed
    if (manager_state.dirty) {
        static uint32_t last_activity = 0;
        if (timer_elapsed32(last_activity) > 5000) { // 5 second delay
            profile_manager_save_current();
            last_activity = timer_read32();
        }
    }
}

void profile_manager_emergency_reset(void) {
    profile_manager_factory_reset();
}

void profile_manager_reset_current(void) {
    profile_manager_factory_reset();
}
