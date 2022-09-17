#pragma once
#include "driver_display.h"



#define BUSCONFIG()                            \
    {                                          \
        .dev_addr = CONFIG_DISPLAY_I2C_ADDR,   \
        .control_phase_bytes = 1,              \
        .dc_bit_offset = 6,                    \
        .lcd_cmd_bits = 8,                     \
        .lcd_param_bits = 8                    \
    }

#define PANELCONFIG()           \
    {                           \
        .bits_per_pixel = 1,    \
        .reset_gpio_num = -1,    \
        .color_space = ESP_LCD_COLOR_SPACE_MONOCHROME \
    }

#define LCDPORT LCDI2C