#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_flash.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "include/factory_reset.h"

#include "esp_partition.h"
#include "esp_spi_flash.h"

#define TAG "factory-reset"


static void __attribute__((noreturn)) task_fatal_error(void)
{
	ESP_LOGE(TAG, "Exiting task due to fatal error...");
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	esp_restart();
	(void)vTaskDelete(NULL);
}

static void factory_reset_task(void *pvParameter)
{
	esp_err_t err;

	vTaskDelay(2000 / portTICK_PERIOD_MS);

	ESP_LOGW(TAG, "Prepare to restart system!");
	esp_restart();
	return;
}

void factory_reset() {
	xTaskCreatePinnedToCore(&factory_reset_task, "factory_reset_task", 8192, NULL, 3, NULL, 0);
}

