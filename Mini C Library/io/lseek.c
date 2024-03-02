// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <fcntl.h>
#include <errno.h>

off_t lseek(int fd, off_t offset, int whence)
{
	/* TODO: Implement lseek(). */
	unsigned long flag = syscall(__NR_fcntl, fd, F_GETFD);
	if(flag > 4096UL){
		errno = EBADF;
		return -1;
	}

	if(whence < 0){
		errno = EINVAL;
		return -1;
	}

	long unsigned ret = syscall(__NR_lseek, fd, offset, whence);
	if(ret > 4096UL){
		errno = -ret;
		return -1;
	}

	return (off_t)ret;
}
