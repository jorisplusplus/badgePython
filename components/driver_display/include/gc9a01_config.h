#pragma once
#include "esp_lcd_gc9a01.h"

#define BITS_PER_PIXEL (16)
#define FB_SIZE (CONFIG_DISPLAY_WIDTH*CONFIG_DISPLAY_HEIGHT)*2
#define FB_TYPE_16BPP

#define BUSCONFIG()                            \
    {                                          \
        .dc_gpio_num = CONFIG_DISPLAY_PIN_DC, \
        .cs_gpio_num = CONFIG_DISPLAY_PIN_CS, \
        .pclk_hz = 10000000,                   \
        .lcd_cmd_bits =  8,                    \
        .lcd_param_bits = 8,                   \
        .spi_mode = 0,                         \
        .trans_queue_depth = 10                \
    }

#define PANELCONFIG()           \
    {                           \
        .bits_per_pixel = BITS_PER_PIXEL,    \
        .reset_gpio_num = CONFIG_DISPLAY_PIN_RST,    \
        .color_space = LCD_RGB_ENDIAN_BGR \
    }

#define LCDSPI (1)

#define LCDINIT esp_lcd_new_panel_gc9a01