#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
    T value;
    struct node* next;
} TNode, *TSortedList;

// TODO : Cerința 1 & Cerința 2

TSortedList create(T value) {
    TNode* head = (TNode*)malloc(sizeof(TNode));
    head->value = value;
    head->next = NULL;
    return head;
}

int isEmpty(TSortedList head)
{
    return(head == NULL);
}

int contains(TSortedList head, T value)
{
    if(isEmpty(head)){
        return 0;
    }
    if(head->value == value){
        return 1;
    }
    
    TNode* prev = head;
    TNode* next = head->next;
    while(next != NULL){
        if(next->value == value)
            return 1;
        prev = next;
        next = next->next;
    }
    return 0;
}

TSortedList insert(TSortedList head, T value){
    if(isEmpty(head)){
        head = create(value);
        return head;
    }

    if(head->value >= value){
        TNode* node = create(value);
        node->next = head;
        head = node;
        return head;
    }

    TNode* prev = head;
    TNode* next = head->next;

    while(next != NULL){
        if(next->value > value){
            TNode* node = create(value);
            node->next = next;
            prev->next = node;
            return head;
        }
        prev = next;
        next = next->next;
    }

    TNode* node = create(value);
    prev->next = node;
    return head;
}

TSortedList deleteOnce(TSortedList head, T value){
    if(isEmpty(head))
        return head;
    TNode* prev = head;
    TNode* next = head->next;
    TNode* del;
    if(head->value == value){
        del = head;
        head = head->next;
        free(del);
        return head;
    }

    while(next != NULL){
        if(next->value == value){
            del = next;
            prev->next = next->next;
            free(del);
            return head;    
        }
        prev = next;
        next = next->next;
    }
    return head;
}

long length(TSortedList head){
    if(isEmpty(head)){
        return 0;
    }
    return 1+length(head->next);
}

T getNth(TSortedList head, int N){
    TSortedList elem = head;
    while(elem != NULL && N>1){
        elem = elem->next;
        N--;
    }
    return elem->value;
}

TSortedList freeTSortedList(TSortedList head){
    TNode* next;
    TNode* tmp;
    next = head;
    while(next != NULL){
        tmp = next;
        next = next->next;
        free(tmp);
    }
    return NULL;
}

#endif
