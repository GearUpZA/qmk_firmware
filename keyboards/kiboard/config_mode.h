#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "quantum.h"

#define CONFIG_MODE_ADDR 100
#define CONFIG_MODE_MAGIC 0x42

// Display modes
typedef enum {
    DISPLAY_MODE_OFF,           // Display off (power saving)
    DISPLAY_MODE_STATUS,        // Show current profile and basic info
    DISPLAY_MODE_PROFILE_SELECT,// Profile selection interface
    DISPLAY_MODE_CONFIG         // Full configuration mode
} display_mode_t;

// Profile display info
typedef struct {
    char name[16];
    char joy1_mode[8];
    char joy2_mode[8];
    uint8_t profile_id;
} profile_info_t;

// Configuration mode functions
bool is_config_mode(void);
void enter_config_mode(void);
void exit_config_mode(void);
void init_config_mode(void);

// Display management functions
void init_persistent_display(void);
void update_display_mode(display_mode_t mode);
display_mode_t get_display_mode(void);
void handle_display_touch(uint16_t x, uint16_t y);

// Profile display functions
void show_profile_status(void);
void show_profile_selector(void);
void update_profile_display(void);
void cycle_display_mode(void);

// Touch zone definitions for profile interface
#define TOUCH_ZONE_PROFILE_1_X  10
#define TOUCH_ZONE_PROFILE_1_Y  50
#define TOUCH_ZONE_PROFILE_2_X  10
#define TOUCH_ZONE_PROFILE_2_Y  100
#define TOUCH_ZONE_PROFILE_3_X  10
#define TOUCH_ZONE_PROFILE_3_Y  150
#define TOUCH_ZONE_PROFILE_4_X  10
#define TOUCH_ZONE_PROFILE_4_Y  200
#define TOUCH_ZONE_WIDTH        300
#define TOUCH_ZONE_HEIGHT       40

#define TOUCH_ZONE_CONFIG_X     200
#define TOUCH_ZONE_CONFIG_Y     10
#define TOUCH_ZONE_CONFIG_W     110
#define TOUCH_ZONE_CONFIG_H     30
