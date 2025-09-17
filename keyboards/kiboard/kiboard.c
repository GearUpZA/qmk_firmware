#include "kiboard.h"
#include "config_mode.h"
#include "drivers/display/ili9341.h"
#include "drivers/touch/ft6236.h"
#include "config_ui.h"

// Touch point structure definition
typedef struct {
    uint16_t x;
    uint16_t y;
    bool pressed;
} touch_point_t;

// Color definitions
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000

// Function declarations for display and touch drivers
bool ili9341_init(void);
bool ft6236_init(void);
bool ft6236_read_touch(touch_point_t* point);
void ili9341_draw_string(uint16_t x, uint16_t y, const char* text, uint16_t color);

// Function declarations for config UI
void init_config_ui(void);
void process_touch_input(uint16_t x, uint16_t y);
void update_config_display(void);

// Weak function declarations for QMK callbacks
__attribute__((weak)) void keyboard_pre_init_user(void) {}
__attribute__((weak)) void matrix_init_user(void) {}
__attribute__((weak)) void matrix_scan_user(void) {}

void init_config_display(void) {
    if (!ili9341_init()) {
        // Missing error handling
        return;
    }
    if (!ft6236_init()) {
        // Should indicate error state on display
        ili9341_draw_string(10, 10, "Touch Init Failed!", COLOR_RED);
        return;
    }
    init_config_ui();
}

void run_config_interface(void) {
    touch_point_t touch;

    if (ft6236_read_touch(&touch)) {
        // Handle touch input
        process_touch_input(touch.x, touch.y);
    }

    // Update display as needed
    update_config_display();
}

void keyboard_pre_init_kb(void) {
    // Always initialize display for persistent operation
    init_config_display();

    // Initialize persistent display mode
    init_persistent_display();

    if (is_config_mode()) {
        // Set display to config mode
        update_display_mode(DISPLAY_MODE_CONFIG);
    } else {
        // Normal operation with status display
        update_display_mode(DISPLAY_MODE_STATUS);
        keyboard_pre_init_user();
    }
}

void matrix_init_kb(void) {
    if (is_config_mode()) {
        // Run configuration interface
        while(1) {
            run_config_interface();

            // Also handle display updates during config mode
            update_profile_display();

            // Check for touch input for display management
            touch_point_t touch;
            if (ft6236_read_touch(&touch)) {
                handle_display_touch(touch.x, touch.y);
            }
        }
    } else {
        // Normal QMK operation with persistent display
        matrix_init_user();
    }
}

void matrix_scan_kb(void) {
    // Handle display updates during normal operation
    update_profile_display();

    // Check for touch input for profile switching
    touch_point_t touch;
    if (ft6236_read_touch(&touch)) {
        handle_display_touch(touch.x, touch.y);
    }

    // Continue with normal matrix scanning
    matrix_scan_user();
}
