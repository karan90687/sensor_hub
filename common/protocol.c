#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"
#include <stdbool.h>


uint8_t checksum_gen(uint8_t *packet, size_t len){
    uint8_t checksum = 0;
        for (size_t i = 0; i < (len-1); i++)
    {
        checksum += packet[i];
        // printf("byte %zu value %x \n",i,packet[i]);
    }
    // printf("checksum byte is %x \n and in dec %d \n",checksum,checksum);
    return checksum;
}

buffer_type packet_decoder(uint8_t *buffer, size_t len)
{
    // Check length
    if (len != 13) {
        return SHORT_BUFFER;
    }

    // Check sync byte
    if (buffer[0] != 0xAA) {
        return BAD_SYNC_BYTE;
    }

    // unused bits set or not
    bool unused_bit = ((buffer[1]) & 0x03);
    if(unused_bit == true){
        return FILLED_UNUSED_BITS;
    }

       // Check checksum
    uint8_t checksum  = checksum_gen(buffer,len);
    if (buffer[12] != checksum) {
        return BAD_CHECKSUM;
    }

    // All checks passed
    return GOOD_BUFFER;
}