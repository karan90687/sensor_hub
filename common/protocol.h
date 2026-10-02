#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>

// frame format
typedef struct packet_t{
    uint8_t sync_byte;
    uint8_t flag;      // 4 msb bits will be sensor type and rest 4 will be flags
    int16_t temprature;
    uint32_t device_id;
    uint32_t timestamp;
    uint8_t checksum;
} frame;

#endif