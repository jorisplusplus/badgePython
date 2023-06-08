#include <sdkconfig.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <esp_log.h>
#include <esp_err.h>

#include "driver/spi_master.h"
#include "driver/i2c.h"

#include "include/buses.h"

#define I2C_MASTER_TX_BUF_DISABLE  0
#define I2C_MASTER_RX_BUF_DISABLE  0
#define WRITE_BIT      I2C_MASTER_WRITE // I2C master write
#define READ_BIT       I2C_MASTER_READ  // I2C master read
#define ACK_CHECK_EN   0x1     // I2C master will check ack from slave
#define ACK_CHECK_DIS  0x0     // I2C master will not check ack from slave
#define ACK_VAL        0x0     // I2C ack value
#define NACK_VAL       0x1     // I2C nack value

#define TAG "buses"

esp_err_t start_buses() {
    // This function initializes the VSPI, HSPI and I2C buses of the ESP32
    #if CONFIG_BUS_SPI2_ENABLE
        spi_bus_config_t spi2BusConfiguration = {0};
        spi2BusConfiguration.mosi_io_num     = CONFIG_PIN_NUM_SPI2_MOSI;
        spi2BusConfiguration.miso_io_num     = CONFIG_PIN_NUM_SPI2_MISO;
        spi2BusConfiguration.sclk_io_num     = CONFIG_PIN_NUM_SPI2_CLK;
        spi2BusConfiguration.max_transfer_sz = CONFIG_BUS_SPI2_MAX_TRANSFERSIZE;
        ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &spi2BusConfiguration, SPI_DMA_CH_AUTO));
    #endif

    #if CONFIG_BUS_SPI3_ENABLE
        spi_bus_config_t spi3BusConfiguration = {0};
        spi3BusConfiguration.mosi_io_num     = CONFIG_PIN_NUM_SPI3_MOSI;
        spi3BusConfiguration.miso_io_num     = CONFIG_PIN_NUM_SPI3_MISO;
        spi3BusConfiguration.sclk_io_num     = CONFIG_PIN_NUM_SPI3_CLK;
        spi3BusConfiguration.max_transfer_sz = CONFIG_BUS_SPI3_MAX_TRANSFERSIZE;
        ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &spi3BusConfiguration, SPI_DMA_CH_AUTO));
    #endif

    #if CONFIG_BUS_I2C0_ENABLE
        ESP_LOGI(TAG, "Init I2C0");
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
        i2c_param_config(I2C_NUM_0, &i2c0BusConfiguration);
        i2c_driver_install(I2C_NUM_0, i2c0BusConfiguration.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    #endif

    #if CONFIG_BUS_I2C1_ENABLE
        ESP_LOGI(TAG, "Init I2C1");
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

        i2c_param_config(I2C_NUM_1, &i2c0BusConfiguration);
        i2c_driver_install(I2C_NUM_1, i2c0BusConfiguration.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    #endif

    return ESP_OK;
}