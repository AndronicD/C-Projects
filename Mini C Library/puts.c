#include <stdio.h>
#include <internal/io.h>
#include <internal/types.h>

int putc(int c){
    if (write(1, &c, 1) != 1) {
        return EOF;
    }
    return c;
}

int puts(const char* string){
    while(*string) 
    {
        if(putc(*string) == EOF)  
        { 
            return EOF;
        }
        string++;
    }
    if(putc('\n') == EOF) 
    {
       return EOF;
    }
    return 1;
}