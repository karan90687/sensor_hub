#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>

// frame format

// flag byte:  7 6 5 4     | 3 2 |     1 0
//             sensor type  flags      unused

typedef struct packet_t{
    uint8_t sync_byte;
    uint8_t flag;      // 4 msb bits will be sensor type and rest 4 will be flags
    int16_t temprature;
    uint32_t device_id;
    uint32_t timestamp;
    uint8_t checksum;
} frame;

typedef enum {
    GOOD_BUFFER,
    SHORT_BUFFER,
    BAD_SYNC_BYTE,
    FILLED_UNUSED_BITS,
    BAD_CHECKSUM
} buffer_type;

// packet decoder function checks the packet inegrity
buffer_type packet_decoder(uint8_t *buffer, size_t len);

// checksum generator 
uint8_t checksum_gen(uint8_t *packet);

#endif