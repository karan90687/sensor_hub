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
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/main.c \
    common/protocol.c \
    host/format.c \
    -o main
karanrajput@karan sensor_hub % ./main
-------  Starting the buffer analysis function  -------
sync byte 170 in hex aa 
flag byte 247 in hex f7 
temprature  43 in hex 2b 
device id 18 in hex 12 
timestamp byte 181208 in hex 2c3d8 
checksum byte 157 in hex 9d 
result code of the buffer 0 
karanrajput@karan sensor_hub % 
    */

