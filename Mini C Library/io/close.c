// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>

int close(int fd)
{
	/* TODO: Implement close(). */
	unsigned long flag = syscall(__NR_fcntl, fd, F_GETFD);
	if(flag > 4096UL){
		errno = EBADF;
		return -1;
	}
	return (int)syscall(__NR_close, fd);
}
