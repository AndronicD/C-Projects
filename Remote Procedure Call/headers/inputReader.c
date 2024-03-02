#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputReader.h"

#define LINE_SIZE 1024
int userCount;
int resourceCount;

// Function to read the input from client.in files
char*** readClientInput(const char* inputFileName, int* lineCount) {
    FILE *file = fopen(inputFileName, "r");

    char ***inputList = (char***)calloc(1, sizeof(char**));
    char line[LINE_SIZE];

    *lineCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Create tokens: (clientId, action, resource)
        char *token;
        char **fields = calloc(3, sizeof(char*));
        int fieldCount = 0;

        token = strtok(line, ",");
        while (token != NULL && fieldCount < 3) {
            int fieldSize = strlen(token) + 1;
            fields[fieldCount] = (char*)malloc(fieldSize * sizeof(char));
            strncpy(fields[fieldCount], token, fieldSize);
            token = strtok(NULL, ",");
            fieldCount++;
        }

        // Add the fields 
        inputList = (char***)realloc(inputList, (*lineCount + 1) * sizeof(char**));
        inputList[*lineCount] = fields;
        (*lineCount)++;
    }

    fclose(file);

    return inputList;
}

// Function to read the input from userIDs.db files
char** readUserList(const char* inputFileName){
    FILE* file = fopen(inputFileName, "r");

    int numStrings;
    // Read the number of inputs
    fscanf(file, "%d", &numStrings);
    fgetc(file);

    userCount = numStrings;
    char** userList = (char**)malloc(numStrings * sizeof(char*));

    int current = 0;
    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        int size = strlen(line) + 1;
        userList[current] = (char*)malloc(size * sizeof(char));

        strncpy(userList[current], line, size);
        current++;
    }

    fclose(file);

    return userList;
}

// Function to read the input from resources.db files
char** readResourceList(const char* inputFileName){
    FILE* file = fopen(inputFileName, "r");

    int numStrings;
    // Read the number of inputs
    fscanf(file, "%d", &numStrings);
    fgetc(file);

    resourceCount = numStrings;
    char** resourceList = (char**)malloc(numStrings * sizeof(char*));

    int current = 0;
    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        int size = strlen(line) + 1;
        resourceList[current] = (char*)malloc(size * sizeof(char));

        strncpy(resourceList[current], line, size);
        current++;
    }

    fclose(file);

    return resourceList;
}

void initPermissionsQueue(PermissionsQueue* permissionsQueue) {
    permissionsQueue->front = permissionsQueue->rear = NULL;
}

void enqueuePermission(PermissionsQueue* permissionsQueue, ResourcePerm* data) {
    PermissionsQueueData* newNode = (PermissionsQueueData*)malloc(sizeof(PermissionsQueueData));
    if (newNode == NULL) {
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (permissionsQueue->rear == NULL) {
        permissionsQueue->front = permissionsQueue->rear = newNode;
    } 
    else {
        permissionsQueue->rear->next = newNode;
        permissionsQueue->rear = newNode;
    }
}

void dequeuePermission(PermissionsQueue* permissionsQueue, ResourcePerm** result) {
    if (permissionsQueue->front == NULL) {
        return;
    }

    PermissionsQueueData* temp = permissionsQueue->front;
    *result = temp->data;

    permissionsQueue->front = temp->next;
    if (permissionsQueue->front == NULL) {
        permissionsQueue->rear = NULL;
    }

    free(temp);  
}

// Function to read the input from approvals.db files and return a queue of permissions
PermissionsQueue* readPermissionsQueue(const char* permissionsFile) {
    FILE* file = fopen(permissionsFile, "r");

    char line[LINE_SIZE];
    PermissionsQueue* permissionsQueue = malloc(sizeof(*permissionsQueue));
    initPermissionsQueue(permissionsQueue);

    while (fgets(line, sizeof(line), file) != NULL) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        ResourcePerm* end = NULL;
        ResourcePerm* list = NULL;

        char* token = strtok(line, ",");
        while (token != NULL){
            ResourcePerm* resourcePerm = (ResourcePerm*)malloc(sizeof(ResourcePerm));
            if (token != NULL) {
                resourcePerm->data.resource = (char*)malloc(strlen(token) + 1);
                strncpy(resourcePerm->data.resource, token, strlen(token) + 1);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                resourcePerm->data.permissions = (char*)malloc(strlen(token) + 1);
                strncpy(resourcePerm->data.permissions, token, strlen(token) + 1);
            }

            if (list == NULL) {
                list = end = resourcePerm;
            } 
            else {
                end->next = resourcePerm;
                end = resourcePerm;
            }
            token = strtok(NULL, ",");
        }
        enqueuePermission(permissionsQueue, list);
    }

    fclose(file);
    return permissionsQueue;
}