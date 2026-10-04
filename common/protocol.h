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

typedef enum {
    GOOD,
    BAD
} checksum;

// packet decoder function checks the packet inegrity
checksum packet_decoder(struct packet_t *p, int len);

// checksum generator 
uint8_t checksum_gen(uint32_t timestamp);

#endif