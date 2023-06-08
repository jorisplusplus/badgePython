#ifndef __FRAMEBUFFER_DISPLAY_H__
#define __FRAMEBUFFER_DISPLAY_H__

#include "display.h"

//This header file containes the defines used by the framebuffer driver.
#ifndef FB_SIZE
#define FB_SIZE (CONFIG_DISPLAY_WIDTH*CONFIG_DISPLAY_HEIGHT)
#endif

#define FB_WIDTH CONFIG_DISPLAY_WIDTH
#define FB_HEIGHT CONFIG_DISPLAY_HEIGHT
#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) display_write_partial(buffer, x0, y0, x1, y1)
#define FB_SET_BACKLIGHT(brightness) display_set_backlight(brightness > 127)
#define COLOR_FILL_DEFAULT 0x000000
#define COLOR_TEXT_DEFAULT 0xFFFFFF
#endif