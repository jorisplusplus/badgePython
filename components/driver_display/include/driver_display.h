#ifndef __DRIVER_DISPLAY_H__
#define __DRIVER_DISPLAY_H__

#include <stdint.h>
#include "esp_err.h"

void driver_display_init();
esp_err_t driver_display_set_backlight(uint8_t brightness);
esp_err_t driver_display_write_partial(const uint8_t *buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
#endif