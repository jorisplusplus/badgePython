#ifndef DRIVER_FSOVERBUS_H
#define DRIVER_FSOVERBUS_H

#define FSOB_PACKETSIZE (80)
#define COMMAND_START_OFFSET (0)
#define COMMAND_END_OFFSET (1)
#define RESET_OFFSET (2)
#define PAYLOAD_SIZE_OFFSET (9)

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>

typedef struct __attribute__((__packed__)) {
    uint16_t header;
    uint16_t flags;
    uint32_t crc;
    uint32_t packet_id;
    uint32_t ack_id;
    uint8_t payload[64];
} fsobpacket_t;

enum FSOB_FLAGMASK {
    COMMAND_START = (1 << COMMAND_START_OFFSET),
    COMMAND_END = (1 << COMMAND_END_OFFSET),
    RESET = (1 << RESET_OFFSET),
    PAYLOAD_SIZE = (0x7F << PAYLOAD_SIZE_OFFSET)
};

//static_assert(FSOB_PACKETSIZE == sizeof(fsobpacket_t));

esp_err_t driver_fsoverbus_init(void);

void fsob_ingest_data(bool newpacket, const uint8_t *src, size_t datasize);
void fsob_handle_command(uint8_t *data, uint16_t command, uint32_t message_id, uint32_t size, uint32_t received, uint32_t length);
void fsob_receive_bytes(uint8_t *data, size_t len);
void fsob_write_bytes(bool newmessage, bool endmessage, const char *src, size_t datasize);
void fsob_reset();

#endif