// verify.h

#ifndef VERIFY_H
#define VERIFY_H
#include "structs.h"
#include <stdbool.h>

bool checkInUserList(char** userList, char* userName, int size);
bool checkInResourceList(char** resourceList, char* resourceName, int size);
bool checkCharInString(const char* string, char character);
bool checkOpPermitted(ResourcePerm* node, char* resource, char* permissions);
#endif /* VERIFY_H */