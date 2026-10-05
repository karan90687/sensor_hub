# Packet / Buffer Format

Total packet size: **13 bytes**

## Packet Layout
| Byte | Field | Size |
|---:|---|---:|
| 0 | `sync_byte` | 1 byte |
| 1 | `flag` | 1 byte |
| 2–3 | `temperature` | 2 bytes |
| 4–7 | `device_id` | 4 bytes |
| 8–11 | `timestamp` | 4 bytes |
| 12 | `checksum` | 1 byte |
---

## Byte 1 — Sync Byte

```text
  7 6 5 4 3 2 1 0
┌─────────────────┐
│     SYNC BYTE   │
└─────────────────┘
```
## Byte 0 — Flag and Device type bit
```
Value: 0xAA

  7 6 5 4   3 2   1 0
┌─────────┬─────┬────-─┐
│ Sensor  │Flags│Unused│
│  Type   │     │      │
└─────────┴─────┴────-─┘
   4 bits   2 bits 2 bits
```
## Bytes 2–3 — Temperature int16_t — Little-endian
```
temperature = 0x1234

Byte 2        Byte 3
  34            12
┌────────┐   ┌────────┐
│  0x34  │   │  0x12  │
└────────┘   └────────┘
   LSB          MSB
```
## Bytes 4–7 — Device ID 
```
Byte 4      Byte 5      Byte 6      Byte 7
┌─────────┬─────────┬─────────┬─────────┐
│  [7:0]  │ [15:8]  │ [23:16] │ [31:24] │
│   LSB   │         │         │   MSB   │
└─────────┴─────────┴─────────┴─────────┘
```
## Bytes 8–11 — Timestamp
```
Byte 8      Byte 9      Byte 10     Byte 11
┌─────────┬─────────┬─────────┬─────────┐
│  [7:0]  │  [15:8] │ [23:16] │ [31:24] │
│   LSB   │         │         │   MSB   │
└─────────┴─────────┴─────────┴─────────┘
```
## Byte 12 — Checksum
```
  7 6 5 4 3 2 1 0
┌─────────────────┐
│     CHECKSUM    │
└─────────────────┘
``` 
## Complete Buffer
```
Byte:      0       1       2       3       4       5       6       7       8       9       10      11      12

         ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────--┐
         │ SYNC  │ FLAG  │ TEMP  │ TEMP  │ DEVICE ID     │       │       │ TIMESTAMP     │       │       │CHECKSUM │
         │       │       │  LSB  │  MSB  │ B0    │ B1    │ B2    │ B3    │ B0    │ B1    │ B2    │ B3    │         │
         └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┴───--────┘
```
