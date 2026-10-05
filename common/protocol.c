#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"
#include <stdbool.h>


uint8_t checksum_gen(uint32_t timestamp){
    uint32_t mask = 0xFF;
    uint8_t t[4];       // array of elements to store the extracted timestamp 
    // printf("timestamp %x \n",timestamp);
    for (int i=0; i<4; i++){
        t[i] = (uint8_t)(mask & timestamp); // to extract the 8 bits from timestamp
    timestamp = (timestamp >> 8);  
    }
    uint8_t checksum = t[0]+t[1]+t[2]+t[3];

    // to print individual bits 
    // for(int j=0; j<4; j++){
    //         printf("bit t[%d] %x \n",j,t[j]);

    //     }
    // printf("checksum %x \n",checksum);

    return checksum;
}

buffer_type packet_decoder(struct packet_t *p, int len)
{
    // Check length
    if (len != 13) {
        printf("short buffer \n");
        return BAD;
    }

    // Check sync byte
    if (p->sync_byte != 0xAA) {
        printf("sync_byte missing in the buffer \n");
        return BAD;
    }

    // Check checksum
    uint8_t recieved_checksum = checksum_gen(p->timestamp);

    if (recieved_checksum != p->checksum) {
        printf("bad checksum buffer \n");
        return BAD;
    }

    // unused bits set or not
    bool unused_bit = ((p->flag) & 0x03);
    if(unused_bit == true){
        printf("unused bits are set in buffer \n");
        return BAD;
    }

    // All checks passed
    return GOOD;
}