#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct TCP_Command{
    uint8_t SF;
    uint8_t type;
    char topic[50];
} TCP_Command;

typedef struct UDP_Response{
    uint8_t type;
    int port;
    char topic[51];
    uint8_t ip[20];
    uint8_t payload[1501];
} UDP_Response;

#endif