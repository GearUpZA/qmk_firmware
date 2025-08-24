#include "ili9341.h"
#include "spi_master.h"

#define LCD_DC_PIN GP16
#define LCD_CS_PIN GP17
#define LCD_RST_PIN GP18
#define LCD_BL_PIN GP19

static void ili9341_write_cmd(uint8_t cmd) {
    writePinLow(LCD_DC_PIN);
    spi_start(LCD_CS_PIN, false, 0, 8);
    spi_send(cmd);
    spi_stop();
}

static void ili9341_write_data(uint8_t data) {
    writePinHigh(LCD_DC_PIN);
    spi_start(LCD_CS_PIN, false, 0, 8);
    spi_send(data);
    spi_stop();
}

void ili9341_init(void) {
    // Configure pins
    setPinOutput(LCD_DC_PIN);
    setPinOutput(LCD_CS_PIN);
    setPinOutput(LCD_RST_PIN);
    setPinOutput(LCD_BL_PIN);

    // Reset sequence
    writePinHigh(LCD_RST_PIN);
    wait_ms(5);
    writePinLow(LCD_RST_PIN);
    wait_ms(20);
    writePinHigh(LCD_RST_PIN);
    wait_ms(150);

    // Initialize display
    ili9341_write_cmd(0x01);  // Software reset
    wait_ms(100);

    ili9341_write_cmd(0xCF);  // Power control B
    ili9341_write_data(0x00);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x30);

    // ... More initialization commands ...

    ili9341_write_cmd(0x29);  // Display ON
    wait_ms(100);

    // Turn on backlight
    writePinHigh(LCD_BL_PIN);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    ili9341_write_cmd(0x2A);  // Column address set
    ili9341_write_data(x >> 8);
    ili9341_write_data(x & 0xFF);
    ili9341_write_cmd(0x2B);  // Row address set
    ili9341_write_data(y >> 8);
    ili9341_write_data(y & 0xFF);
    ili9341_write_cmd(0x2C);  // Memory write
    ili9341_write_data(color >> 8);
    ili9341_write_data(color & 0xFF);
}
