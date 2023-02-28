#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	//TODO:
	PriQueue *h = malloc(sizeof(PriQueue));
	h->elem = (ItemType*)malloc(maxHeapSize*sizeof(ItemType));
	h->size = 0;
	h->maxHeapSize = maxHeapSize;
	return h;
}

int getLeftChild(int i){
	//TODO:
	return (2*i+1);
}

int getRightChild(int i) {
	//TODO:
	return (2*i+2);
}

int getParent(int i) {
	//TODO:
	return (i-1)/2;
}

ItemType getMax(APriQueue h) {
	//TODO:
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	//TODO:
	int parent = getParent(idx);
	while(parent >= 0 && h->elem[parent].prior < h->elem[idx].prior){
		ItemType aux = h->elem[parent];
		h->elem[parent] = h->elem[idx];
		h->elem[idx] = h->elem[parent];

		idx = parent;
		parent = getParent(idx);
	}
}


void insert(PriQueue *h, ItemType x) {
	//TODO:
	if(h->size == h->maxHeapSize){
		h->maxHeapSize *= 2;
		h->elem = (ItemType*)realloc(h->elem, h->maxHeapSize * sizeof(ItemType));
	}

	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx){
	//TODO:
	int leftchild;
	int rightchild;
	int largest = idx;

	do{

	idx = largest;
	leftchild = getLeftChild(idx);
	rightchild = getRightChild(idx);

	if(leftchild < h->size && h->elem[leftchild].prior > h->elem[largest].prior){
		largest = leftchild;
	}

	if(rightchild < h->size && h->elem[rightchild].prior > h->elem[largest].prior){
		largest = rightchild;
	}

	if(largest != idx){
		ItemType aux = h->elem[largest];
		h->elem[largest] = h->elem[idx];
		h->elem[idx] = aux;
	}
	
	}while(largest != idx);
}

void removeMax(APriQueue h) {
	//TODO:
	h->elem[0] = h->elem[h->size - 1];
	h->size--;
	siftDown(h, 0);
}

void freeQueue(APriQueue h){
	// TODO:
	free(h->elem);
	free(h);
}

#endif
