#include "include/platform.h"
#include "include/platform_gen.h"
#include "include/factory_reset.h"
#include "driver_framebuffer.h"
#include <driver/gpio.h>
#include <esp_event.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <spi_flash_mmap.h>
#include <esp_partition.h>
#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <esp_log.h>
#include "buses.h"
#include <driver/uart.h>
#include "driver_display.h"

#define TAG "platform"

esp_err_t isr_init() {
  esp_err_t res = gpio_install_isr_service(0);
  if (res == ESP_FAIL) {
    ESP_LOGW(TAG, "Failed to install gpio isr service. Ignoring this.");
    res = ESP_OK;
  }
  if (res != ESP_OK) {
    ESP_LOGE(TAG, "Failed to install gpio isr service: %d", res);
  }
  return res;
}

bool fbReady = false;

void fatal_error(const char *message) {
  printf("A fatal error occurred while initializing the driver for '%s'.\n", message);
  if (fbReady) {
#ifdef CONFIG_DRIVER_FRAMEBUFFER_ENABLE
#if defined(CONFIG_DRIVER_EINK_ENABLE) || defined(CONFIG_DRIVER_ILI9341_ENABLE)
    driver_framebuffer_fill(NULL, COLOR_WHITE);
    uint16_t y =
        driver_framebuffer_print(NULL, "Fatal error\n", 0, 0, 1, 1, COLOR_BLACK, &roboto_12pt7b);
    y = driver_framebuffer_print(NULL, "Failure while starting driver.\n", 0, y, 1, 1, COLOR_BLACK,
                                 &roboto_12pt7b);
    y = driver_framebuffer_print(NULL, message, 0, y, 1, 1, COLOR_BLACK, &roboto_12pt7b);
    y = driver_framebuffer_print(NULL, "\n\nRestarting in 10 seconds...\n", 0, y, 1, 1, COLOR_BLACK,
                                 &roboto_12pt7b);
    driver_framebuffer_flush(0);
#endif
#if defined(CONFIG_DRIVER_SSD1306_ENABLE) || defined(CONFIG_DRIVER_ERC12846_ENABLE)
    driver_framebuffer_fill(NULL, COLOR_BLACK);
    uint16_t y =
        driver_framebuffer_print(NULL, "Fatal error\n", 0, 0, 2, 2, COLOR_WHITE, &ipane7x5);
    y = driver_framebuffer_print(NULL, message, 0, y + 5, 1, 1, COLOR_WHITE, &ipane7x5);
    driver_framebuffer_flush(0);
#endif
#endif
  }
  restart();
}

void platform_init()
{  	
	logo();
	bool is_first_boot = nvs_init();

	fflush(stdout);

	if (isr_init() != ESP_OK) restart();
  //Static inits can be performed here
  start_buses();
  driver_display_init();

  //Init generated modules
  platform_gen_init();
  
	fflush(stdout);
	vTaskDelay(100 / portTICK_PERIOD_MS); //Give things time to settle.

  // size_t mp_task_heap_size = mp_preallocate_heap();
    // ESP_LOGI(TAG, "Heap size: %d", mp_task_heap_size);

// #ifndef CONFIG_FW_DISABLE_OTA_AND_FIRSTBOOT
// 	if (is_first_boot) {
// 		#ifdef CONFIG_DRIVER_FRAMEBUFFER_ENABLE
// 			driver_framebuffer_fill(NULL, COLOR_BLACK);
// 			driver_framebuffer_print(NULL, "Extracting ZIP...\n", 0, 0, 1, 1, COLOR_WHITE, &roboto_12pt7b);
// 			driver_framebuffer_flush(0);
// 		#endif
// 		printf("Attempting to unpack FAT initialization ZIP file...\b");
// 		if (unpack_first_boot_zip() != ESP_OK) { //Error
// 			#ifdef CONFIG_DRIVER_FRAMEBUFFER_ENABLE
// 				driver_framebuffer_fill(NULL, COLOR_BLACK);
// 				driver_framebuffer_print(NULL, "ZIP error!\n", 0, 0, 1, 1, COLOR_WHITE, &roboto_12pt7b);
// 				driver_framebuffer_flush(0);
// 			#endif
// 			printf("An error occured while unpacking the ZIP file!");
// 			nvs_write_zip_status(false);
// 		} else {
// 			nvs_write_zip_status(true);
// 		}
// 		esp_restart();
// 	}
// #endif

	 int magic = get_magic();
   if (magic == MAGIC_FACTORY_RESET) {
    factory_reset();
    esp_restart();
   }
}

