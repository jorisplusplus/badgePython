#include <stdbool.h>
#include <stdint.h>

#include "sdkconfig.h"
#include "esp_system.h"

#include <esp_log.h>
#include <esp_err.h>

#include "rom/usb/chip_usb_dw_wrapper.h"
#include "rom/usb/usb_persist.h"
#include "soc/rtc_cntl_reg.h"
#include "tinyusb.h"

#include "include/bootloader.h"
#include "rom/ets_sys.h"
#include "rom/usb/usb_dc.h"

const char *TAG = "modbootloader";

typedef enum {
  REBOOT_NORMAL,
  REBOOT_BOOTLOADER,
  REBOOT_BOOTLOADER_DFU,
} reboot_type_t;

reboot_type_t next_reboot = REBOOT_NORMAL;

void reboot_into_bootloader(bool dfu) {
  next_reboot = dfu ? REBOOT_BOOTLOADER_DFU : REBOOT_BOOTLOADER;
  esp_restart();
}

/* This function gets registered as a restart handler.
 * Prepares USB peripheral for restart and sets up persistence.
 * Non-static to allow placement into IRAM by ldgen.
 */
void bootloader_shutdown_handler(void)
{
  if (next_reboot == REBOOT_BOOTLOADER) {
    chip_usb_set_persist_flags(0);
    REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);

    // Disconnect USB peripheral from host to allow bootloader to create new one
    tud_disconnect();

    // Wait to make sure the USB peripheral is disconnected
    ets_delay_us(1 * 1000 * 1000);
  } else if (next_reboot == REBOOT_BOOTLOADER_DFU) {
    chip_usb_set_persist_flags(USBDC_BOOT_DFU);
    REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);
  } else {
    usb_dc_prepare_persist();
    chip_usb_set_persist_flags(USBDC_PERSIST_ENA);
    REG_WRITE(RTC_CNTL_OPTION1_REG, 0);
  }
}

esp_err_t bootloader_init(void) {
  ESP_LOGI(TAG, "DWC USB Persist: %d", chip_usb_dw_did_persist());

  // Disable bootloader mode on reboot by default
  REG_WRITE(RTC_CNTL_OPTION1_REG, 0);

  esp_err_t err;
  err = esp_register_shutdown_handler(bootloader_shutdown_handler);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Couldn't install shutdown handler: %s", esp_err_to_name(err));
    return ESP_FAIL;
  }

  return ESP_OK;
}
