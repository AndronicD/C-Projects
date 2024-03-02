#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"
#include "application.h"
#include "headers/inputReader.h"
#include "headers/verify.h"
#include "headers/userData.h"

#define okText "OK"
#define defaultText "DEFAULT TEXT"

// Get the userCount and resourceCount from inputReader.c
extern int userCount;
extern int resourceCount;
// Get the number of operations, user list, resouce list, permissions queue and database info from 
// application_svc.c 
extern int numberOfOperations;
extern char** userList;
extern char** resourceList;
extern PermissionsQueue* permissionsQueue;
extern UserInfoList* userInfoDB;

Response_Auth *authenticate_1_svc(Request_Auth *request, struct svc_req *clnt){
    // Create response
    Response_Auth* response_auth = calloc(1, sizeof(response_auth));
    response_auth->status = calloc(50, sizeof(char));
    response_auth->token = calloc(20, sizeof(char));

    printf("BEGIN %s AUTHZ\n", request->clientId);
    // If the user is in the user list generate token
    if(checkInUserList(userList, request->clientId, userCount)){
        response_auth->status = strdup(okText);
        response_auth->token = generate_access_token(request->clientId);

        printf("  RequestToken = %s\n", response_auth->token);
        UserInfo* addedUser = createUserInfo(NULL, request->clientId, response_auth->token, defaultText, defaultText, numberOfOperations, request->autoRefresh);
        addInList(userInfoDB, addedUser);
    }
    else{
    // Else send default token 
        response_auth->status = strdup("USER_NOT_FOUND");
        response_auth->token = strdup(defaultText);
    }

    return response_auth;
}

Response_Approve *approve_1_svc(Request_Approve *argp, struct svc_req *clnt){
    // Create response
    Response_Approve* response_approve = calloc(1, sizeof(response_approve));
    ResourcePerm* permissionList = (ResourcePerm*)malloc(sizeof(permissionList));
    // Get permissions
    dequeuePermission(permissionsQueue, &permissionList);

    // If the permissions are *,- do not sign token
    if((strcmp(permissionList->data.resource, "*") == 0) && (strcmp(permissionList->data.permissions, "-") == 0)){
        response_approve->isSigned = 0;
    }
    else{
    // Else sign the token and assign permissions to user
        response_approve->isSigned = 1;
        UserInfo* user = findByAuthToken(userInfoDB, argp->authToken);
        user->permissionsList = permissionList;
    }

    return response_approve;
}

Response_Bearer *bearer_1_svc(Request_Bearer *argp, struct svc_req *clnt){
    // Create response
    Response_Bearer* response_bearer = calloc(1, sizeof(response_bearer));
    response_bearer->status = calloc(50, sizeof(char));
    response_bearer->accessToken = calloc(20, sizeof(char));
    response_bearer->refreshToken = calloc(20, sizeof(char));

    // If the request is not signed create Request_Denied response
    if(argp->isSigned == 0){
        response_bearer->status = strdup("REQUEST_DENIED");
        response_bearer->accessToken = strdup(defaultText);
        response_bearer->refreshToken = strdup(defaultText);
        response_bearer->timeout = 0;
    }
    else{
    // Else generate access token and if the autoRefresh is on generate the refresh token
        UserInfo* user = findByAuthToken(userInfoDB, argp->authToken);
        response_bearer->status = "REQUEST_APPROVED";
        response_bearer->accessToken = generate_access_token(user->authToken);
        strncpy(user->accessToken, response_bearer->accessToken, strlen(response_bearer->accessToken));
        printf("  AccessToken = %s\n", response_bearer->accessToken);
        
        if(user->autoRefresh == 1){
            response_bearer->refreshToken = generate_access_token(response_bearer->accessToken);
            strncpy(user->refreshToken, response_bearer->refreshToken, strlen(response_bearer->refreshToken));
            printf("  RefreshToken = %s\n", response_bearer->refreshToken);
        }
        else{
            response_bearer->refreshToken = defaultText;
        }
        response_bearer->timeout = numberOfOperations;
    }
    return response_bearer;
}

Renew_Response_Bearer *renew_bearer_1_svc(Renew_Request_Bearer *argp, struct svc_req *clnt){
    // Create response
    Renew_Response_Bearer* renew_response_bearer = calloc(1, sizeof(renew_response_bearer));
    renew_response_bearer->newAccessToken = (char*)calloc(20, sizeof(char));
    renew_response_bearer->newRefreshToken = (char*)calloc(20, sizeof(char));

    // Generate new tokens
    UserInfo* user = findByRefreshToken(userInfoDB, argp->refreshToken);
    user->accessToken = generate_access_token(user->refreshToken);
    user->refreshToken = generate_access_token(user->accessToken);

    // Renew number of actions
    user->numberOfActions = numberOfOperations;

    printf("BEGIN %s AUTHZ REFRESH\n", user->userId);
    printf("  AccessToken = %s\n", user->accessToken);
    printf("  RefreshToken = %s\n", user->refreshToken);

    strncpy(renew_response_bearer->newAccessToken, user->accessToken, strlen(user->accessToken));
    strncpy(renew_response_bearer->newRefreshToken, user->refreshToken, strlen(user->refreshToken));
    renew_response_bearer->timeout = numberOfOperations;

    return renew_response_bearer;
}

Response_Validate_Action *validate_action_1_svc(Request_Validate_Action *argp, struct svc_req *clnt){
    Response_Validate_Action* response_validate = calloc(1, sizeof(response_validate));
    response_validate->status = (char*)calloc(20, sizeof(char));
    UserInfo* user = findByAccessToken(userInfoDB, argp->accessToken);

    // If the user is does not have an access token generated -> Permission Denied
    if(user == NULL){
        response_validate->status = strdup("PERMISSION_DENIED");
        validatePrint("DENY", argp->action, argp->resource, "", 0);
        return response_validate;
    }
    else{
        bool isResource = checkInResourceList(resourceList, argp->resource, resourceCount);
        bool isOpPermitted = checkOpPermitted(user->permissionsList, argp->resource, argp->action);
        // If the user does not have any operations left -> Token expired
        if(user->numberOfActions == 0){
            response_validate->status = strdup("TOKEN_EXPIRED");
            validatePrint("DENY", argp->action, argp->resource, "", 0);
            return response_validate;
        }
        else if(!isResource){
        // If the resource is not found in the resource list
            user->numberOfActions--;
            response_validate->status = strdup("RESOURCE_NOT_FOUND");
            validatePrint("DENY", argp->action, argp->resource, argp->accessToken, user->numberOfActions);
            return response_validate;
        }
        else if(!isOpPermitted){
        // If the action and the resource are not in the permissions list of the user
            user->numberOfActions--;
            response_validate->status = strdup("OPERATION_NOT_PERMITTED");
            validatePrint("DENY", argp->action, argp->resource, argp->accessToken, user->numberOfActions);
            return response_validate;
        }
        else{
        // Is a good request
            user->numberOfActions--;
            response_validate->status =  strdup("PERMISSION_GRANTED");
            validatePrint("PERMIT", argp->action, argp->resource, argp->accessToken, user->numberOfActions);
            return response_validate;
        }
    }
    return response_validate;
}