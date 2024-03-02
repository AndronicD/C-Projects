// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <fcntl.h>
#include <internal/syscall.h>
#include <errno.h>

int ftruncate(int fd, off_t length)
{
	/* TODO: Implement ftruncate(). */
	if(length < 0){
		errno = EINVAL;
		return -1;
	}

	unsigned long flag = syscall(__NR_fcntl, fd, F_GETFD);
	if(flag > 4096UL){
		errno = EBADF;
		return -1;
	}

	long unsigned ret = syscall(__NR_ftruncate, fd, length);
	if(ret > 4096UL){
		errno = -ret;
		return -1;
	}

	return (int)ret;
}
