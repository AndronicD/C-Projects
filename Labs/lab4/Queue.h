#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;


Queue* createQueue(void){
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	q->size = 0;
	return q;
} 

int isQueueEmpty(Queue *q){
	return (q == NULL || q->rear == NULL || q->front == NULL);
}

void enqueue(Queue *q, Item elem){
	if(q == NULL) return;
	QueueNode* iter = (QueueNode*)malloc(sizeof(QueueNode));
	iter->elem = elem;
	iter->next = NULL;
	if(q->front == NULL && q->rear == NULL){
		q->front = q->rear = iter;
		q->size = 1;
		return;
	}
	QueueNode* end = q->rear;
	end->next = iter;
	q->rear = iter;
	q->size = q->size + 1;
	return;
}

Item front(Queue* q){
	return q->front->elem;
}

void dequeue(Queue* q){
	if(q == NULL) return;
	if(q->front == NULL || q->rear == NULL) return;
	QueueNode* elem = q->front;
	if(elem == q->rear){
		q->front = NULL;
		q->rear = NULL;
		free(elem);
		return;
	}
	q->front = elem->next;
	q->size = q->size - 1;
	return;
}

void destroyQueue(Queue *q){
	QueueNode* iter = q->front;
	while(iter != NULL){
		QueueNode* t = iter;
		q->front = iter->next;
		iter = iter->next;
		free(t);
	}
	free(q);
	return;
}

#endif
