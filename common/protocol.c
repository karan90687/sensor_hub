#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"


checksum packet_decoder(struct packet_t *p, int len){
    // checking the lenght of packet
    if (len == 13){
        // printf("Buffer lenght is 13 as expected \n");
        return GOOD;
    }else if(len<13){
        // printf("Buffer lenght is greater than expected %d \n",len);
        return BAD;
    }else{
        // printf("Buffer lenght is less than expected %d \n",len);
        return BAD;
    }

    // checking the sync byte present or not
    if((p->sync_byte) == 0xAA){
        // printf("sync byte 0xAA is present \n");
        return GOOD;
    }else{
        // printf("sync byte 0xAA is not present buffer contains %x \n",(p->sync_byte));
        return BAD;
    }


    uint8_t recieved_checksum = checksum_gen(p->timestamp);
    if (recieved_checksum == p->checksum){
        // printf("buffer is perfect cheksum byte present \n");
        return GOOD;
    }else{
        // printf("checksum byte is missing or corrupted %x \n",recieved_checksum);
        return BAD;
    }


}