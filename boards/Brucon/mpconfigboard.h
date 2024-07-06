#include "platform.h"

#define MICROPY_HW_BOARD_NAME "Brucon 2024 even badge"
#define MICROPY_HW_MCU_NAME "ESP32-S2"
#define MICROPY_PY_BLUETOOTH (0)
#define MICROPY_PY_NETWORK_LAN (0)

#ifndef MICROPY_BOARD_STARTUP
#define MICROPY_BOARD_STARTUP platform_init
#endif

#define MICROPY_HW_ENABLE_SDCARD (0)
#define MICROPY_PY_MACHINE_I2S (0)
#define MICROPY_HW_ENABLE_UART_REPL (0)