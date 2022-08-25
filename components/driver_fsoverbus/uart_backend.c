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

#define TAG "FSOVERBUS_UART"

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
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    uint16_t command = 0;
    uint32_t size = 0;
    uint32_t recv = 0;
    uint16_t verif = 0;

    

    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    ESP_LOGI(TAG, "siz: %d", event.size);
                    uart_read_bytes(CONFIG_DRIVER_FSOVERBUS_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    if (event.size != FSOB_PACKETSIZE) {
                        ESP_LOGE(TAG, "Received packet with incorrect size (%d)", event.size);
                        break;
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

                    
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGW(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(CONFIG_DRIVER_FSOVERBUS_UART_NUM);
                    xQueueReset(uart_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGW(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(CONFIG_DRIVER_FSOVERBUS_UART_NUM);
                    xQueueReset(uart_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}


void fsob_backend_init() {
    uart_param_config(CONFIG_DRIVER_FSOVERBUS_UART_NUM, &uart_config);   //Configure the uart hardware
    uart_set_pin(CONFIG_DRIVER_FSOVERBUS_UART_NUM, CONFIG_DRIVER_FSOVERBUS_UART_TX, CONFIG_DRIVER_FSOVERBUS_UART_RX, -1, -1); //Change pins
    uart_driver_install(CONFIG_DRIVER_FSOVERBUS_UART_NUM, CONFIG_DRIVER_FSOVERBUS_UART_BUFFER_SIZE, CONFIG_DRIVER_FSOVERBUS_UART_BUFFER_SIZE, 40, &uart_queue, 0); //Install driver
    xTaskCreatePinnedToCore(fsob_uart_task, "fsoverbus_uart", 8000, NULL, 100, NULL, 0);

}

void fsob_backend_reset() {
    receiving = 0;
}

void fsob_backend_write_packet(const void *data) {
    uart_write_bytes(CONFIG_DRIVER_FSOVERBUS_UART_NUM, data, FSOB_PACKETSIZE);
}

#endif