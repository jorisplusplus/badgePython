#include "display.h"
#include "buses.h"
#include "esp_log.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"




#define TAG "DISPLAY"

static esp_lcd_panel_io_handle_t io_handle = NULL;
static  esp_lcd_panel_handle_t panel_handle = NULL;

esp_err_t display_set_backlight(uint8_t brightness) {
    return ESP_OK;
}

esp_err_t display_write_partial(const uint8_t *buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    esp_lcd_panel_draw_bitmap(panel_handle, x0, y0, x1, y1, buffer);
    return ESP_OK;
}

#if CONFIG_DISPLAY_ENABLE
esp_err_t display_init() {
    #if LCDI2C
        esp_lcd_panel_io_i2c_config_t io_config = BUSCONFIG();
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_NUM_0, &io_config, &io_handle));
    #elif LCDSPI
        esp_lcd_panel_io_spi_config_t io_config = BUSCONFIG();
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t) SPI2_HOST, &io_config, &io_handle));
    #else
        return;
    #endif
    esp_lcd_panel_dev_config_t panel_config = PANELCONFIG();
    ESP_ERROR_CHECK(LCDINIT(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    esp_rom_gpio_pad_select_gpio(39);
    gpio_set_direction(39, GPIO_MODE_OUTPUT);
    gpio_set_level(39, 1);
    ESP_LOGI(TAG, "Init display done");
    return ESP_OK;
}
#else
void display_init() {}
#endif