// structs.h

#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>

// Queue for permissions list
typedef struct {
    char* resource;
    char* permissions;
} ResourcePermData;

typedef struct ResourcePerm {
    ResourcePermData data;
    struct ResourcePerm* next;
} ResourcePerm;

typedef struct PermissionsQueueData {
    ResourcePerm* data;  
    struct PermissionsQueueData* next;
} PermissionsQueueData;

typedef struct PermissionsQueue{
    PermissionsQueueData* front;
    PermissionsQueueData* rear;
} PermissionsQueue;


// User informations in database
typedef struct UserInfo {
    ResourcePerm* permissionsList;
    char *userId;
    char *authToken;
    char *accessToken;
    char *refreshToken;
    int numberOfActions;
    int autoRefresh;
} UserInfo;

typedef struct UserInfoNode{
    UserInfo data;
    struct UserInfoNode* next;
} UserInfoNode;

typedef struct UserInfoList{
    UserInfoNode *head;
} UserInfoList;

#endif /* STRUCTS_H */