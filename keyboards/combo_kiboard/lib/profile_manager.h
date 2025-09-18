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
#include "eeconfig.h"

// Profile system configuration
#define MAX_PROFILES 4
#define PROFILE_NAME_LENGTH 16

// EEPROM layout for profile data
#define PROFILE_EEPROM_BASE_ADDR (EECONFIG_SIZE + EECONFIG_KEYMAP_SIZE + EECONFIG_USER_SIZE)
#define PROFILE_EEPROM_SIZE (sizeof(profile_data_t) * MAX_PROFILES + sizeof(profile_manager_config_t))

// Profile configuration structure
typedef struct {
    char name[PROFILE_NAME_LENGTH];     // Human-readable profile name
    bool enabled;                       // Whether this profile is active
    
    // Joystick settings
    struct {
        bool analog_mode;               // True for analog, false for digital
        uint16_t x_center;              // Calibrated center X position
        uint16_t y_center;              // Calibrated center Y position
        uint16_t deadzone;              // Deadzone radius
        uint8_t sensitivity;            // Sensitivity multiplier
        bool inverted_x;                // Invert X axis
        bool inverted_y;                // Invert Y axis
    } joystick1, joystick2;
    
    // Combo settings
    struct {
        bool enabled;                   // Enable/disable combos
        uint16_t timeout_ms;            // Combo timeout
        bool strict_timing;             // Require precise timing
        uint8_t enabled_combos[16];     // Bitmask of enabled combos
    } combo_config;
    
    // Touchscreen settings
    struct {
        bool enabled;                   // Enable/disable touchscreen
        uint16_t calibration[8];        // Calibration data
        uint8_t sensitivity;            // Touch sensitivity
        bool haptic_feedback;           // Enable haptic feedback
    } touchscreen_config;
    
    // Layer settings
    struct {
        uint8_t default_layer;          // Default layer for this profile
        bool layer_lock;                // Lock layers in this profile
        uint8_t quick_layers[4];        // Quick access layers
    } layer_config;
    
    // Timing settings
    struct {
        uint16_t debounce_ms;           // Button debounce time
        uint16_t joystick_poll_ms;      // Joystick polling interval
        uint16_t touch_poll_ms;         // Touch polling interval
    } timing_config;
    
    // Feature flags
    struct {
        bool auto_save;                 // Auto-save profile changes
        bool quick_switch;              // Allow quick profile switching
        bool reset_protection;         // Protect against accidental reset
    } feature_flags;
    
    // CRC for data integrity
    uint16_t crc;
} profile_data_t;

// Profile manager global configuration
typedef struct {
    uint8_t active_profile;             // Currently active profile
    uint8_t num_profiles;               // Number of configured profiles
    bool auto_load_on_boot;             // Load profile on startup
    bool profile_switching_enabled;     // Allow profile switching
    uint32_t last_save_time;            // Last time profiles were saved
    uint16_t config_version;            // Configuration version
    uint16_t crc;                       // Configuration CRC
} profile_manager_config_t;

// Profile manager state
typedef struct {
    profile_data_t profiles[MAX_PROFILES];
    profile_manager_config_t config;
    bool initialized;
    bool dirty;                         // Needs saving
    uint32_t last_activity;             // Last profile activity
    uint8_t backup_profile;             // Emergency backup profile
} profile_manager_state_t;

// Function declarations
void profile_manager_init(void);
void profile_manager_task(void);

// Profile management
bool profile_manager_create_profile(uint8_t profile_id, const char* name);
bool profile_manager_delete_profile(uint8_t profile_id);
bool profile_manager_switch_profile(uint8_t profile_id);
uint8_t profile_manager_get_active_profile(void);
bool profile_manager_copy_profile(uint8_t src_id, uint8_t dst_id);

// Profile data access
profile_data_t* profile_manager_get_profile(uint8_t profile_id);
profile_data_t* profile_manager_get_active_profile_data(void);
bool profile_manager_is_profile_valid(uint8_t profile_id);
const char* profile_manager_get_profile_name(uint8_t profile_id);
bool profile_manager_set_profile_name(uint8_t profile_id, const char* name);

