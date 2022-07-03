#pragma once

#ifndef NO_QSTR
#include <stdint.h>
#include <esp_err.h>
#include "spi_bus.h"
#include "i2c_bus.h"
#endif

esp_err_t start_buses();
spi_bus_handle_t get_vspi_bus();
spi_bus_handle_t get_hspi_bus();
spi_bus_handle_t get_spi_bus(int spi);
i2c_bus_handle_t get_i2c0_bus();
i2c_bus_handle_t get_i2c1_bus();
i2c_bus_handle_t get_i2c_bus(int i2c);