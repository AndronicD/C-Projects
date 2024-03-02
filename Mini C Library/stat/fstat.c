// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <errno.h>

int fstat(int fd, struct stat *st)
{
	/* TODO: Implement fstat(). */
	if (fd < 0){ 
		errno = EBADF;
		return -1;
	}
	return fstatat(fd, "", st, 0X1000);
}
