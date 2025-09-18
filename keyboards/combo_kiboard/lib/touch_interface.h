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

#include "touch_interface.h"
#include "spi_master.h"

static touch_zone_t touch_zones[TOUCH_ZONES];
static bool initialized = false;

void touch_interface_init(void) {
    // Initialize SPI
    spi_init();
    
    // Set up default touch zones for profile switching
    touch_interface_set_zone(0, 0, 0, 80, 60, 1);     // Profile 1
    touch_interface_set_zone(1, 80, 0, 160, 60, 2);   // Profile 2
    touch_interface_set_zone(2, 160, 0, 240, 60, 3);  // Profile 3
    touch_interface_set_zone(3, 240, 0, 320, 60, 4);  // Profile 4
    
    initialized = true;
}

void touch_interface_task(void) {
    if (!initialized) return;
    
    // Check for touch events
    if (!readPin(TOUCHSCREEN_INT_PIN)) {
        touch_interface_handle_touch();
    }
}

void touch_interface_handle_touch(void) {
    touch_point_t point;
    if (touch_interface_read_point(&point)) {
        // Check which zone was touched
        for (uint8_t i = 0; i < TOUCH_ZONES; i++) {
            touch_zone_t* zone = &touch_zones[i];
            if (point.x >= zone->x1 && point.x <= zone->x2 && 
                point.y >= zone->y1 && point.y <= zone->y2) {
                
                // Execute zone action
                switch (zone->action_type) {
                    case 1: // Profile switch
                        if (zone->action_data <= 4) {
                            extern void profile_manager_switch_profile(uint8_t profile_id);
                            profile_manager_switch_profile(zone->action_data - 1);
                        }
                        break;
                }
                break;
            }
        }
    }
}

bool touch_interface_read_point(touch_point_t* point) {
    // Simplified touch reading - would implement actual SPI communication here
    // This is a placeholder that would read from the Waveshare touchscreen
    point->x = 160;  // Placeholder
    point->y = 120;  // Placeholder
    point->active = true;
    point->timestamp = timer_read32();
    return true;
}

bool touch_interface_calibrate(void) {
    // Placeholder for calibration routine
    return true;
}

void touch_interface_set_zone(uint8_t zone_id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t action) {
    if (zone_id < TOUCH_ZONES) {
        touch_zones[zone_id] = (touch_zone_t){x1, y1, x2, y2, action, action};
    }
}