#include "driver_fsoverbus.h"
#include "packethelper.h"
#include "fsob_backend.h"
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "rom/crc.h"

#define TAG "fsob_ph"

typedef enum {EMPTY = 0, WRITING, READY} packetstate; 

static uint32_t s_packet_id = 0;    //Packet ids being send by the badge
static uint32_t s_ack_id = 0;       //Packet id which have been process
static uint32_t s_last_ack = 0;     //Packet id which ack has been send

static fsobpacket_t *s_rx_packetbuf[CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT] = {0};
static fsobpacket_t s_tx_packetbuf[CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT] = {0};
static packetstate s_tx_valid[CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT] = {0};
static TickType_t s_tx_tick[CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT] = {0}; 
static int s_head = 0;
static SemaphoreHandle_t xSemaphore;
static TickType_t s_last_ack_time = 0;
static TaskHandle_t xTask = NULL;
static TimerHandle_t xTimer = NULL;
static QueueHandle_t ingestQueue;

static void fsob_packet_handle_ack(uint32_t ack_id);
static bool fsob_packet_buffer_push(fsobpacket_t *packet);
static void fsob_packet_resend(int packet_id);

static bool fsob_packet_buffer_push(fsobpacket_t *packet) {
    int32_t index = packet->packet_id - s_packet_id - 1;
    //check if packet is going to proceed within the inflight limit
    if (index < 0 || index >= CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT) {
        return false;
    }

    //Check if packet is already stored in cache
    for (int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        if (s_rx_packetbuf[i] != NULL) {
            if(s_rx_packetbuf[i]->packet_id == packet->packet_id) {
                return true;
            }
        }
    }
    
    //Copy packet to first available slot
    for (int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        if (s_rx_packetbuf[i] == NULL) {
            s_rx_packetbuf[i] = malloc(sizeof(fsobpacket_t));
            memcpy(s_rx_packetbuf[i], packet, sizeof(fsobpacket_t));
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if packet in receive buffer is ready to be processed and remove any packets that have already been acked
 * 
 * @return int index of the packet in the buffer, returns -1 if no packet buffered is ready
 */
static int fsob_packet_buffer_check() {
    for (int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        if (s_rx_packetbuf[i] != NULL) {
            if (s_rx_packetbuf[i]->packet_id <= s_ack_id) { //Already processed packet detected, remove from buffer
                free(s_rx_packetbuf[i]);
                s_rx_packetbuf[i] = NULL;
            } else if(s_rx_packetbuf[i]->packet_id == (s_ack_id + 1)) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * @brief Checks if the crc of the packet is correct
 * 
 * @param packet packet to be checked
 * @return true - crc correct
 * @return false - crc incorrect
 */
bool fsob_packet_verify_crc(fsobpacket_t *packet) {
    uint32_t packetcrc = packet->crc;
    packet->crc = 0;
    uint32_t crcval = crc32_le(0, (uint8_t *) packet, FSOB_PACKETSIZE);
    bool result = (crcval == packetcrc);
    packet->crc = packetcrc;
    return result;
}

/**
 * @brief Task loop that processes incoming packets and performs periodic checks
 * 
 * @param args 
 */
void fsob_packet_loop(void *args) {
    for( ;; ) {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        
        //Process all packets in queue
        BaseType_t res = pdTRUE;
        while (res == pdTRUE) {
            fsobpacket_t packet;
            res = xQueueReceive(ingestQueue, &packet, 0);
            if (res == pdTRUE) {
                fsob_packet_handler(&packet);
            }
        }

        //Check if tx packets are timed out
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        for (int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
            if(s_tx_valid[i] == READY && (xTaskGetTickCount() - s_tx_tick[i]) > pdMS_TO_TICKS(600)) {
                fsob_packet_resend(i);
            }
        }
        xSemaphoreGive(xSemaphore);


    }
}

/**
 * @brief Queue packet to be processed and activates the processing task
 * 
 * @param packet Packet to be ingested
 */
void fsob_packet_ingest(fsobpacket_t *packet) {
    xQueueSend(ingestQueue, packet, portMAX_DELAY);
    xTaskNotify(xTask, 0, eNoAction);
}

/**
 * @brief Handled packet
 * 
 * @param packet 
 */
void fsob_packet_handler(fsobpacket_t *packet) {
    if (packet->packet_id == 0) {    //Nack packet
        if (packet->flags & RESET) {
            ESP_LOGI(TAG, "Resetting");
            fsob_reset();
        }
        
        ESP_LOGI(TAG, "Handling ack %d", packet->ack_id);
        fsob_packet_handle_ack(packet->ack_id);
        return;
    }
    if (packet->packet_id < s_ack_id) {    //Already processed packet
        ESP_LOGW(TAG, "Dropped duplicate packet");
        return;
    }
    
    if (packet->packet_id == s_ack_id + 1) {    //Packet has been received after 
        //Should process packet here
        ESP_LOGI(TAG, "Ingesting packet %d %d %d %d", packet->packet_id, packet->flags & COMMAND_START, packet->flags & COMMAND_END, (packet->flags & PAYLOAD_SIZE) >> PAYLOAD_SIZE_OFFSET);
        s_ack_id = packet->packet_id;
        ESP_LOGI(TAG, "ACK ID %d", s_ack_id);
        fsob_packet_handle_ack(packet->ack_id);
        //Check if any cached packets are available
        fsob_ingest_data(packet->flags & COMMAND_START, packet->payload, (packet->flags & PAYLOAD_SIZE) >> PAYLOAD_SIZE_OFFSET);
        int res = fsob_packet_buffer_check();
        while(res != -1) {
            fsobpacket_t *cached_packet = s_rx_packetbuf[res];
            s_ack_id = cached_packet->packet_id;
            fsob_packet_handle_ack(packet->ack_id);
            fsob_ingest_data(cached_packet->flags & COMMAND_START, cached_packet->payload, (cached_packet->flags & PAYLOAD_SIZE) >> PAYLOAD_SIZE_OFFSET);
            free(s_rx_packetbuf[res]);
            s_rx_packetbuf[res] = NULL;
            res = fsob_packet_buffer_check();
        }
        ESP_LOGI(TAG, "ACK ID %d", s_ack_id);    
        return;
    }

    if (fsob_packet_buffer_push(packet)) {
        ESP_LOGI(TAG, "Cached packet");
    } else {
        ESP_LOGI(TAG, "Dropped packet");
    }

    //Send ack packet if half of max inflight packets have not been acked
    if ((s_ack_id - s_last_ack) > (CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT / 2)) {
        int packet_id; 
        fsobpacket_t* packet = fsob_packet_get_nonblocking(&packet_id);
        if (packet != NULL) {
            packet->flags = 0;
            fsob_packet_send(packet_id);
        }
    }
}

static void fsob_packet_handle_ack(uint32_t ack_id) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    int next_head = s_head;
    for(int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        int index = (i + s_head) % CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT;
        if (s_tx_valid[index] == READY) {
            if(ack_id >= s_tx_packetbuf[index].packet_id) {
                s_tx_valid[index] = EMPTY;
                next_head = (index+1) % CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; //Update head position to next no acked packet
            } else {
                break;
            }
        }
    }
    s_head = next_head;
    xSemaphoreGive(xSemaphore);
}

fsobpacket_t* fsob_packet_get(int *packet_id) {
    while (1) {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        for(int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
            int index = (i + s_head) % CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT;
            if (s_tx_valid[index] == EMPTY) {
                *packet_id = index;
                s_tx_valid[index] = WRITING;
                s_packet_id += 1;
                s_tx_packetbuf[index].packet_id = s_packet_id;
                xSemaphoreGive(xSemaphore);
                return &s_tx_packetbuf[index];
            }
        }
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1);
    }
}

fsobpacket_t* fsob_packet_get_nonblocking(int *packet_id) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        int index = (i + s_head) % CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT;
        if (s_tx_valid[index] == EMPTY) {
            *packet_id = index;
            s_tx_valid[index] = WRITING;
            s_packet_id += 1;
            s_tx_packetbuf[index].packet_id = s_packet_id;
            xSemaphoreGive(xSemaphore);
            return &s_tx_packetbuf[index];
        }
    }
    xSemaphoreGive(xSemaphore);
    return NULL;    
}

void fsob_packet_send(int packet_id) {
    if (packet_id == -1) {
        ESP_LOGE(TAG, "Invalid packet id");
        return;
    }
    if (s_tx_valid[packet_id] != WRITING) {
        ESP_LOGE(TAG, "Packet not in right state send");
    }
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    ESP_LOGI(TAG, "Preparing transmission id: %d flags: %d, ack: %d", s_tx_packetbuf[packet_id].packet_id, s_tx_packetbuf[packet_id].flags, s_ack_id);
    s_tx_packetbuf[packet_id].ack_id = s_ack_id;
    s_tx_packetbuf[packet_id].crc = 0;
    s_tx_packetbuf[packet_id].crc = crc32_le(0, (uint8_t *) &s_tx_packetbuf[packet_id], sizeof(fsobpacket_t));
    s_tx_valid[packet_id] = READY;
    s_tx_tick[packet_id] = xTaskGetTickCount();    
    s_last_ack = s_ack_id;
    ESP_LOGI(TAG, "Writing to backend");
    fsob_backend_write_packet(&s_tx_packetbuf[packet_id]);
    xSemaphoreGive(xSemaphore);
}

static void fsob_packet_resend(int packet_id) {
    if (packet_id == -1) {
        ESP_LOGE(TAG, "Invalid packet id");
        return;
    }
    if (s_tx_valid[packet_id] != READY) {
        ESP_LOGE(TAG, "Packet not in right state resend");
    }
    ESP_LOGI(TAG, "Preparing retransmission id: %d, ack: %d", s_tx_packetbuf[packet_id].packet_id, s_ack_id);
    s_tx_packetbuf[packet_id].ack_id = s_ack_id;
    s_tx_packetbuf[packet_id].crc = 0;
    s_tx_packetbuf[packet_id].crc = crc32_le(0, (uint8_t *) &s_tx_packetbuf[packet_id], sizeof(fsobpacket_t));
    s_tx_valid[packet_id] = READY;
    s_tx_tick[packet_id] = xTaskGetTickCount();    
    s_last_ack = s_ack_id;
    ESP_LOGI(TAG, "Writing to backend");
    fsob_backend_write_packet(&s_tx_packetbuf[packet_id]);
}

void fsob_packet_reset() {
    s_packet_id = 0;
    s_ack_id = 0;
    s_last_ack = 0;
    s_last_ack_time = xTaskGetTickCount();
    
    for (int i = 0; i < CONFIG_DRIVER_FSOVERBUS_PACKETS_INFLIGHT; i++) {
        if (s_rx_packetbuf[i] != NULL) {
            free(s_rx_packetbuf[i]);
            s_tx_valid[i] = EMPTY;
        }
    }
}

void fsob_check(TimerHandle_t timer) {
    xTaskNotify(xTask, 0, eNoAction);
}

void fsob_packet_init() {
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);
    xTaskCreate(fsob_packet_loop, "Packet handler", 16000, NULL, 10, &xTask);
    ingestQueue = xQueueCreate(10, sizeof(fsobpacket_t));
    xTimer = xTimerCreate("Periodic FSOB check", pdMS_TO_TICKS(100), pdTRUE, (void *) 0, fsob_check);
    xTimerStart(xTimer, 0);
}

