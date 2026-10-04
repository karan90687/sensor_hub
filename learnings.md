### extracting bits from a varaible
- Imp point to extracting bits from the reg like this is wrong 
- for example:
```c
 uint32_t device_id = 0x12345678;
uint32_t 0xFF = 0xFF;
     packet[4] = (f1.device_id & (0xFF));
    packet[5] = (f1.device_id & (0xFF<<8));
    packet[6] = (f1.device_id & (0xFF<<16));
    packet[7] = (f1.device_id & (0xFF<<24));
```
- so for the `packet[4]` is fine it will capture the first 8 bit as 78 but for the others `packet[5]` 
- `device_id & (0xFF << 16)` produces 0x00340000 not 34 
- so in order to fix this we have to shift bits 7-16 to 0-8 
```c
    0x12345678
     ↓ >> 8
    0x00123456
    now we apply the mask
    0x00123456
        &
    0x000000FF
        =
    0x00000056
    // so this is the coorect way now we will get the 56 
    packet[5] = (device_id >> 8) & 0xFF;
```

### C's integer promotions

```c
uint8_t checksum_gen(uint32_t timestamp){
    uint32_t mask = 0xFF;
    uint8_t t[4];       // array of elements to store the extracted timestamp 
    // printf("timestamp %x \n",timestamp);
    for (int i=0; i<4; i++){
        t[i] = mask & timestamp; // to extract the 8 bits from timestamp
```
- here mask and timestamp are `uint32_t` var so this operation `mask & timestamp` will produce a uint32_t result and t[i] is uint8_t so 24 bits will be ignored so compiler throws the warning which becomes error using this flag -Werror while compilation 
```bash
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/gen.c common/protocol.c -o build/gen
host/gen.c:13:21: error: implicit conversion loses integer precision: 'uint32_t' (aka 'unsigned int') to 'uint8_t' (aka 'unsigned char') [-Werror,-Wimplicit-int-conversion]
   13 |         t[i] = mask & timestamp; // to extract the 8 bits from timestamp
      |              ~ ~~~~~^~~~~~~~~~~
```
- but only u know that only lower bits are relevant so u are doing this compiler dont know to fix this we use `t[i] = (uint8_t)(mask & timestamp);`

### size_t 
- size_t represents sizes/counts use it in things like:
```c
size_t length;
size_t count;
size_t bytes;
```
- especially with 
```c
sizeof()
strlen()
malloc()
memcpy()
fread()
fwrite()
```
- in our code i passed `int` where `size_t` was expected so i got warning 
```c
int count = 13;
size_t written = fwrite(packet, sizeof(uint8_t), count, file);
```
- so fix is we have to pass `size_t count =13` here
```bash
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/gen.c common/protocol.c -o build/gen
host/gen.c:105:50: error: implicit conversion changes signedness: 'int' to 'unsigned long' [-Werror,-Wsign-conversion]
  105 | size_t written = fwrite(packet, sizeof(uint8_t), count, file);
      |                  ~~~~~~                          ^~~~~
```
- and one more thing to print the `size_t` values in c we have to use `%zu` instead of `%d` otherwise we get the warning 
- `%u` is used to print the unsigned int values
```bash
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/gen.c common/protocol.c -o build/gen
host/gen.c:110:68: error: format specifies type 'int' but the argument has type 'size_t' (aka 'unsigned long') [-Werror,-Wformat]
  110 |     printf("Error: Only wrote %zu out of %d elements.\n", written, count);
      |                                          ~~                        ^~~~~
      |                                          %zu
1 error generated.
```

### return statement
- `return` doesn't just return a value; it terminates the current function immediately.
- here in our code the code will never reach these statemetns as it return will cause to exit the function in teh beginging
```c

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
```
- so due this we got these type of errors 
```bash
karanrajput@karan sensor_hub % gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
    -g -fsanitize=address,undefined \
    -Icommon -Ihost \
    host/gen.c common/protocol.c -o build/gen
common/protocol.c:30:33: error: code will never be executed [-Werror,-Wunreachable-code]
   30 |     uint8_t recieved_checksum = checksum_gen(p->timestamp);
      |                                 ^~~~~~~~~~~~
common/protocol.c:21:9: error: code will never be executed [-Werror,-Wunreachable-code]
   21 |     if((p->sync_byte) == 0xAA){
      |         ^
2 errors generated.
karanrajput@karan sensor_hub % 
```
- so in the situation we have to we have to check multiple cases and all have to return the value so we can do this 
- now it will return BAD if any of the test case will fail else if all passed it will return GOOD 
```c
checksum packet_decoder(struct packet_t *p, int len)
{
    // Check length
    if (len != 13) {
        return BAD;
    }

    // Check sync byte
    if (p->sync_byte != 0xAA) {
        return BAD;
    }

    // Check checksum
    uint8_t recieved_checksum = checksum_gen(p->timestamp);

    if (recieved_checksum != p->checksum) {
        return BAD;
    }

    // All checks passed
    return GOOD;
}
```
