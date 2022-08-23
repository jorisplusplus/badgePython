#include <sdkconfig.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <esp_err.h>
#include <esp_log.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <esp_vfs.h>
#include <dirent.h>
#include <esp_intr_alloc.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"

#include "include/driver_fsoverbus.h"
#include "include/filefunctions.h"
#include "include/messageutils.h"
#include "include/specialfunctions.h"
#include "include/fsob_backend.h"
#include "include/appfsfunctions.h"
#include "include/functions.h"
#include "include/packethelper.h"

#define TAG "fsob"
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define CACHE_SIZE (2048)

TimerHandle_t timeout;

uint8_t buffer[CACHE_SIZE];
void fsob_timeout_function( TimerHandle_t xTimer );


//Function lookup tables

int (*specialfunction[SPECIALFUNCTIONSLEN])(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length);
int (*filefunction[FILEFUNCTIONSLEN])(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length);

static uint32_t s_message_id = 0;
static bool s_processing;

/**
 * @brief Returns if the system is currently processing a fsob command
 * 
 * @return true 
 * @return false 
 */
bool fsob_is_processing() {
    return s_processing;
}

void fsob_ingest_data(bool newpacket, const uint8_t *src, size_t datasize) {
    static uint16_t command = 0; //Message command id
    static uint32_t size = 0;  //Total message size
    static uint32_t recv = 0; //Total bytes received so far
    static uint32_t message_id; //Received message id
    static uint16_t verif = 0; //Verif field
    static uint8_t buf[1024];

    if (newpacket) {
        command = *((uint16_t *) &src[0]);
        size = *((uint32_t *) &src[2]);
        verif = *((uint16_t *) &src[6]);
        message_id = *((uint32_t *) &src[8]);
        recv = datasize-12;
        s_processing = true;
        ESP_LOGI(TAG, "New packet %d %d %d %d", command, message_id, size, verif);
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, src, datasize, ESP_LOG_INFO);
        fsob_handle_command(&src[12], command, message_id, size, recv, datasize-12);
    } else {
        recv += datasize;
        fsob_handle_command(src, command, message_id, size, recv, datasize);
    }
    if (recv == size) {
        s_processing = false;
    }
}

void fsob_write_bytes(bool newmessage, bool endmessage, const char *src, size_t datasize) {
    for (int i = 0; i < datasize; i+=64) {
        int payloadsize = min(64, datasize-i);
        int packet_id;
        fsobpacket_t *packet = fsob_packet_get(&packet_id);
        packet->header = 0xFAAF;
        packet->flags = 0;
        if (newmessage && i == 0) {
            packet->flags |= COMMAND_START;           
        }
        memcpy(packet->payload, &src[i], payloadsize);
        packet->flags |= (payloadsize << PAYLOAD_SIZE_OFFSET);
        if (endmessage && (i+64) > datasize) { //If end of message and last iteration
            packet->flags |= COMMAND_END;
        }
        ESP_LOGI(TAG, "Sending packet");
        fsob_packet_send(packet_id);
    }
}


void fsob_init()  {
    
}

void fsob_reset()  {
    s_processing = false;
    fsob_backend_reset();
    fsob_packet_reset();
}

void fsob_handle_command(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length) {
    static uint32_t write_pos;
    if(received == length) { //First data of the packet
        write_pos = 0;
    }
    
    if(length > 0) {
        memcpy(&buffer[write_pos], data, length);
        write_pos += length;
    }

    int return_val = 0;
    if(command < FILEFUNCTIONSBASE) {
        if(command < SPECIALFUNCTIONSLEN) {
            ESP_LOGI(TAG, "Invoking specialfuntion");
            return_val = specialfunction[command](buffer, command, message_id, size, received, length);
        }
    } else if(command < BADGEFUNCTIONSBASE) {
        if((command-FILEFUNCTIONSBASE) < FILEFUNCTIONSLEN) {
            ESP_LOGI(TAG, "Invoking filefunction");
            return_val = filefunction[command-FILEFUNCTIONSBASE](buffer, command, message_id, size, received, length);
        }
    }
    if(return_val) {    //Function has indicated that next payload should write at start of buffer.
        write_pos = 0;
    }
}

esp_err_t driver_fsoverbus_init(void) { 
    specialfunction[EXECFILE] = execfile;
    specialfunction[HEARTBEAT] = heartbeat;
    specialfunction[PYTHONSTDIN] = pythonstdin;
    
    filefunction[GETDIR] = getdir;
    filefunction[READFILE] = readfile;
    filefunction[WRITEFILE] = writefile;
    filefunction[DELFILE] = delfile;
    filefunction[DUPLFILE] = duplfile;
    filefunction[MVFILE] = mvfile;
    filefunction[MAKEDIR] = makedir;

    #if CONFIG_DRIVER_FSOVERBUS_APPFS_SUPPORT
    specialfunction[APPFSBOOT] = appfsboot;
    filefunction[APPFSDIR] = appfslist;
    filefunction[APPFSDEL] = appfsdel;
    filefunction[APPFSWRITE] = appfswrite;
    #else
    specialfunction[APPFSBOOT] = notsupported;
    filefunction[APPFSDIR] = notsupported;
    filefunction[APPFSDEL] = notsupported;
    filefunction[APPFSWRITE] = notsupported;
    #endif
        
    fsob_init();
    fsob_packet_init();
    fsob_backend_init();

    ESP_LOGI(TAG, "fs over bus registered.");
    return ESP_OK;
} 