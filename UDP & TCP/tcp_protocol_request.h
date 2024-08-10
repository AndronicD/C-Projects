#ifndef _PROTO_H_
#define _PROTO_H_
#define LENGTH 6000
#define MAX_TOPIC 50
#define MAX_ID 20
#define MAX_PAYLOAD 1500
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include "helpers.h"
#include "data.h"
#include "list.h"
#include "queue.h"
#include "queue_tcp_command.h"

void protocol_tcp(char* buffer, int socket, Queue_TCP* q){
    int n;
    TCP_Command command;
    TCP_Command avarie;
    avarie.SF = 10;
    avarie.type = 10;
    memset(&avarie.topic, 0, 50);
    memset(&command, 0, sizeof(TCP_Command)); 
    int m = 0;
    while(1){
        n = recv(socket, buffer + m, LENGTH, 0);
        //Pentru viitoare concatenari de mesaje
        m += n;
        //Daca serverul e inchis adaug mesaj de inchidere
        if(n == 0){
            memcpy(buffer + m, &avarie, sizeof(TCP_Command));
            m += sizeof(TCP_Command);
            break;
        }
        //Daca nu am primit un mesaj intreg mai trebuie sa primesc
        if(n < sizeof(TCP_Command)){
            continue;
        }
        //Daca am primit cel putin dimensiunea unui mesaj 
        if(n >= sizeof(TCP_Command)){
            break;
        }
    }
    while(m >= sizeof(TCP_Command)){
        memcpy(&command, buffer, sizeof(TCP_Command));
        //Adaug comanda in coada
        enqueue_tcp(q, command);
        char* newbuff = (char*)calloc(LENGTH, sizeof(char));
        //Copiez ce era dupa mesaj
        memcpy(newbuff, buffer, LENGTH - sizeof(TCP_Command));
        memcpy(buffer, newbuff, LENGTH);
        free(newbuff);
        //Copiez cat timp am destui bytes in buffer
        m -= sizeof(TCP_Command);
        if(m < sizeof(TCP_Command)){
            break;
        }
    }
}
#endif