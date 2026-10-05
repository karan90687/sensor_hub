#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "gen.h"
#include "format.h"

// to print the buffer 
void buffer_print(struct packet_t *p){
    printf("sync byte %d in hex %x \n",p->sync_byte,p->sync_byte);
    printf("flag byte %d in hex %x \n",p->flag,p->flag);
    printf("temprature  %d in hex %x \n",p->temprature,p->temprature);
    printf("device id %d in hex %x \n",p->device_id,p->device_id);
    printf("timestamp byte %d in hex %x \n",p->timestamp,p->timestamp);
    printf("checksum byte %d in hex %x \n",p->checksum,p->checksum);
}
