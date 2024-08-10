#ifndef _PROTO_H_
#define _PROTO_H_
#define LENGTH2 15800 
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
#include "queue_udp_response.h"

void protocol_udp(char* buffer, int socket, Queue_UDP* q, int *start){
    int n;
    int start_point = *start;
    UDP_Response response;
    UDP_Response avarie;
    avarie.type = 10;
    memset(&avarie.port, 1, sizeof(int));
    memset(&avarie.topic, 1, 51);
    memset(&avarie.ip, 1, 20);
    memset(&avarie.payload, 1, 1501);
    memset(&response, 0, sizeof(UDP_Response)); 
    int m = 0;
    while(1){
        n = recv(socket, buffer + start_point + m, LENGTH2, 0);
        //Pentru viitoare concatenari de mesaje
        m += n;
        if(n == 0){
            //Daca serverul e inchis adaug mesaj de inchidere
            memcpy(buffer + start_point + m, &avarie, sizeof(UDP_Response));
            m += sizeof(UDP_Response);
            break;
        }
        //Daca nu am primit un mesaj intreg mai trebuie sa primesc
        if(n < sizeof(UDP_Response)){
            continue;
        }
        //Daca am primit cel putin dimensiunea unui mesaj 
        if(n >= sizeof(UDP_Response)){
            break;
        }
    }
    while(m >= sizeof(UDP_Response)){
        memcpy(&response, buffer, sizeof(UDP_Response));
        //Adaug comanda in coada
        enqueue_udp(q, response);
        char* newbuff = (char*)calloc(LENGTH2, sizeof(char));
        //Copiez ce era dupa mesaj
        memcpy(newbuff, buffer, LENGTH2 - sizeof(UDP_Response));
        memcpy(buffer, newbuff, LENGTH2);
        free(newbuff);
        //Copiez cat timp am destui bytes in buffer
        m -= sizeof(UDP_Response);
        if(m < sizeof(UDP_Response)){
            break;
        }
    }
    //Salvez cati bytes nu am adaugat in coada
    *start = m;
}
#endif