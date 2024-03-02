#include <stdio.h>
#include <string.h>
#include "verify.h"

// Checks if a user is in the user list
bool checkInUserList(char** userList, char* userName, int size){
    for (int i = 0; i < size; ++i) {
        if (strcmp(userName, userList[i]) == 0) {
            return true; // The target is found in the array
        }
    }
    return false; // The target is not found in the array
}

// Checks if a resource is in the resource list
bool checkInResourceList(char** resourceList, char* resourceName, int size){
    for (int i = 0; i < size; ++i) {
        if (strcmp(resourceName, resourceList[i]) == 0) {
            return true; // The target is found in the array
        }
    }
    return false; // The target is not found in the array
}

// Checks if a char is in a string
bool checkCharInString(const char* string, char character) {
    while (*string != '\0') {
        if (*string == character) {
            return true;  // Character found
        }
        string++;
    }
    return false;  // Character not found
}

// Checks if the resource and the permission is in the permissions list of an user 
bool checkOpPermitted(ResourcePerm* node, char* resource, char* permissions){
    while (node != NULL) {
        if (strcmp(node->data.resource, resource) == 0) {
            switch (permissions[0]) {
                case 'I':
                    return checkCharInString(node->data.permissions, 'I');
                case 'M':
                    return checkCharInString(node->data.permissions, 'M');
                case 'D':
                    return checkCharInString(node->data.permissions, 'D');
                case 'R':
                    return checkCharInString(node->data.permissions, 'R');
                case 'E':
                    return checkCharInString(node->data.permissions, 'X');
                default:
                    return false;
            }
        }
        node = node->next;
    }
    return false;
}