// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <errno.h>

int truncate(const char *path, off_t length)
{
	/* TODO: Implement truncate(). */
	if(length < 0){
		errno = EINVAL;
		return -1;
	}

	long unsigned ret = syscall(__NR_truncate, path, length);
	if(ret > 4096UL){
		errno = -ret;
		return -1;
	}

	return (int)ret;
}
