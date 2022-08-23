#ifndef __MESSAGEUTILS_H__
#define __MESSAGEUTILS_H__

#include <stdint.h>

#define RD_BUF_SIZE (512)

#define PACKET_HEADER_SIZE (12)

void createMessageHeader(uint8_t *header, uint16_t command, uint32_t size, uint32_t message_id);
void sendok(uint16_t command, uint32_t message_id);
void sender(uint16_t command, uint32_t message_id);
void sendte(uint16_t command, uint32_t message_id);
void sendto(uint16_t command, uint32_t message_id);
void sendns(uint16_t command, uint32_t message_id);
void buildfile(char *source, char *target);

#endif