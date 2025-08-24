#include "ft6236.h"
#include "i2c_master.h"

#define FT6236_ADDR 0x38
#define TOUCH_RST_PIN GP20

bool ft6236_init(void) {
    setPinOutput(TOUCH_RST_PIN);

    // Reset touch controller
    writePinLow(TOUCH_RST_PIN);
    wait_ms(5);
    writePinHigh(TOUCH_RST_PIN);
    wait_ms(100);

    // Initialize I2C
    i2c_init();

    // Check if device responds
    uint8_t data = 0;
    if (!i2c_readReg(FT6236_ADDR, 0x00, &data, 1)) {
        return false;
    }

    return true;
}

bool ft6236_read_touch(touch_point_t* point) {
    uint8_t data[4];

    if (!i2c_readReg(FT6236_ADDR, 0x02, data, 4)) {
        return false;
    }

    if ((data[0] & 0x0F) == 0) {
        return false;  // No touch detected
    }

    point->x = ((data[0] & 0xF0) << 4) | data[1];
    point->y = ((data[2] & 0xF0) << 4) | data[3];
    return true;
}
