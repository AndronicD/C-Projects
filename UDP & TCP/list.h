#ifndef _LIST_H_
#define _LIST_H_
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Client_Info{
    int socket;
    char id[11];
    int status;
    struct Queue_Message* queue_message;
    struct Topic_List* topics;
}Client_Info;

typedef struct Client_List{
    struct Client_Info client;
    struct Client_List* next;
}Client_List;

void add_client(Client_List** head, Client_Info client){
    struct Client_List* iter = *head;
    if(*head == NULL){
        (*head) = (Client_List*)malloc(sizeof(Client_List));
        (*head)->next = NULL;
        (*head)->client = client;
        return;    
    }
    struct Client_List* elem = (Client_List*)malloc(sizeof(Client_List));
    elem->next = NULL;
    elem->client = client;
    while(iter->next != NULL){
        iter = iter->next;
    }
    iter->next = elem;
    return;
}

struct Client_List* findClient(Client_List* head, int socket){
    struct Client_List* iter = head;
    while(iter){
       if(iter->client.socket == socket){
           return iter;
       }
       iter = iter->next; 
    }
    return NULL;
}

typedef struct Topic{
    char name[50];
    int sf;
}Topic;

typedef struct Topic_List{
    struct Topic topic;
    struct Topic_List* next; 
}Topic_List;

void add_topic(Topic_List** head, Topic topic){
    struct Topic_List* iter = *head;
    if(*head == NULL){
        (*head) = (Topic_List*)malloc(sizeof(Topic_List));
        (*head)->next = NULL;
        (*head)->topic = topic;
        return;
    }
    struct Topic_List* elem = (Topic_List*)malloc(sizeof(Topic_List));
    elem->next = NULL;
    elem->topic = topic;
    while(iter -> next != NULL){
        iter = iter->next;
    }
    iter->next = elem;
    return;
}

void remove_topic(Topic_List* head, char topic[]){
    struct Topic_List* iter = head;
    if(strcmp(iter->topic.name, topic) == 0){
        head = iter->next;
        free(iter);
        return;
    }
    while(iter != NULL){
        if(strcmp(iter->next->topic.name, topic) == 0){
            struct Topic_List* temp = iter->next;
            iter->next = iter->next->next;
            free(temp);
            return;
        }
        iter = iter->next;
    }
    return;
}

struct Topic_List* get_topic(Topic_List *head, char topic[50]){
    struct Topic_List* iter = head;
    while(iter != NULL){
        if(strcmp(iter->topic.name, topic) == 0){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

int contains(Topic_List *head, char topic[]){
    struct Topic_List* iter = head;
    while(iter != NULL){
        if(strcmp(iter->topic.name, topic) == 0){
            return 1;
        }
        iter = iter->next;
    }
    return 0;
}

void freeTopicList(Topic_List *head){
    struct Topic_List* iter = head;
    while(iter){
        struct Topic_List* temp = iter;
        iter = iter->next;
        free(temp); 
    }
}

void freeClientList(Client_List* head){
    struct Client_List* iter = head;
    while(iter){
       struct Client_List* temp = iter;
       iter = iter->next;
       freeTopicList(temp->client.topics);
       destroy_queue(temp->client.queue_message);
       free(temp); 
    }
    return;
}
#endif