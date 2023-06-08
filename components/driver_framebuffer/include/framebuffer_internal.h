#pragma once

#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_system.h"

#include "framebuffer_compositor.h"
#include "framebuffer_font.h"
#include "framebuffer_devices.h"
#include "framebuffer_dirty.h"
#include "framebuffer_orientation.h"
#include "framebuffer_drawing.h"
#include "framebuffer_text.h"
#include "framebuffer.h"

//PNG library
#include "mem_reader.h"
#include "file_reader.h"
#include "png_reader.h"
