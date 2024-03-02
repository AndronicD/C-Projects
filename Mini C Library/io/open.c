// SPDX-License-Identifier: BSD-3-Clause

#include <fcntl.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>
#include <../include/internal/types.h>
#include <string.h>
#include <../include/internal/io.h>

int open(const char *filename, int flags, ...)
{
	/* TODO: Implement open system call. */
	va_list va;
	mode_t mode = 0;
	long unsigned fd;

	if (flags & O_CREAT) {

		va_start(va, flags);
		mode = va_arg(va, mode_t);
		va_end(va);
	}

	fd = syscall(__NR_open, filename, flags, mode);

	if(fd >= 4096UL){
		errno = -fd;
		return -1;
	}
	return (int)fd;
}
