#include <time.h>
#include <internal/syscall.h>
#include <unistd.h>

unsigned int sleep(unsigned int seconds)
{
    struct timespec tv = {seconds, 0};
    if(nanosleep(&tv, &tv)){
        return tv.tv_sec;
    }
    return 0;
}