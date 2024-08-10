#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct UDP_Message{
    char topic[51];
    uint8_t type;
    uint8_t payload[1501];
}UDP_Message;

typedef struct QueueNode{
    struct UDP_Message item;
    struct QueueNode* next;
}QueueNode;

typedef struct Queue_Message{
    struct QueueNode* front;
    struct QueueNode* rear;
}Queue_Message;

Queue_Message* createQueue(void){
    Queue_Message* q = (Queue_Message*)malloc(sizeof(Queue_Message));
    q->front = q->rear = NULL;
    return q;
}

int isEmpty(Queue_Message *q){
    return (q == NULL || q->front == NULL || q->rear == NULL);
}

UDP_Message front(Queue_Message *q){
    return q->front->item;
}

void enqueue(Queue_Message *q, UDP_Message item){
    if(q == NULL) return;
    QueueNode* iter = (QueueNode*)malloc(sizeof(QueueNode));
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

void dequeue(Queue_Message* q){
    if(q == NULL) return;
    if(q->front == NULL || q->rear == NULL){
        return;
    }
    QueueNode* elem = q->front;
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

void destroy_queue(Queue_Message* q){
    if(q == NULL) return;
    struct QueueNode* iter = q->front; 
    while(iter){
        struct QueueNode* temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(q);
}

#endif