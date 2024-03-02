/* SPDX-License-Identifier: BSD-3-Clause */

#include <string.h>

char *strcpy(char *destination, const char *source)
{
	if (destination == NULL) {
        return NULL;
    }
 
    char *ptr = destination;
 
    while (*source)
    {
        *destination = *source;
        destination++;
        source++;
    }
 
    *destination = '\0';
 	return ptr;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	if (destination == NULL) {
        return NULL;
    }
 
    char* ptr = destination;
 
    int j = strlen(source);
    for(int i = 0; i <= j && i < (int)len; i++)
    {
        *destination = *source;
        destination++;
        source++;
    }
 
	if(*destination != '\0'){
		return ptr;
	}
	*destination = '\0';
    return ptr;
}

char *strcat(char *destination, const char *source)
{
	if (destination == NULL) {
        return NULL;
    }

	char* ptr = destination;
	while(*destination){
		destination++;
	}

	while(*source){
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';
	return ptr;
}

char *strncat(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncat(). */
	if (destination == NULL) {
        return NULL;
    }

	char* ptr = destination;
	while(*destination){
		destination++;
	}
	while(*source && len--){
		*destination = *source;
		destination++;
		source++;
	}
	*destination = '\0';
	return ptr;
}

int strcmp(const char *str1, const char *str2)
{
	/* TODO: Implement strcmp(). */
	while(*str1 || *str2){
		if(*str1 == *str2){
			str1++;
			str2++;
			continue;
		}
		else if(*str1 > *str2){
			return 1;
		}
		else{
			return -1;
		}
	}
	return 0;
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	/* TODO: Implement strncmp(). */
	while((*str1 || *str2) && len--){
		if(*str1 == *str2){
			str1++;
			str2++;
			continue;
		}
		else if(*str1 > *str2){
			return 1;
		}
		else{
			return -1;
		}
	}
	return 0;
}

size_t strlen(const char *str)
{
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	/* TODO: Implement strchr(). */
	void* ptr = NULL;
	while(*str){
		if(*str == c){
			ptr = str;
			break;
		}
		str++;
	}
	if(ptr == NULL){
		return NULL;
	}
	return (char*)ptr;
}

char *strrchr(const char *str, int c)
{
	/* TODO: Implement strrchr(). */
	void* ptr = NULL;
	while(*str){
		if(*str == c){
			ptr = str;
		}
		str++;
	}
	if(ptr == NULL){
		return NULL;
	}
	return (char*)ptr;
}

char *strstr(const char *str1, const char *str2)
{
	/* TODO: Implement strstr(). */
	void* ptr = NULL;
	while(*str1){
		if(!strncmp(str1, str2, strlen(str2))){
			ptr = str1;
			break;
		}
		str1++;
	}
	if(ptr == NULL){
		return NULL;
	}
	return (char*)ptr;
}

char *strrstr(const char *str1, const char *str2)
{
	/* TODO: Implement strrstr(). */
	void* ptr = NULL;
	while(*str1){
		if(!strncmp(str1, str2, strlen(str2))){
			ptr = str1;
		}
		str1++;
	}
	if(ptr == NULL){
		return NULL;
	}
	return (char*)ptr;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memcpy(). */
	if (destination == NULL) {
        return NULL;
    }

	char* str1 = (char*)destination;
	char* str2 = (char*)source;

	if (str1 == NULL) {
        return NULL;
    }
 
    void *ptr = str1;

    int j = strlen(str2);
    for(int i = 0; i <= j && i < (int)num; i++)
    {
        *str1 = *str2;
        str2++;
        str1++;
    }
 
	if(*str1 != '\0'){
		return ptr;
	}
    *str1 = '\0';
 
    return ptr;
}

void *memmove(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memmove(). */
	if (destination == NULL) {
        return NULL;
    }
	
	char* str1 = (char*)destination;
	char* str2 = (char*)source;

	if (str1 == NULL) {
        return NULL;
    }
 	
	void *ptr = str1;
 
    while((*str1 || *str2) && num--)
    {
        *str1 = *str2;
        str2++;
        str1++;
    }

	if(*str1 != '\0'){
		return ptr;
	}
    *str1 = '\0';

	return ptr;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	/* TODO: Implement memcmp(). */
	char* str1 = (char*)ptr1;
	char* str2 = (char*)ptr2;

	while((*str1 || *str2) && num--){
		if(*str1 == *str2){
			str1++;
			str2++;
			continue;
		}
		else if(*str1 > *str2){
			return 1;
		}
		else{
			return -1;
		}
	}
	return 0;
}

void *memset(void *source, int value, size_t num)
{
	/* TODO: Implement memset(). */
	void* ptr = source;
	char* str = (char*)source;
	while(num--){
		*str = (unsigned char)value;
		str++;
	}
	return ptr;
}
