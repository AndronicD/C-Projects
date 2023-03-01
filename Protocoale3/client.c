#include <stdio.h>    
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>    
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

#define CODE_LENGTH 4
#define MAX_USERNAME 100
#define MAX_PASSWORD 100

int numberCheck(char *s){
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            return 0;
        }
    }
    return 1;
}

void register_func(char* username, char* password){
    int sockfd;
    char *message;
    char *response; 
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    strcpy(json_data[0], json_serialize_to_string_pretty(root_value));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "application/json", json_data, 1, NULL, 0, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char)); 
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    if (strncmp(code, "201", 3) == 0){
        printf("Code: %s - Register succes!\n", code);
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer = strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }

    free(code);
    free(json_data[0]);
    free(json_data);
    json_value_free(root_value);
    return;
}

char** login_func(char* username, char* password){
    int sockfd;
    char **cookies = NULL;
    char *message;
    char *response;
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    strcpy(json_data[0], json_serialize_to_string_pretty(root_value));

    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "application/json", json_data, 1, NULL, 0, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char)); 
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    if (strncmp(code, "200", 3) == 0){
        printf("Code: %s - Login succes!\n", code);
        
        char *start = strstr(response, "Set-Cookie:");
        char *end = strtok(start + 12, ";");
        
        cookies = (char**)malloc(sizeof(char *));
        cookies[0] = calloc((strlen(end) + 1), sizeof(char));
        memcpy(cookies[0], start + 12, strlen(end));
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    
    free(code);
    free(json_data[0]);
    free(json_data);
    json_value_free(root_value);
    return cookies;
}

char* enter_library(char **cookies){
    int sockfd;
    char* token;
    char *message;
    char *response;
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, cookies, 1, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    if (strncmp(code, "200", 3) == 0){
        printf("%s - Library access!\n", code);
        
        char *start = basic_extract_json_response(response);
        char *end = strtok(start, "{:}\"");
        end = strtok(NULL, "{:}\"");
        
        token = calloc((strlen(end) + 1), sizeof(char));
        memcpy(token, start + 10, strlen(end));
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    
    free(code);
    free(json_data[0]);
    free(json_data);
    return token;
}

void get_books(char **cookies, char *token){
    int sockfd;
    char *message;
    char *response;
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));
    
    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, cookies, 1, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    char *no_books = strstr(response, "[]");

    if (strncmp(code, "200", 3) == 0){
        if(no_books != NULL){
            printf("No books in the library!\n");
            return;
        }
        printf("Code: %s\n", code);
        char *start = basic_extract_json_response(response);
        JSON_Value *root_value = json_parse_string(start - 1);
        JSON_Array *arr = json_value_get_array(root_value);
        int count = json_array_get_count(arr);
        
        for (int i = 0; i < count; i++){
            JSON_Object *object = json_array_get_object(arr, i);
            printf("\n");
            printf("Id: %.0f\n", json_object_get_number(object, "id"));
            printf("Title: %s\n", json_object_get_string(object, "title"));
            printf("\n");
        }
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    free(code);
    return;
}

void get_book(char **cookies, char *token, char* id){
    int sockfd;
    char *message;
    char *response;
    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    char s[BUFLEN];
    strcpy(s, "/api/v1/tema/library/books/");
    strcat(s, id);

    message = compute_get_request("34.241.4.235", s, NULL, cookies, 1, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    if (strncmp(code, "200", 3) == 0){
        printf("Code: %s\n", code);
        char *start = basic_extract_json_response(response);
        JSON_Value *root_value = json_parse_string(start - 1);
        JSON_Array *arr = json_value_get_array(root_value);
        JSON_Object *object = json_array_get_object(arr, 0);
        printf("Title: %s\n", json_object_get_string(object, "title"));
        printf("Author: %s\n", json_object_get_string(object, "author"));
        printf("Genre: %s\n", json_object_get_string(object, "genre"));
        printf("Publisher: %s\n", json_object_get_string(object, "publisher"));
        printf("Page count: %.0f\n", json_object_get_number(object, "page_count"));
        printf("\n");
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    free(code);
    return;
}

void add_book(char **cookies, char *token, char* title, char*author, char *genre, int page_count, char* publisher){
    int sockfd;
    char *message;
    char *response;
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));
    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);
    strcpy(json_data[0], json_serialize_to_string_pretty(root_value));

    message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", json_data, 1, cookies, 1, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';
    
    if (strncmp(code, "200", 3) == 0){
        printf("Code: %s - Book :\"%s\" added!\n", code, title);
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer);
    }
    free(code);
    return;
}

