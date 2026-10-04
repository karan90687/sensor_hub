// this file will contain the program to generate the test frames

#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"

void good_frame(struct packet_t *f1 ,uint32_t timestamp,uint8_t c){

        f1->sync_byte = 0xAA;
    f1->flag = 0xF7;       // 11110111
    f1->temprature = 0x2B;    // 43
    f1->device_id =  0x12;   // 18
    f1->timestamp =  timestamp;     // 181208 sec:mnt:hr
    f1->checksum = c;
}

void tmp_negative_frame(struct packet_t *f1 ,uint32_t timestamp,uint8_t c){

        f1->sync_byte = 0xAA;
    f1->flag = 0xF7;       // 11110111
    f1->temprature = -43;    // 43
    f1->device_id =  0x12;   // 18
    f1->timestamp =  timestamp;     // 181208 sec:mnt:hr
    f1->checksum = c;
}

void bad_sync_frame(struct packet_t *f1 ,uint32_t timestamp,uint8_t c){

        f1->sync_byte = 0xAB;
    f1->flag = 0xF7;       // 11110111
    f1->temprature = -43;    // 43
    f1->device_id =  0x12;   // 18
    f1->timestamp =  timestamp;     // 181208 sec:mnt:hr
    f1->checksum = c;
}

void bad_checksum_frame(struct packet_t *f1 ,uint32_t timestamp,uint8_t c){

        f1->sync_byte = 0xAA;
    f1->flag = 0xF7;       // 11110111
    f1->temprature = -43;    // 43
    f1->device_id =  0x12;   // 18
    f1->timestamp =  timestamp;     // 181208 sec:mnt:hr
    f1->checksum = 0x34;
}



int main(){

    uint32_t t = 0x2C3D8;
uint8_t c = checksum_gen(t);

    struct packet_t f1;
    // to generate a good frame
    good_frame(&f1,t,c);

    // to make a frame with negative temprature 


    // to debug 
    // printf("sync byte %d \n",f1.sync_byte);
    // printf("flag byte %d \n",f1.flag);
    // printf("temprature  %d \n",f1.temprature);
    // printf("device id %d \n",f1.device_id);
    // printf("timestamp byte %d \n",f1.timestamp);
    // printf("checksum byte %d \n",f1.checksum);

    // convert the int16_t temprature to uint16_t
    uint16_t u_temprature = (uint16_t)f1.temprature;    
    uint8_t packet[13];

    // storing the data into the array to be written in edge.bin
    packet[0] = f1.sync_byte;
    packet[1] = f1.flag;
    packet[2] = (u_temprature  & 0xFF);
    packet[3] = ((u_temprature >> 8) & 0xFF);
    packet[4] = (f1.device_id & (0xFF));
    packet[5] = (f1.device_id >> 8) & 0xFF;
    packet[6] = (f1.device_id >> 16) & 0xFF;
    packet[7] = (f1.device_id >> 24) & 0xFF;
    packet[8] = (f1.timestamp & 0xFF);
    packet[9] = ((f1.timestamp >> 8) & 0xFF);
    packet[10] = ((f1.timestamp >> 16) & 0xFF);
    packet[11] = ((f1.timestamp >> 24) & 0xFF);
    packet[12] = f1.checksum;


        // writting the data to edge.bin
FILE *file = fopen("tests/data/edge.bin", "wb");

if (file == NULL) {
    perror("Error opening file");
    return 1;
}

size_t count = 13;

size_t written = fwrite(packet, sizeof(uint8_t), count, file);

if (written == count) {
    printf("Successfully wrote %zu elements to the file.\n", written);
} else {
    printf("Error: Only wrote %zu out of %zu elements.\n", written, count);
}

fclose(file);
    return 0;

}

/* i used this to compile this file to generate a buffer and store it in the edge.bin
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/gen.c common/protocol.c -o gen      
karanrajput@karan sensor_hub % ./gen
Successfully wrote 13 elements to the file.
karanrajput@karan sensor_hub % 

*/