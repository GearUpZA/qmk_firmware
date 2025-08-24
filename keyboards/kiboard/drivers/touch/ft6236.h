#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    bool pressed;
} touch_point_t;

bool ft6236_init(void);
bool ft6236_read_touch(touch_point_t* point);
