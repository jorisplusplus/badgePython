#include <sdkconfig.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <esp_log.h>
#include <esp_err.h>

#include <driver/spi_master.h>
#include <driver/i2c.h>

#include "include/buses.h"
#include "spi_bus.h"
#include "i2c_bus.h"

#define I2C_MASTER_TX_BUF_DISABLE  0
#define I2C_MASTER_RX_BUF_DISABLE  0
#define WRITE_BIT      I2C_MASTER_WRITE // I2C master write
#define READ_BIT       I2C_MASTER_READ  // I2C master read
#define ACK_CHECK_EN   0x1     // I2C master will check ack from slave
#define ACK_CHECK_DIS  0x0     // I2C master will not check ack from slave
#define ACK_VAL        0x0     // I2C ack value
#define NACK_VAL       0x1     // I2C nack value

static spi_bus_handle_t vspi_bus_handle = NULL;
static spi_bus_handle_t hspi_bus_handle = NULL;
static i2c_bus_handle_t i2c0_bus_handle = NULL;
static i2c_bus_handle_t i2c1_bus_handle = NULL;
esp_err_t start_buses() {
    // This function initializes the VSPI, HSPI and I2C buses of the ESP32
    #ifdef CONFIG_BUS_VSPI_ENABLE
        spi_config_t vspiBusConfiguration = {0};
        vspiBusConfiguration.mosi_io_num     = CONFIG_PIN_NUM_VSPI_MOSI;
        vspiBusConfiguration.miso_io_num     = CONFIG_PIN_NUM_VSPI_MISO;
        vspiBusConfiguration.sclk_io_num     = CONFIG_PIN_NUM_VSPI_CLK;
        vspiBusConfiguration.max_transfer_sz = CONFIG_BUS_VSPI_MAX_TRANSFERSIZE;
        vspi_bus_handle = spi_bus_create(VSPI_HOST, &vspiBusConfiguration);
        if (res != ESP_OK) return res;
    #endif

    #ifdef CONFIG_BUS_HSPI_ENABLE
        spi_config_t hspiBusConfiguration = {0};
        hspiBusConfiguration.mosi_io_num     = CONFIG_PIN_NUM_HSPI_MOSI;
        hspiBusConfiguration.miso_io_num     = CONFIG_PIN_NUM_HSPI_MISO;
        hspiBusConfiguration.sclk_io_num     = CONFIG_PIN_NUM_HSPI_CLK;
        hspiBusConfiguration.max_transfer_sz = CONFIG_BUS_HSPI_MAX_TRANSFERSIZE;
        hspi_bus_handle = spi_bus_initialize(HSPI_HOST, &hspiBusConfiguration);
        if (res != ESP_OK) return res;
    #endif

    #ifdef CONFIG_BUS_I2C0_ENABLE
        i2c_config_t i2c0BusConfiguration = {0};
        i2c0BusConfiguration.mode             = I2C_MODE_MASTER;
        i2c0BusConfiguration.sda_io_num       = CONFIG_PIN_NUM_I2C0_DATA;
        i2c0BusConfiguration.scl_io_num       = CONFIG_PIN_NUM_I2C0_CLK;
        i2c0BusConfiguration.master.clk_speed = CONFIG_I2C0_MASTER_FREQ_HZ,
        #ifdef CONFIG_PIN_PULL_I2C0_DATA
            i2c0BusConfiguration.sda_pullup_en = GPIO_PULLUP_ENABLE,
        #else
            i2c0BusConfiguration.sda_pullup_en = GPIO_PULLUP_DISABLE,
        #endif
        #ifdef CONFIG_PIN_PULL_I2C0_CLK
            i2c0BusConfiguration.scl_pullup_en = GPIO_PULLUP_ENABLE,
        #else
            i2c0BusConfiguration.scl_pullup_en = GPIO_PULLUP_DISABLE,
        #endif

        i2c0_bus_handle = i2c_bus_create(I2C_NUM_0, &i2c0BusConfiguration);
    #endif

    #ifdef CONFIG_BUS_I2C1_ENABLE
        i2c_config_t i2c1BusConfiguration = {0};
        i2c1BusConfiguration.mode             = I2C_MODE_MASTER;
        i2c1BusConfiguration.sda_io_num       = CONFIG_PIN_NUM_I2C1_DATA;
        i2c1BusConfiguration.scl_io_num       = CONFIG_PIN_NUM_I2C1_CLK;
        i2c1BusConfiguration.master.clk_speed = CONFIG_I2C1_MASTER_FREQ_HZ,
        #ifdef CONFIG_PIN_PULL_I2C1_DATA
            i2c1BusConfiguration.sda_pullup_en = GPIO_PULLUP_ENABLE,
        #else
            i2c1BusConfiguration.sda_pullup_en = GPIO_PULLUP_DISABLE,
        #endif
        #ifdef CONFIG_PIN_PULL_I2C1_CLK
            i2c1BusConfiguration.scl_pullup_en = GPIO_PULLUP_ENABLE,
        #else
            i2cqBusConfiguration.scl_pullup_en = GPIO_PULLUP_DISABLE,
        #endif

        i2c1_bus_handle = i2c_bus_create(I2C_NUM_1, &i2c1BusConfiguration);
    #endif

    return ESP_OK;
}

spi_bus_handle_t get_vspi_bus() {
    return vspi_bus_handle;
}

spi_bus_handle_t get_hspi_bus() {
    return hspi_bus_handle;
}

spi_bus_handle_t get_spi_bus(int spi) {
    if (spi == VSPI_HOST) {
        return get_vspi_bus();
    } else if (spi == HSPI_HOST) {
        return get_hspi_bus();
    }
    return NULL;
}

i2c_bus_handle_t get_i2c0_bus() {
    return i2c0_bus_handle;
}

i2c_bus_handle_t get_i2c1_bus() {
    return i2c1_bus_handle;
}

i2c_bus_handle_t get_i2c_bus(int i2c) {
    if (i2c == I2C_NUM_0) {
        return get_i2c0_bus();
    } else if (i2c == I2C_NUM_1) {
        return get_i2c1_bus();
    }
    return NULL;
}