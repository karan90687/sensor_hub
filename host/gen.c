// this file will contain the program to generate the test frames

#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"

uint8_t checksum_gen(uint32_t timestamp){
    uint32_t mask = 0xFF;
    uint8_t t[4];       // array of elements to store the extracted timestamp 
    // printf("timestamp %x \n",timestamp);
    for (int i=0; i<4; i++){
        t[i] = mask & timestamp; // to extract the 8 bits from timestamp
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

// int main(){
    
// uint32_t t = 0x2C3D8;
// uint8_t c = checksum_gen(t);

//     struct packet_t f1;
//     f1.sync_byte = 0xAA;
//     f1.flag = 0xF7;       // 11110111
//     f1.temprature = 0x2B;    // 43
//     f1.device_id =  0x12;   // 18
//     f1.timestamp =  t;     // 181208 sec:mnt:hr
//     f1.checksum = c;

//     // to debug 
//     // printf("sync byte %d \n",f1.sync_byte);
//     // printf("flag byte %d \n",f1.flag);
//     // printf("temprature  %d \n",f1.temprature);
//     // printf("device id %d \n",f1.device_id);
//     // printf("timestamp byte %d \n",f1.timestamp);
//     // printf("checksum byte %d \n",f1.checksum);

//     // convert the int16_t temprature to uint16_t
//     uint16_t u_temprature = (uint16_t)f1.temprature;    
//     uint8_t packet[13];

//     // storing the data into the array to be written in edge.bin
//     packet[0] = f1.sync_byte;
//     packet[1] = f1.flag;
//     packet[2] = (u_temprature  & 0xFF);
//     packet[3] = ((u_temprature >> 8) & 0xFF);
//     packet[4] = (f1.device_id & (0xFF));
//     packet[5] = (f1.device_id >> 8) & 0xFF;
//     packet[6] = (f1.device_id >> 16) & 0xFF;
//     packet[7] = (f1.device_id >> 24) & 0xFF;
//     packet[8] = (f1.timestamp & 0xFF);
//     packet[9] = ((f1.timestamp >> 8) & 0xFF);
//     packet[10] = ((f1.timestamp >> 16) & 0xFF);
//     packet[11] = ((f1.timestamp >> 24) & 0xFF);
//     packet[12] = f1.checksum;

// /*Imp point to extracting bits from the reg like this is wrong 
//  for example:
//  uint32_t device_id = 0x12345678;
// uint32_t 0xFF = 0xFF;
//      packet[4] = (f1.device_id & (0xFF));
//     packet[5] = (f1.device_id & (0xFF<<8));
//     packet[6] = (f1.device_id & (0xFF<<16));
//     packet[7] = (f1.device_id & (0xFF<<24));
//     so for the packet[4] is fine it will capture the first 8 bit as 78 
//     but for the others packet[5] 
//     device_id & (0xFF << 16) produces 0x00340000 not 34 
    
//     so in order to fix this we have to shift bits 7-16 to 0-8 
//     0x12345678
//      ↓ >> 8
//     0x00123456
//     now we apply the mask
//     0x00123456
//         &
//     0x000000FF
//         =
//     0x00000056
//     so this is the coorect way 
//     packet[5] = (device_id >> 8) & 0xFF;

// */

//         // writting the data to edge.bin
// FILE *file = fopen("tests/data/edge.bin", "wb");

// if (file == NULL) {
//     perror("Error opening file");
//     return 1;
// }

// int count = 13;

// size_t written = fwrite(packet, sizeof(uint8_t), count, file);

// if (written == count) {
//     printf("Successfully wrote %zu elements to the file.\n", written);
// } else {
//     printf("Error: Only wrote %zu out of %d elements.\n", written, count);
// }

// fclose(file);
//     return 0;

// }

// /* i used this to compile this file to generate a buffer and store it in the edge.bin
// karanrajput@karan sensor_hub % gcc  -Icommon host/gen.c common/protocol.c -o gen
// karanrajput@karan sensor_hub % ./gen                                            
// Successfully wrote 13 elements to the file.
// karanrajput@karan sensor_hub % xxd tests/data/edge.bin
// 00000000: aaf7 2b00 1200 0000 d8c3 0200 9d         ..+..........
// karanrajput@karan sensor_hub % xxd -p tests/data/edge.bin
// aaf72b0012000000d8c302009d
// karanrajput@karan sensor_hub % xxd -g 1 tests/data/edge.bin
// 00000000: aa f7 2b 00 12 00 00 00 d8 c3 02 00 9d           ..+..........
// karanrajput@karan sensor_hub % 
// */