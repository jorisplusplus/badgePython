/* This file specifies the framebuffer configuration for the displays that are supported. */
/* The order in this file determines priority if multiple drivers are enabled */

#pragma once

#ifdef CONFIG_DRIVER_FRAMEBUFFER_ENABLE

// #include "ssd1306.h"
// #include "sh1106.h"
// #include "erc12864.h"
// #include "eink.h"
// #include "ili9341.h"
// #include "hub75.h"
// #include "i2c_display.h"
// #include "gxgde0213b1.h"
// #include "fri3d.h"
// #include "flipdotter.h"
// #include "st7735.h"
// #include "st7789v.h"
// #include "nokia6100.h"
// #include "disobey_samd.h"
// #include "ledmatrix.h"
#include "framebuffer_display.h"

// /* E-INK display as used on the SHA2017 and HackerHotel 2019 badges */
// #if defined(CONFIG_DRIVER_EINK_ENABLE)
// 	#define FB_SIZE EINK_BUFFER_SIZE
// 	#define FB_WIDTH DRIVER_EINK_WIDTH
// 	#define FB_HEIGHT DRIVER_EINK_HEIGHT
// 	#ifdef CONFIG_DRIVER_EINK_FORCE_1BPP
// 		#define FB_TYPE_1BPP
// 		#define FB_1BPP_HORI_INV
// 		#warning "Greyscale is disabled because framebuffer is forced to 1 bit-per-pixel!"
// 	#else
// 		#define FB_TYPE_8BPP
// 		#define FB_ALPHA_ENABLED
// 		#define FB_FLUSH_GS(buffer,eink_flags) eink_display_greyscale(buffer,eink_flags,16);
// 	#endif
// 	//#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) eink_display(buffer,eink_flags);
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) eink_display_part(buffer,eink_flags,x0,x1);
// 	#define COLOR_FILL_DEFAULT 0xFFFFFF
// 	#define COLOR_TEXT_DEFAULT 0x000000

// /* E-INK display as used on the OHS badge */
// #elif defined(CONFIG_DRIVER_GXGDE0213B1_ENABLE)
// 	#define FB_SIZE GXGDE0213B1_BUFFER_SIZE
// 	#define FB_WIDTH GXGDE0213B1_WIDTH
// 	#define FB_HEIGHT GXGDE0213B1_HEIGHT
// 	#define FB_TYPE_1BPP
// 	#define FB_1BPP_OHS
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) gxgde0213b1_write(buffer)
// 	#define COLOR_FILL_DEFAULT 0xFFFFFF
// 	#define COLOR_TEXT_DEFAULT 0x000000
	
// /* OLED display as used on the Disobey 2020 badge */
// #elif defined(CONFIG_DRIVER_SSD1306_ENABLE)
// 	#define FB_SIZE SSD1306_BUFFER_SIZE
// 	#define FB_WIDTH SSD1306_WIDTH
// 	#define FB_HEIGHT SSD1306_HEIGHT
// 	#define FB_TYPE_1BPP
// 	#define FB_1BPP_VERT2
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) ssd1306_write_part(buffer,x0,y0,x1,y1)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// #elif defined(CONFIG_DRIVER_SH1106_ENABLE)
// 	#define FB_SIZE SH1106_BUFFER_SIZE
// 	#define FB_WIDTH SH1106_WIDTH
// 	#define FB_HEIGHT SH1106_HEIGHT
// 	#define FB_TYPE_1BPP
// 	#define FB_1BPP_VERT
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) sh1106_write_part(buffer,x0,y0,x1,y1)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// /* LCD display as used on the Disobey 2019 badge */
// #elif defined(CONFIG_DRIVER_ERC12864_ENABLE)
// 	#define FB_SIZE ERC12864_BUFFER_SIZE
// 	#define FB_WIDTH ERC12864_WIDTH
// 	#define FB_HEIGHT ERC12864_HEIGHT
// 	#define FB_TYPE_1BPP
// 	#define FB_1BPP_VERT
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) erc12864_write_part(buffer,x0,y0,x1,y1)
// 	#ifdef CONFIG_DRIVER_DISOBEY_SAMD_ENABLE
// 		#define FB_SET_BACKLIGHT(brightness) disobey_samd_write_backlight(brightness)
// 	#endif
// 	#define COLOR_FILL_DEFAULT 0xFFFFFF
// 	#define COLOR_TEXT_DEFAULT 0x000000

// /* LCD display as used on the Espressif Wrover kit */
// #elif defined(CONFIG_DRIVER_ILI9341_ENABLE)
// 	#define FB_SIZE ILI9341_BUFFER_SIZE
// 	#define FB_WIDTH ILI9341_WIDTH
// 	#define FB_HEIGHT ILI9341_HEIGHT
// 	#ifdef CONFIG_DRIVER_ILI9341_8C
// 			#define FB_TYPE_8CBPP
// 	#else
// 			#define FB_TYPE_16BPP
// 	#endif
// 	#define FB_ALPHA_ENABLED
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) ili9341_write_partial(buffer, x0, y0, x1, y1)
// 	#define FB_SET_BACKLIGHT(brightness) ili9341_set_backlight(brightness > 127)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF
	
