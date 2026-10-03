#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "format.h"
#include "gen.h"

#define MAX_SIZE 13

int main() {
    printf("-------  Starting the buffer analysis function  -------\n");
    // printf(" ------- Reading the data form the edg.bin ------- \n");

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
    frame.temprature = ((uint16_t)buffer[3] << 8) | buffer[2];
    frame.device_id = ((uint32_t)buffer[7] << 24) | ((uint32_t)buffer[6] << 16) | ((uint32_t)buffer[5] << 8) | buffer[4];
    frame.timestamp = ((uint32_t)buffer[11] << 24) | ((uint32_t)buffer[10] << 16) | ((uint32_t)buffer[9] << 8) | buffer[8];
    frame.checksum = buffer[12];

    /* concept : to recombine the splitted bits 
    like example a variable of uint16_t 

    uint16_t value = 0x1234;
    uint8_t high = (value >> 8) & 0xFF;
    uint8_t low  = value & 0xFF;    

    to combine back we will use "|" operator 
    uint16_t value = ((uint16_t)high << 8) | low;

    similarly for uint32_t as shown above in frame.timestamp
    */
    int len =13;
    // printf("------- Decoding and analysing the obtained buffer ------- \n");
    packet_decoder(&frame, len);
    // printf("------- printing the obtained buffer ------- \n");
    buffer_print(&frame, len);
    


    return 0;
}


/*
to compile the main.c 
gcc -Icommon -Ihost \
    host/main.c \
    common/protocol.c \
    host/gen.c \
    host/format.c \
    -o main
    */