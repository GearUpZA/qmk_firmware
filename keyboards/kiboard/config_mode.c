#include QMK_KEYBOARD_H
#include "quantum.h"
#include "eeprom.h"
#include "config_mode.h"

// Config mode flag in EEPROM
#define CONFIG_MODE_ADDR 100
#define CONFIG_MODE_MAGIC 0x42

bool is_config_mode(void) {
    return eeprom_read_byte((uint8_t*)CONFIG_MODE_ADDR) == CONFIG_MODE_MAGIC;
}

void enter_config_mode(void) {
    eeprom_update_byte((uint8_t*)CONFIG_MODE_ADDR, CONFIG_MODE_MAGIC);
}

void exit_config_mode(void) {
    eeprom_update_byte((uint8_t*)CONFIG_MODE_ADDR, 0x00);
}

void init_config_mode(void) {
    // Initialize RPi_Touch_Pico here if needed
    // This will be handled by the Python script on the Pico
}