void delete_book(char **cookies, char *token, char *id){
    int sockfd;
    char *message;
    char *response;
    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    char string[BUFLEN];
    strcpy(string, "/api/v1/tema/library/books/");
    strcat(string, id);

    message = compute_delete_request("34.241.4.235", string, NULL, cookies, 1, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';
    
    if (strncmp(code, "200", 3) == 0){
        printf("Code: %s - Book with id:\"%s\" deleted!\n", code, id);
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    free(code);
    return;
}

void logout_func(char **cookies, char *token){
    int sockfd;
    char *message;
    char *response;
    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    
    message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", NULL, cookies, 1, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    char *code = calloc(CODE_LENGTH, sizeof(char));  
    strncpy(code, response + 9, 3);
    code[3] = '\0';

    if (strncmp(code, "200", 3) == 0){
        printf("Code: %s - Logout succes!\n", code);
        close_connection(sockfd);
    }
    else if(strncmp(code, "429", 3) == 0){
        printf("Code: %s - Too many requests! Try later!\n", code);
    }
    else{
        char *json_response = basic_extract_json_response(response);
        char *pointer = strtok(json_response, "{:}\"");
        pointer= strtok(NULL, "{:}\"");
        printf("Code: %s - Error: %s\n", code, pointer); 
    }
    free(code);
    return;
}

int main(int argc, char *argv[])
{
    char **cookies = NULL;
    char *token = NULL;
    int sockfd;

    char *username = calloc(MAX_USERNAME, sizeof(char));
    char *password = calloc(MAX_PASSWORD, sizeof(char));
    char *title = calloc(LINELEN, sizeof(char));
    char *author = calloc(LINELEN, sizeof(char));;
    char *genre = calloc(LINELEN, sizeof(char));;
    char *page_count = calloc(LINELEN, sizeof(char));;
    char *publisher = calloc(LINELEN, sizeof(char));;

    char buffer[BUFLEN];
    int status = 0;
    
    char **json_data = calloc(1, sizeof(char *));
    json_data[0] = calloc(LINELEN, sizeof(char));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    while (1)
    {
        fgets(buffer, LINELEN, stdin);
        buffer[strlen(buffer) - 1] = '\0';

        if (strncmp(buffer, "register", 8) == 0)
        {
            printf("username=");
            fgets(username, MAX_USERNAME, stdin);
            username[strlen(username) - 1] = '\0';
            
            printf("password=");
            fgets(password, MAX_PASSWORD, stdin);
            password[strlen(password) - 1] = '\0';
            
            register_func(username, password);
        }

        if (strncmp(buffer, "login", 5) == 0)
        {
            if (status == 1)
            {
                printf("You are already logged in!\n");
                continue;
            }
            
            printf("username=");
            fgets(username, MAX_USERNAME, stdin);
            username[strlen(username) - 1] = '\0';
            
            printf("password=");
            fgets(password, MAX_USERNAME, stdin);
            password[strlen(password) - 1] = '\0';

            cookies = login_func(username, password);
            if(cookies != NULL){
               status = 1; 
            }
        }
        
        if (strncmp(buffer, "enter_library", 13) == 0)
        {
           token = enter_library(cookies);
        }

        if (strcmp(buffer, "get_books") == 0)
        {
           get_books(cookies, token);
        }

        if (strcmp(buffer, "get_book") == 0)
        {   
            char *id = calloc(10, sizeof(char));
            printf("id=");
            fgets(id, 10, stdin);
            id[strlen(id) - 1] = '\0';

            if (!numberCheck(id))
            {
                printf("Wrong ID format!\n");
            }
            else{    
                get_book(cookies, token, id);
            }
            free(id);
        }

        if (strncmp(buffer, "add_book", 8) == 0)
        {
            printf("title=");
            fgets(title, LINELEN, stdin);
            title[strlen(title) - 1] = '\0';

            printf("author=");
            fgets(author, LINELEN, stdin);
            author[strlen(author) - 1] = '\0';
            
            printf("genre=");
            fgets(genre, LINELEN, stdin);
            genre[strlen(genre) - 1] = '\0';

            printf("publisher=");
            fgets(publisher, LINELEN, stdin);
            publisher[strlen(publisher) - 1] = '\0';

            printf("page_count=");
            fgets(page_count, LINELEN, stdin);
            page_count[strlen(page_count) - 1] = '\0';

            if(!numberCheck(page_count)){
                printf("Wrong page_count format!\n");
            }
            else{
                int page_count_num = atoi(page_count);
                add_book(cookies, token, title, author, genre, page_count_num, publisher);
            }    
        }
        
        if (strcmp(buffer, "delete_book") == 0)
        {
            char *id = calloc(10, sizeof(char));
            printf("id=");
            fgets(id, 10, stdin);
            id[strlen(id) - 1] = '\0';

            if (!numberCheck(id))
            {
                printf("Wrong ID format!\n");
            }
            else{
                delete_book(cookies, token, id);
            }    
            free(id);
        }

        if (strcmp(buffer, "logout") == 0)
        {
            logout_func(cookies, token);
            status = 0;
            free(token);
            free(cookies[0]);
            free(cookies);            
        }

        if (strncmp(buffer, "exit", 4) == 0)
        {
            if(status == 1){
                if(token != NULL){
                    free(token);
                }
                if(cookies[0] != NULL){
                    free(cookies[0]);
                }
                if(cookies != NULL){
                    free(cookies);
                }
            }
            break;
        }
    }

    free(username);
    free(password);
    free(title);
    free(author);
    free(genre);
    free(publisher);
    free(page_count);

    close_connection(sockfd);
    return 0;
}