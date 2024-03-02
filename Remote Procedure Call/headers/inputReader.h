// inputReader.h

#ifndef INPUTREADER_H
#define INPUTREADER_H
#include "structs.h"

char** readUserList(const char* inputFileName);
char** readResourceList(const char* inputFileName);
char*** readClientInput(const char* inputFileName, int *lineCount);
PermissionsQueue* readPermissionsQueue(const char* permissionsFile);

void initPermissionsQueue(PermissionsQueue* permissionsQueue);
void enqueuePermission(PermissionsQueue* permissionsQueue, ResourcePerm* data);
void dequeuePermission(PermissionsQueue* permissionsQueue, ResourcePerm** result);


#endif /* INPUTREADER_H */