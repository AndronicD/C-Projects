#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "data.h"

typedef struct UDPNode{
    struct UDP_Response item;
    struct UDPNode* next;
}UDPNode;

typedef struct Queue_UDP{
    struct UDPNode* front;
    struct UDPNode* rear;
}Queue_UDP;

Queue_UDP* createQueue_udp(void){
    Queue_UDP* q = (Queue_UDP*)malloc(sizeof(Queue_UDP));
    q->front = q->rear = NULL;
    return q;
}

int isEmpty_udp(Queue_UDP *q){
    return (q == NULL || q->front == NULL || q->rear == NULL);
}

UDP_Response front_udp(Queue_UDP *q){
    return q->front->item;
}

void enqueue_udp(Queue_UDP *q, UDP_Response item){
    if(q == NULL) return;
    UDPNode* iter = (UDPNode*)malloc(sizeof(UDPNode));
    iter->item = item;
    iter->next = NULL;
    if(q->front == NULL && q->rear == NULL){
        q->front = q->rear = iter;
        return;
    }
    q->rear->next = iter;
    q->rear = iter;
    return;
}

int size(Queue_UDP *q){
    UDPNode* iter = q->front;
    int n = 0;
    while(iter){
        n++;
        iter = iter->next;
    }
    return n;
}

void dequeue_udp(Queue_UDP* q){
    if(q == NULL) return;
    if(q->front == NULL || q->rear == NULL){
        return;
    }
    UDPNode* elem = q->front;
    if(elem == q->rear){
        q->front = NULL;
        q->rear = NULL;
        free(elem);
        return;
    }
    q->front = elem->next;
    free(elem);
    return;
}

void destroy_queue_udp(Queue_UDP* q){
    if(q == NULL) return;
    struct UDPNode* iter = q->front; 
    while(iter){
        struct UDPNode* temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(q);
}