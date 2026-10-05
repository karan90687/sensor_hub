#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "format.h"
#include "gen.h"

#define MAX_SIZE 13

int main() {
    printf("-------  Starting the buffer analysis function  -------\n");

    uint8_t edge[MAX_SIZE];

    FILE *file = fopen("/Users/karanrajput/sensor_hub/tests/data/edge.bin", "rb");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    size_t count = fread(edge, sizeof(uint8_t), MAX_SIZE, file);

    fclose(file);

        // printf("Read %zu bytes:\n", count);
    uint8_t buffer[13];

    for (size_t i = 0; i < count; i++) {
        buffer[i] = edge[i];
        // printf("%02X ", edge[i]);
    }
    // printf("\n");
    struct packet_t frame;
    frame.sync_byte = buffer[0];
    frame.flag  = buffer[1];
    frame.temprature = (int16_t)(((uint16_t)buffer[3] << 8) | buffer[2]);
    frame.device_id = ((uint32_t)buffer[7] << 24) | ((uint32_t)buffer[6] << 16) | ((uint32_t)buffer[5] << 8) | buffer[4];
    // while decoding the big endian device_id use this not above 
        // frame.device_id = ((uint32_t)buffer[4] << 24) | ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[6] << 8) | buffer[7];
    frame.timestamp = ((uint32_t)buffer[11] << 24) | ((uint32_t)buffer[10] << 16) | ((uint32_t)buffer[9] << 8) | buffer[8];
    frame.checksum = buffer[12];

    int len =(int)count;
    // printf("------- Decoding and analysing the obtained buffer ------- \n");
    buffer_type result = packet_decoder(&frame, len);
    if (result == 0){
    printf("The buffer is good Kernel Happy \n");
    buffer_print(&frame);
    }else{
        // printf("bad buffer provided \n");
        return -1;
    }
    
    return 0;
}


/*
to compile the main.c 
karanrajput@Mac sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/main.c \
    common/protocol.c \
    host/format.c \
    -o main
karanrajput@Mac sensor_hub % ./main
-------  Starting the buffer analysis function  -------
The buffer is good Kernel Happy 
sync byte 170 in hex aa 
flag byte 244 in hex f4 
temprature  43 in hex 2b 
device id 18 in hex 12 
timestamp byte 181208 in hex 2c3d8 
checksum byte 157 in hex 9d
karanrajput@karan sensor_hub % 
    */

