#pragma once

#include <stdint.h>

// Display dimensions
#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 320

// Color definitions
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED   0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE  0x001F

void ili9341_init(void);
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_draw_string(uint16_t x, uint16_t y, const char* str, uint16_t color);
