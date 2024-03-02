// userData.h

#ifndef USERDATA_H
#define USERDATA_H
#include "structs.h"

UserInfoList* initializeList(UserInfoList** list);
UserInfo* createUserInfo(ResourcePerm* permissionsList, char *userId, char *authToken, char *accessToken, char *refreshToken, int numberOfActions, int autoRefresh);
void addInList(UserInfoList* list, UserInfo* data);
UserInfo* findByUserId(UserInfoList* list, const char* userId);
UserInfo* findByAuthToken(UserInfoList* list, const char* authToken);
UserInfo* findByAccessToken(UserInfoList* list, const char* accessToken);
UserInfo* findByRefreshToken(UserInfoList* list, const char* refreshToken);
void validatePrint(char* status, char* action, char* resource, char* accessToken, int opLeft);

#endif /* USERDATA_H */