// Configuration access
bool profile_manager_load_profile(uint8_t profile_id);
bool profile_manager_save_profile(uint8_t profile_id);
bool profile_manager_save_current(void);
bool profile_manager_save_all(void);
void profile_manager_mark_dirty(void);

// Joystick profile functions
void profile_manager_set_joystick_mode(uint8_t profile_id, uint8_t joystick_id, bool analog_mode);
void profile_manager_set_joystick_calibration(uint8_t profile_id, uint8_t joystick_id, 
                                             uint16_t x_center, uint16_t y_center, uint16_t deadzone);
void profile_manager_set_joystick_sensitivity(uint8_t profile_id, uint8_t joystick_id, uint8_t sensitivity);
bool profile_manager_get_joystick_mode(uint8_t profile_id, uint8_t joystick_id);

// Combo profile functions
void profile_manager_set_combo_enabled(uint8_t profile_id, bool enabled);
void profile_manager_set_combo_timeout(uint8_t profile_id, uint16_t timeout_ms);
void profile_manager_enable_combo(uint8_t profile_id, uint8_t combo_id);
void profile_manager_disable_combo(uint8_t profile_id, uint8_t combo_id);
bool profile_manager_is_combo_enabled(uint8_t profile_id, uint8_t combo_id);

// Touchscreen profile functions
void profile_manager_set_touchscreen_enabled(uint8_t profile_id, bool enabled);
void profile_manager_set_touchscreen_calibration(uint8_t profile_id, const uint16_t* calibration);
void profile_manager_set_touchscreen_sensitivity(uint8_t profile_id, uint8_t sensitivity);

// Layer profile functions
void profile_manager_set_default_layer(uint8_t profile_id, uint8_t layer);
void profile_manager_set_quick_layer(uint8_t profile_id, uint8_t slot, uint8_t layer);
uint8_t profile_manager_get_default_layer(uint8_t profile_id);

// Timing profile functions
void profile_manager_set_debounce_time(uint8_t profile_id, uint16_t debounce_ms);
void profile_manager_set_poll_rates(uint8_t profile_id, uint16_t joystick_ms, uint16_t touch_ms);

// Reset and recovery functions
void profile_manager_reset_profile(uint8_t profile_id);
void profile_manager_reset_current(void);
void profile_manager_factory_reset(void);
void profile_manager_emergency_reset(void);
bool profile_manager_create_backup(void);
bool profile_manager_restore_backup(void);

// Validation and integrity
bool profile_manager_validate_profile(uint8_t profile_id);
bool profile_manager_repair_profile(uint8_t profile_id);
uint16_t profile_manager_calculate_crc(const profile_data_t* profile);
bool profile_manager_verify_integrity(void);

// Import/Export functions
bool profile_manager_export_profile(uint8_t profile_id, uint8_t* buffer, size_t buffer_size);
bool profile_manager_import_profile(uint8_t profile_id, const uint8_t* buffer, size_t buffer_size);

// Configuration functions
void profile_manager_set_auto_save(bool enabled);
void profile_manager_set_auto_load(bool enabled);
bool profile_manager_get_auto_save(void);
bool profile_manager_get_auto_load(void);

// Utility functions
void profile_manager_apply_current_profile(void);
void profile_manager_debug_print_profile(uint8_t profile_id);
uint32_t profile_manager_get_last_save_time(void);
bool profile_manager_is_dirty(void);

// Default profile templates
extern const profile_data_t default_gaming_profile;
extern const profile_data_t default_productivity_profile;
extern const profile_data_t default_minimal_profile;
extern const profile_data_t default_advanced_profile;

// Error codes
typedef enum {
    PROFILE_SUCCESS = 0,
    PROFILE_ERROR_INVALID_ID,
    PROFILE_ERROR_NOT_FOUND,
    PROFILE_ERROR_FULL,
    PROFILE_ERROR_EEPROM,
    PROFILE_ERROR_CRC,
    PROFILE_ERROR_VERSION,
    PROFILE_ERROR_SIZE
} profile_error_t;