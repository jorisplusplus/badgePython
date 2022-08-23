#ifndef __PACKETHELPER_H__
#define __PACKETHELPER_H__

#include "driver_fsoverbus.h"

enum packet_result {
    PROCESS_PACKET = 0,
    HOLD_PACKET,
    REJECT_PACKET
};

bool fsob_packet_verify_crc(fsobpacket_t *packet);
void fsob_packet_handler(fsobpacket_t *packet);
fsobpacket_t* fsob_packet_get(int *packet_id);
fsobpacket_t* fsob_packet_get_nonblocking(int *packet_id);
void fsob_packet_send(int packet_id);
void fsob_packet_reset();
void fsob_packet_init();


#endif