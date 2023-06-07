#include "platform.h"

#define MICROPY_HW_BOARD_NAME "ESP32 module"
#define MICROPY_HW_MCU_NAME "ESP32"
#define MICROPY_PY_BLUETOOTH (0)
#define MICROPY_PY_NETWORK_LAN (0)

#ifndef MICROPY_BOARD_STARTUP
#define MICROPY_BOARD_STARTUP platform_init
#endif