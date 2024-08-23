#include "include/platform.h"
#include "include/platform_gen.h"
#include "include/factory_reset.h"
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
//#include "display.h"

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
  //display_init();

  //Init generated modules
  platform_gen_init();
  
	fflush(stdout);
	vTaskDelay(100 / portTICK_PERIOD_MS); //Give things time to settle.

	 int magic = get_magic();
   if (magic == MAGIC_FACTORY_RESET) {
    factory_reset();
    esp_restart();
   }
}