// /* ST7735 */
// #elif defined(CONFIG_DRIVER_ST7735_ENABLE)
// 	#define FB_SIZE ST7735_BUFFER_SIZE
// 	#define FB_WIDTH ST7735_WIDTH
// 	#define FB_HEIGHT ST7735_HEIGHT
// 	#define FB_TYPE_16BPP
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) st7735_write_partial(buffer, x0, y0, x1, y1)
// 	#define FB_SET_BACKLIGHT(brightness) st7735_set_backlight(brightness > 127)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF
	
// /* ST7789V */
// #elif defined(CONFIG_DRIVER_ST7789V_ENABLE)
// 	#define FB_SIZE ST7789V_BUFFER_SIZE
// 	#define FB_WIDTH ST7789V_WIDTH
// 	#define FB_HEIGHT ST7789V_HEIGHT
// 	#ifdef CONFIG_DRIVER_ST7789V_8C
// 			#define FB_TYPE_8CBPP
// 	#else
// 			#define FB_TYPE_16BPP
// 	#endif
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) st7789v_write_partial(buffer, x0, y0, x1, y1)
// 	#define FB_SET_BACKLIGHT(brightness) st7789v_set_backlight(brightness > 127)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF
	
// /* HUB75 led matrix */
// #elif defined(CONFIG_DRIVER_HUB75_ENABLE)
//     #define CONFIG_DRIVER_FRAMEBUFFER_DOUBLE_BUFFERED
// 	#define FB_SIZE HUB75_BUFFER_SIZE
// 	#define FB_WIDTH HUB75_WIDTH
// 	#define FB_HEIGHT HUB75_HEIGHT
// 	#define FB_TYPE_32BPP
// 	#define FB_ALPHA_ENABLED
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) hub75_switch_buffer(buffer)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// /* FRI3D 2018 led matrix */
// #elif CONFIG_DRIVER_FRI3D_ENABLE
// 	#define FB_SIZE FRI3D_BUFFER_SIZE
// 	#define FB_WIDTH FRI3D_WIDTH
// 	#define FB_HEIGHT FRI3D_HEIGHT
// 	#define FB_TYPE_8BPP
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) fri3d_write(buffer);
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// /* Otter flipdot matrix */
// #elif CONFIG_DRIVER_FLIPDOTTER_ENABLE
//     #define CONFIG_DRIVER_FRAMEBUFFER_DOUBLE_BUFFERED
// 	#define FB_SIZE FLIPDOTTER_BUFFER_SIZE
// 	#define FB_WIDTH FLIPDOTTER_WIDTH
// 	#define FB_HEIGHT FLIPDOTTER_HEIGHT
// 	#define FB_TYPE_1BPP
// 	#define FB_1BPP_VERT
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) flipdotter_write(buffer);
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF
	
// /* Nokia 6100 LCD */
// #elif defined(CONFIG_DRIVER_NOKIA6100_ENABLE)
// 	#define FB_SIZE NOKIA6100_BUFFER_SIZE
// 	#define FB_WIDTH NOKIA6100_WIDTH
// 	#define FB_HEIGHT NOKIA6100_HEIGHT
// 	#define FB_TYPE_16BPP //HACK use 12-bit color depth when this mode is ready for use
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) nokia6100_write_partial(buffer, x0, y0, x1, y1)
// 	#define FB_SET_BACKLIGHT(brightness) nokia6100_set_backlight(brightness > 127)
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// /* LED matrix*/
// #elif defined(CONFIG_DRIVER_LEDMATRIX_ENABLE)
// 	#define FB_SIZE LEDMATRIX_BUFFER_SIZE
// 	#define FB_WIDTH LEDMATRIX_WIDTH
// 	#define FB_HEIGHT LEDMATRIX_HEIGHT
// 	#define FB_TYPE_8BPP
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) ledmatrix_set_buffer(buffer);
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF

// /* Generic i2c display*/
// #elif defined(CONFIG_DRIVER_DISPLAY_I2C_ENABLE)
// 	#define FB_SIZE I2C_DISPLAY_BUFFER_SIZE
// 	#define FB_WIDTH I2C_DISPLAY_WIDTH
// 	#define FB_HEIGHT I2C_DISPLAY_HEIGHT
// 	#define FB_TYPE_24BPP
// 	#define FB_FLUSH(buffer,eink_flags,x0,y0,x1,y1) i2c_display_write(buffer);
// 	#define COLOR_FILL_DEFAULT 0x000000
// 	#define COLOR_TEXT_DEFAULT 0xFFFFFF
// #else
// 	#error "Framebuffer driver enabled without a target display available!"
// #endif

#if defined(FB_TYPE_1BPP)
	#define PIXEL_SIZE 1
#elif defined(FB_TYPE_8BPP)
	#define PIXEL_SIZE 8
#elif defined(FB_TYPE_8CBPP)
	#define PIXEL_SIZE 8
#elif defined(FB_TYPE_12BPP)
	#define PIXEL_SIZE 12
#elif defined(FB_TYPE_16BPP)
	#define PIXEL_SIZE 16
#elif defined(FB_TYPE_24BPP)
	#define PIXEL_SIZE 24
#elif defined(FB_TYPE_32BPP)
	#define PIXEL_SIZE 32
#endif

#endif
