#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include "esp_system.h"

#include <esp_log.h>
#include <esp_err.h>

#include "rom/usb/usb_device.h"
#include "rom/usb/chip_usb_dw_wrapper.h"
#include "rom/usb/usb_persist.h"
#include "soc/usb_struct.h"
#include "soc/rtc_cntl_reg.h"

#include "include/bootloader.h"
#include "rom/ets_sys.h"

#define DWC2_FS_PERIPH_BASE  0x60080000UL
#define DWC2 ((dwc2_regs_t*)(DWC2_FS_PERIPH_BASE))

#define GRSTCTL_CSRST_Pos                (0U)
#define GRSTCTL_CSRST_Msk                (0x1UL << GRSTCTL_CSRST_Pos)             // 0x00000001 */
#define GRSTCTL_CSRST                    GRSTCTL_CSRST_Msk                        // Core soft reset          */

#define TU_BIT(n)                       (1UL << (n))
#define PCGCTL_RSTPDWNMODULE            TU_BIT(3)
#define PCGCTL_PWRCLMP                  TU_BIT(2)
#define PCGCTL_GATEHCLK                 TU_BIT(1)
#define PCGCTL_STOPPCLK                 TU_BIT(0)

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
//  if (next_reboot == REBOOT_BOOTLOADER) {
    chip_usb_set_persist_flags(0);
    REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);

    // stop PHY clock
    DWC2->pcgctl &= (PCGCTL_STOPPCLK | PCGCTL_GATEHCLK | PCGCTL_PWRCLMP | PCGCTL_RSTPDWNMODULE);

    ets_delay_us(1000 * 1000);
//
//  } else if (next_reboot == REBOOT_BOOTLOADER_DFU) {
//    chip_usb_set_persist_flags(USBDC_BOOT_DFU);
//    REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);
//  } else {
//    chip_usb_set_persist_flags(USBDC_PERSIST_ENA);
//    REG_WRITE(RTC_CNTL_OPTION1_REG, 0);
//  }
}

esp_err_t bootloader_init(void) {
  esp_err_t err;
  err = esp_register_shutdown_handler(bootloader_shutdown_handler);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Couldn't install shutdown handler: %s", esp_err_to_name(err));
    return ESP_FAIL;
  }

  return ESP_OK;
}
