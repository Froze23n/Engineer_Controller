#ifndef RS232_H
#define RS232_H

#include "main.h"


#pragma pack(push, 1)
typedef struct{
    uint8_t OxA5;
    uint16_t data_length;
    uint8_t seq;
    uint8_t CRC8;

    uint16_t cmd_id;
    
    float J6006;
    float J10010;
    float MG5010;
    float DM4310[4];
    uint8_t reserved[2];
    
    uint16_t CRC16;
} Control_Type;
#pragma pack(pop)


void RS232_Init(void);
void RS232_Task(void);

#endif
