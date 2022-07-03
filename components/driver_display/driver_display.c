#include "screen_driver.h"
#include "driver_display.h"
#include "buses.h"
#include "esp_log.h"

#define TAG "DISPLAY"

static scr_driver_t lcd;
static scr_info_t lcd_info;

esp_err_t driver_display_set_backlight(uint8_t brightness) {
    return ESP_OK;
}

esp_err_t driver_display_write_partial(const uint8_t *buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    x0 = (x0/8)*8;
    y0 = (y0/8)*8;
    int width = x1-x0+1;
    width = ((width + 7)/8)*8;
    int height = y1-y0+1;
    height = ((height + 7)/8)*8;
    ESP_LOGW(TAG, "%d %d", width, height);
    lcd.draw_bitmap(x0, y0, width, height, buffer);
    return ESP_OK;
}

esp_err_t generate_interface(scr_interface_driver_t **iface_drv) {
#ifdef CONFIG_DISPLAY_SPI
    spi_bus_handle_t spi_bus = get_vspi_bus();
    scr_interface_spi_config_t iface_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = CONFIG_DISPLAY_PIN_CS,
        .pin_num_dc = CONFIG_DISPLAY_PIN_DC,
        .clk_freq = 20000000,
        .swap_data = true,
    };
    return scr_interface_create(SCREEN_IFACE_SPI, &iface_cfg, iface_drv);
#else
    i2c_bus_handle_t i2c_bus = get_i2c0_bus();
    scr_interface_i2c_config_t iface_cfg = {
        .i2c_bus = i2c_bus,
        .clk_speed = 100000,
        .slave_addr = CONFIG_DISPLAY_I2C_ADDR
    };
    return scr_interface_create(SCREEN_IFACE_I2C, &iface_cfg, iface_drv);
#endif
}

void driver_display_init() {
    #ifdef CONFIG_DRIVER_DISPLAY_ENABLE
    scr_interface_driver_t *iface_drv;
    generate_interface(&iface_drv);
    scr_controller_config_t lcd_cfg = {0};
    lcd_cfg.interface_drv = iface_drv,
    lcd_cfg.pin_num_rst = -1,
    lcd_cfg.pin_num_bckl = -1,
    lcd_cfg.rst_active_level = 0,
    lcd_cfg.bckl_active_level = 1,
    lcd_cfg.width = CONFIG_DISPLAY_WIDTH;
    lcd_cfg.height = CONFIG_DISPLAY_HEIGHT;
    lcd_cfg.rotate = SCR_DIR_LRTB;
    lcd_cfg.offset_hor = CONFIG_DISPLAY_HORIZONTAL_OFFSET;
    lcd_cfg.offset_ver = CONFIG_DISPLAY_VERTICAL_OFFSET;

    if (ESP_OK != scr_find_driver(CONFIG_DISPLAY_TYPE, &lcd)) {
        ESP_LOGE(TAG, "Failed to find driver.");
        return;
    }
    if (ESP_OK != lcd.init(&lcd_cfg)) {
        ESP_LOGE(TAG, "Failed to init lcd.");
        return;
    }
    if (ESP_OK != lcd.get_info(&lcd_info)) {
        ESP_LOGE(TAG, "Failed to get info.");
        return;
    }
    #endif
    ESP_LOGI(TAG, "Screen name:%s | width:%d | height:%d", lcd_info.name, lcd_info.width, lcd_info.height);
}