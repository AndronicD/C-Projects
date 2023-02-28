#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs 
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){
	Stack* s = (Stack*)malloc(sizeof(Stack));
	s->head = NULL;
	s->size = 0;
	return s;
}

int isStackEmpty(Stack* stack){
	//if(stack == NULL) return 0;
	//if(stack->head == NULL) return 1;
	return (stack == NULL || stack->head == NULL);
}

void push(Stack *stack, Item elem){
	StackNode* item=(StackNode*)malloc(sizeof(StackNode));
	item->next = stack->head;
	item->elem = elem;
	stack->head = item;
	stack->size = stack->size + 1;
	return;
}

Item top(Stack *stack){	
	return stack->head->elem;
} 

void pop(Stack *stack){
	StackNode* iter = stack->head;
	stack->head = iter->next;
	stack->size = stack->size - 1;
	iter->next = NULL;
	return iter->elem;
}

void destroyStack(Stack *stack){
	StackNode* iter = stack->head;
	while(iter != NULL){
		StackNode* t = iter;
		stack->head->next = iter->next;
		iter = iter->next;
		free(t); 
	}
	free(stack);
}

#endif 
