#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "data.h"
#define LENGTH2 10000 

typedef struct TCPNode{
    struct TCP_Command item;
    struct TCPNode* next;
}TCPNode;

typedef struct Queue_TCP{
    struct TCPNode* front;
    struct TCPNode* rear;
}Queue_TCP;

Queue_TCP* createQueue_tcp(void){
    Queue_TCP* q = (Queue_TCP*)malloc(sizeof(Queue_TCP));
    q->front = q->rear = NULL;
    return q;
}

int isEmpty_tcp(Queue_TCP *q){
    return (q == NULL || q->front == NULL || q->rear == NULL);
}

TCP_Command front_tcp(Queue_TCP *q){
    return q->front->item;
}

void enqueue_tcp(Queue_TCP *q, TCP_Command item){
    if(q == NULL) return;
    TCPNode* iter = (TCPNode*)malloc(sizeof(TCPNode));
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

void dequeue_tcp(Queue_TCP* q){
    if(q == NULL) return;
    if(q->front == NULL || q->rear == NULL){
        return;
    }
    TCPNode* elem = q->front;
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

void destroy_queue_tcp(Queue_TCP* q){
    if(q == NULL) return;
    struct TCPNode* iter = q->front; 
    while(iter){
        struct TCPNode* temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(q);
}