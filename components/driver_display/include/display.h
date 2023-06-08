#ifndef __DRIVER_DISPLAY_H__
#define __DRIVER_DISPLAY_H__

#include <stdint.h>
#include "esp_err.h"

#if CONFIG_DISPLAY_SCREEN_CONTROLLER_SSD1306
#include "sdd1306_config.h"
#elif CONFIG_DISPLAY_SCREEN_CONTROLLER_GC9A01
#include "gc9a01_config.h"
#endif

void display_init();
esp_err_t display_set_backlight(uint8_t brightness);
esp_err_t display_write_partial(const uint8_t *buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
#endif