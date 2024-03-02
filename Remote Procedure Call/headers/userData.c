#include "userData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to initialize database
UserInfoList* initializeList(UserInfoList** list) {
    *list = (UserInfoList*)malloc(sizeof(list));
    (*list)->head = NULL;
    return (*list);
}

// Function to add user information in database 
void addInList(UserInfoList* list, UserInfo* data) {
    UserInfo* user = findByUserId(list, data->userId);
    if(user != NULL){
        user->permissionsList = data->permissionsList;
        if(data->userId != NULL){
            strcpy(user->userId, data->userId);
        }
        if(data->accessToken != NULL){
            strcpy(user->accessToken, data->accessToken);
        }
        if(data->authToken != NULL){
            strcpy(user->authToken, data->authToken);
        }
        if(data->refreshToken != NULL){
            strcpy(user->refreshToken, data->refreshToken);
        }
        user->numberOfActions = data->numberOfActions;
        user->autoRefresh = data->autoRefresh;
    }
    else{
        UserInfoNode* newNode = (UserInfoNode*)malloc(sizeof(UserInfoNode));

        newNode->data = *data;
        newNode->next = list->head;
        list->head = newNode;
    }
}

// Function to find a user with an ID in database 
UserInfo* findByUserId(UserInfoList* list, const char* userId) {
    UserInfoNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.userId, userId) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;  // User not found
}

// Function to find a user with an authentication token in database 
UserInfo* findByAuthToken(UserInfoList* list, const char* authToken){
    UserInfoNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.authToken, authToken) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;  // User not found
}

// Function to find a user with an access token in database 
UserInfo* findByAccessToken(UserInfoList* list, const char* accessToken) {
    UserInfoNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.accessToken, accessToken) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;  // User not found
}

// Function to find a user with an refresh token token in database 
UserInfo* findByRefreshToken(UserInfoList* list, const char* refreshToken){
    UserInfoNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.refreshToken, refreshToken) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;  // User not found
}

// Creates a row information in the database
UserInfo* createUserInfo(ResourcePerm* permissionsList, char *userId, char *authToken, char *accessToken, char *refreshToken, int numberOfActions, int autoRefresh){
    UserInfo* userInfo = (UserInfo*)calloc(1, sizeof(UserInfo));
    userInfo->userId = (char*)calloc(20, sizeof(char));
    userInfo->authToken = (char*)calloc(20, sizeof(char));
    userInfo->accessToken = (char*)calloc(20, sizeof(char));
    userInfo->refreshToken = (char*)calloc(20, sizeof(char));

    userInfo->permissionsList = permissionsList;
    if(userId != NULL){
        strcpy(userInfo->userId, userId);
    }
    if(accessToken != NULL){
        strcpy(userInfo->accessToken, accessToken);
    }
    if(authToken != NULL){
        strcpy(userInfo->authToken, authToken);
    }
    if(refreshToken != NULL){
        strcpy(userInfo->refreshToken, refreshToken);
    }
    userInfo->numberOfActions = numberOfActions;
    userInfo->autoRefresh = autoRefresh;
    return userInfo;
}

// Print the validate action result in server
void validatePrint(char* status, char* action, char* resource, char* accessToken, int opLeft){
    printf("%s (%s,%s,%s,%d)\n", status, action, resource, accessToken, opLeft);
}