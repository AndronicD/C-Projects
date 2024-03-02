#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "application.h"
#include "headers/inputReader.h"
#include "headers/userData.h"
#include "headers/userData.h"

#define DEFAULT_TEXT "defaultText"
#define MAX_LINE_LENGTH 100
UserInfoList* userInfoDB;

int main(int argc, char** argv) {
    //Authenticate Fields
    userInfoDB = initializeList(&userInfoDB);
    Response_Auth *response_auth;
    Request_Auth *request_auth;

    //Approve Fields
    Response_Approve *response_approve;
    Request_Approve *request_approve;

    //Bearer Fields
    Response_Bearer *response_bearer;
    Request_Bearer *request_bearer;

    //Renew Bearer Fields
    Renew_Response_Bearer *renew_response_bearer;
    Renew_Request_Bearer *renew_request_bearer;

    //Validate Action Fields
    Response_Validate_Action *response_validate;
    Request_Validate_Action *request_validate;

    //Create client
    CLIENT *client = clnt_create("localhost", MANAGER_OF_RESOURCES_APP, MANAGER_OF_RESOURCES_APP_VERSION, "tcp");
    char ***inputFromFile;

    FILE *file;
    char line[MAX_LINE_LENGTH];
    int linecount;

    inputFromFile = readClientInput(argv[2], &linecount);

    for(int i = 0; i < linecount; i++){
        // Fourth step: Renew
        // If there is a user (with 0 number of actions left and opted for auto refresh) and the action is not a request
        // sends a renew token request
        UserInfo* user = findByUserId(userInfoDB, inputFromFile[i][0]);
        if(user != NULL && !(strcmp(inputFromFile[i][1], "REQUEST") == 0)){
            if(user->numberOfActions == 0 && user->autoRefresh == 1){
                // Create request
                renew_request_bearer = calloc(1, sizeof(*renew_request_bearer));
                renew_request_bearer->refreshToken = (char*)calloc(20, sizeof(char));

                // Copy data in request
                strncpy(renew_request_bearer->refreshToken, user->refreshToken, strlen(user->refreshToken));
                renew_response_bearer = renew_bearer_1(renew_request_bearer, client);

                //Modify data in database
                strncpy(user->accessToken, renew_response_bearer->newAccessToken, strlen(renew_response_bearer->newAccessToken));
                strncpy(user->refreshToken, renew_response_bearer->newRefreshToken, strlen(renew_response_bearer->newRefreshToken));
                user->numberOfActions = renew_response_bearer->timeout;

                // Free request and response fields
                free(renew_response_bearer->newAccessToken);
                free(renew_response_bearer->newRefreshToken);
                free(renew_request_bearer->refreshToken);
            }
        }
        if(strcmp(inputFromFile[i][1], "REQUEST") == 0){
            // Create requests
            request_auth = calloc(1, sizeof(*request_auth));
            request_auth->clientId = (char*)calloc(16, sizeof(char));

            request_approve = calloc(1, sizeof(*request_approve));
            request_approve->authToken = (char*)calloc(20, sizeof(char));

            request_bearer = calloc(1, sizeof(*request_bearer));
            request_bearer->authToken = (char*)calloc(20, sizeof(char));

            // First step: Authenticate
            // Copy data in request
            strncpy(request_auth->clientId, inputFromFile[i][0], strlen(inputFromFile[i][0]));
            request_auth->autoRefresh = atoi(inputFromFile[i][2]);
            response_auth = authenticate_1(request_auth, client);

            // If user not found skip the iteration
            if(strcmp(response_auth->status, "USER_NOT_FOUND") == 0){
                printf("USER_NOT_FOUND\n");
                continue;
            }
            
            // Second step: Approve
            // Copy data in request and send request
            strncpy(request_approve->authToken, response_auth->token, strlen(response_auth->token));
            response_approve = approve_1(request_approve, client);

            // Third step: Access
            // Copy data in request and send request
            strncpy(request_bearer->authToken, response_auth->token, strlen(response_auth->token));
            request_bearer->isSigned = response_approve->isSigned;
            response_bearer = bearer_1(request_bearer, client);

            if(strcmp(response_bearer->status, "REQUEST_DENIED") == 0){
                printf("REQUEST_DENIED\n");
            }
            else{
                if(request_auth->autoRefresh == 1){
                    printf("%s -> %s,%s\n", response_auth->token, response_bearer->accessToken, response_bearer->refreshToken);
                }
                else{
                    printf("%s -> %s\n", response_auth->token, response_bearer->accessToken);
                }
            }
            
            // Create and add user information in database 
            UserInfo* addedUser = createUserInfo(NULL, request_auth->clientId, response_auth->token, response_bearer->accessToken, 
                                                response_bearer->refreshToken, response_bearer->timeout, request_auth->autoRefresh);
            addInList(userInfoDB, addedUser);

            // Free the requests and responses
            free(response_auth->status);
            free(response_auth->token);
            free(response_bearer->accessToken);
            free(response_bearer->refreshToken);
            free(response_bearer->status);

            free(request_bearer->authToken);
            free(request_bearer);
            free(request_approve->authToken);
            free(request_approve);
            free(request_auth->clientId);
            free(request_auth);
        }
        else{
            // Fifth step: Validate
            // Create request
            UserInfo* user = findByUserId(userInfoDB, inputFromFile[i][0]);
            request_validate = calloc(1, sizeof(*request_validate));
            request_validate->accessToken = (char*)calloc(20, sizeof(char));
            request_validate->action = (char*)calloc(20, sizeof(char));
            request_validate->resource = (char*)calloc(20, sizeof(char));

            // If the user does not exist in the database send a dummy request
            if(user == NULL){
                strcpy(request_validate->accessToken, DEFAULT_TEXT);
                strncpy(request_validate->action, inputFromFile[i][1], strlen(inputFromFile[i][1]));
                strncpy(request_validate->resource, inputFromFile[i][2], strlen(inputFromFile[i][2]));
            }
            else{
            // Else create a request from database 
                strncpy(request_validate->accessToken, user->accessToken, strlen(user->accessToken));
                strncpy(request_validate->action, inputFromFile[i][1], strlen(inputFromFile[i][1]));
                strncpy(request_validate->resource, inputFromFile[i][2], strlen(inputFromFile[i][2]));
            }
            response_validate = validate_action_1(request_validate, client);
            printf("%s\n", response_validate->status);

            if((strcmp(response_validate->status, "PERMISSION_DENIED") == 0) 
                || (strcmp(response_validate->status, "TOKEN_EXPIRED") == 0)){
                continue;
            }
            else{
            // Decrement the number of operation: OPPERATION_NOT_PERMITTED, RESOURCE_NOT_FOUND, PERMISSION_GRANTED
                user->numberOfActions--;
            }

            // Free the request
            free(request_validate->accessToken);
            free(request_validate->action);
            free(request_validate->resource);
            free(response_validate->status);
        }

    }
    return 0;
}