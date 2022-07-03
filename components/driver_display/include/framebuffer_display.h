#ifndef __FRAMEBUFFER_DISPLAY_H__
#define __FRAMEBUFFER_DISPLAY_H__

#include "driver_display.h"

//This header file containes the defines used by the framebuffer driver.

#define FB_SIZE (CONFIG_DISPLAY_WIDTH*CONFIG_DISPLAY_HEIGHT/8)
#define FB_WIDTH CONFIG_DISPLAY_WIDTH
#define FB_HEIGHT CONFIG_DISPLAY_HEIGHT
#define FB_TYPE_1BPP
#define FB_1BPP_VERT
#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) driver_display_write_partial(buffer, x0, y0, x1, y1)
#define FB_SET_BACKLIGHT(brightness) driver_display_set_backlight(brightness > 127)
#define COLOR_FILL_DEFAULT 0x000000
#define COLOR_TEXT_DEFAULT 0xFFFFFF
#endif