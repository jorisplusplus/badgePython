#include "include/fsob_backend.h"
#include "include/driver_fsoverbus.h"
#include "include/packethelper.h"
#include <driver/gpio.h>
#include <driver/uart.h>
#include <soc/uart_reg.h>
#include <esp_err.h>
#include <esp_log.h>

/*
* This is the uart backend implementation used in CZ20-badge
*/
#if (CONFIG_DRIVER_FSOVERBUS_BACKEND == 0)

#define TAG "fsob_uart"

#define UART_EMPTY_THRESH_DEFAULT  (10)
#define UART_FULL_THRESH_DEFAULT  (120)
#define UART_TOUT_THRESH_DEFAULT   (10)
#define UART_CLKDIV_FRAG_BIT_WIDTH  (3)
#define UART_TOUT_REF_FACTOR_DEFAULT (UART_CLK_FREQ/(REF_CLK_FREQ<<UART_CLKDIV_FRAG_BIT_WIDTH))
#define UART_TX_IDLE_NUM_DEFAULT   (0)
#define UART_PATTERN_DET_QLEN_DEFAULT (10)
#define UART_MIN_WAKEUP_THRESH      (2)

uart_config_t uart_config = {
    .baud_rate = 921600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 32,
    };

QueueHandle_t uart_queue;
int receiving = 0;
uint32_t message_id = 0;

void fsob_uart_task(void *pvParameter);

uint32_t min(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}

void fsob_uart_task(void *pvParameter) {
    ESP_LOGI(TAG, "Uart backend started");
    bool corruption_detect = false;
    uint8_t* dtmp = (uint8_t*) malloc(FSOB_PACKETSIZE);
    for(;;) {
        int length = 0;
        ESP_ERROR_CHECK(uart_get_buffered_data_len(CONFIG_DRIVER_FSOVERBUS_UART_NUM, (size_t*)&length));
        if (length >= 80) {
            corruption_detect = false;
            length = uart_read_bytes(CONFIG_DRIVER_FSOVERBUS_UART_NUM, dtmp, 80, 100);
            if (length != FSOB_PACKETSIZE) {
                ESP_LOGE(TAG, "Received packet with incorrect size (%d)", length);
                continue;
            }
            fsobpacket_t *packet = (fsobpacket_t *) dtmp;
            if (packet->header != 0xAFFA) {
                ESP_LOGE(TAG, "Error in fsob packet header");
                break;
            }

            if(fsob_packet_verify_crc(packet)) {
                fsob_packet_handler(packet);
            } else {
                ESP_LOGE(TAG, "CRC Failed");
            }

        } else if(length > 0) {
            if (corruption_detect) {
                ESP_LOGE(TAG, "Corruption detected in uart queue");
                uart_flush(CONFIG_DRIVER_FSOVERBUS_UART_NUM);
                corruption_detect = false;
            } else {
                corruption_detect = true;
            }
        }
        vTaskDelay(1);        
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}


void fsob_backend_init() {
    uart_param_config(CONFIG_DRIVER_FSOVERBUS_UART_NUM, &uart_config);   //Configure the uart hardware
    uart_set_pin(CONFIG_DRIVER_FSOVERBUS_UART_NUM, CONFIG_DRIVER_FSOVERBUS_UART_TX, CONFIG_DRIVER_FSOVERBUS_UART_RX, -1, -1); //Change pins
    uart_driver_install(CONFIG_DRIVER_FSOVERBUS_UART_NUM, CONFIG_DRIVER_FSOVERBUS_UART_BUFFER_SIZE, CONFIG_DRIVER_FSOVERBUS_UART_BUFFER_SIZE, 0, NULL, 0); //Install driver
    xTaskCreatePinnedToCore(fsob_uart_task, "fsoverbus_uart", 8192, NULL, 100, NULL, 0);

}

void fsob_backend_reset() {
    receiving = 0;
}

void fsob_backend_write_packet(const void *data) {
    uart_write_bytes(CONFIG_DRIVER_FSOVERBUS_UART_NUM, data, FSOB_PACKETSIZE);
}

#endif