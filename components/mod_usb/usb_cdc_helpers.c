#include "tinyusb.h"
#include "tusb_cdc_acm.h"

bool is_usb_cdc_connected(void) {
  return tud_cdc_connected();
}

bool usb_disconnect(void) {
  return tud_disconnect();
}