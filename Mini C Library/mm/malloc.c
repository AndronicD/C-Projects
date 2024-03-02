// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

void *malloc(size_t size)
{
	/* TODO: Implement malloc(). */


	void* new_elem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(new_elem != MAP_FAILED){
		if(mem_list_add(new_elem, size) == -1){
			munmap(new_elem, size);
			return NULL;
		}
		return (mem_list_find(new_elem)->start != NULL) ? mem_list_find(new_elem)->start : NULL;
	}
	return NULL;
}

void *calloc(size_t nmemb, size_t size)
{
	/* TODO: Implement calloc(). */

	void* new_elem = mmap(NULL, size * nmemb, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(new_elem != MAP_FAILED){
		memset(new_elem, 0, size * nmemb);
		if(mem_list_add(new_elem, size * nmemb) == -1){
			munmap(new_elem, size * nmemb);
			return NULL;
		}
		return (mem_list_find(new_elem)->start != NULL)?mem_list_find(new_elem)->start:NULL;
	}
	return NULL;
}

void free(void *ptr)
{
	/* TODO: Implement free(). */
	if(mem_list_find(ptr)->start == ptr){
		munmap(mem_list_find(ptr)->start, mem_list_find(ptr)->len);
		if(mem_list_del(ptr) == -1){
			return;
		}
		return;
	}
	return;
}

void *realloc(void *ptr, size_t size)
{
	/* TODO: Implement realloc(). */
	if(ptr == NULL){
		ptr = malloc(size);
		return ptr;
	}
	if(mem_list_find(ptr)->start == ptr){
		void *new_ptr = NULL;
		new_ptr = mremap(ptr, sizeof(ptr), size, MREMAP_MAYMOVE);

		if (new_ptr == MAP_FAILED) {
            new_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (new_ptr == MAP_FAILED) {
                return ptr;
            }
		}

		size_t old_size = mem_list_find(ptr)->len;

		if (old_size < size) {
			memcpy(new_ptr, ptr, old_size);
		} 
		else {
			memcpy(new_ptr, ptr, size);
		}

		if(mem_list_add(new_ptr, size) == -1){
			munmap(new_ptr, size);
			return ptr;
		}
		return new_ptr;
	}
	return ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	/* TODO: Implement reallocarray(). */
	return realloc(ptr, nmemb * size);
}
