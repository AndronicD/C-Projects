// SPDX-License-Identifier: BSD-3-Clause

#include "osmem.h"
#include "helpers.h"
#include <string.h>

int first = 1;

void coalesce(){
	void* ptr = head_heap;
	if(head_heap == NULL){
		return;
	}
	while(ptr != NULL){
		struct block_meta *header = ptr;
		if(header->status == STATUS_FREE){
			while(header->next != NULL && header->next->status == STATUS_FREE){
				coal(header, header->next);
			}
		}
		ptr = header->next;
	}
}

void try_split(struct block_meta *fp, size_t needed){
	int remaining = fp->size - needed;
	fp->status = (int)STATUS_ALLOC;

	if(remaining < (int)(ALIGN(META_SIZE) + ALIGN(1))){
		return;
	}

	fp->size = (size_t)needed;
	struct block_meta *sp = (struct block_meta *)((void *)fp + ALIGN(needed) + ALIGN(META_SIZE));
 	sp->size = (size_t)remaining - ALIGN(META_SIZE);
	sp->status = (int)STATUS_FREE;

	sp->next = fp->next;
	fp->next = sp;
}

void *findbest(size_t size){
	void* ptr = head_heap;

	if(head_heap == NULL){
		return NULL;
	}

	size_t to_fit = ALIGN(META_SIZE) + ALIGN(size);
	struct block_meta *ret = NULL;
	while(ptr != NULL){
		struct block_meta *header = ptr;
		if(header->status == STATUS_FREE){
			if(ALIGN(header->size) + ALIGN(META_SIZE) - to_fit == 0){ 
				return header;
			}
			else if((int)(ALIGN(header->size) + ALIGN(META_SIZE) - to_fit) > 0){
				if(ret != NULL){
					if((ALIGN(ret->size) + ALIGN(META_SIZE)) > (ALIGN(header->size) + ALIGN(META_SIZE))){
						ret = header;
					}
				}
				else{
					ret = header;
				}
			}
		}
		ptr = header->next;
	}
	return ret;
}

void *os_malloc(size_t size)
{
	/* TODO: Implement os_malloc */
	if(size == 0){
		return NULL;
	}

	void* ptr; 
	if(size >= MMAP_THRESHOLD){
		if(head_map == NULL){
			init_map();
		}
		ptr = add_mmap(size, STATUS_MAPPED);
		return ptr + ALIGN(META_SIZE);
	}
	else{
		if(head_heap == NULL){
			init_heap();
		}
		if(first){
			ptr = add_brk(128*1024 - ALIGN(META_SIZE), STATUS_FREE);
			first = 0;
		}
		coalesce();
		struct block_meta *find = findbest(size);
	
		if(find != NULL){
			try_split(find, size);
			return (void*)find + ALIGN(META_SIZE);
		}
		else{
			ptr = add_brk(size, STATUS_ALLOC);
			return ptr + ALIGN(META_SIZE);
		}
	}
	return NULL;
}

void os_free(void *ptr)
{
	/* TODO: Implement os_free */
	if(ptr == NULL){
		return;
	}
	struct block_meta *header = ptr - ALIGN(META_SIZE);
	if(header->status == STATUS_MAPPED){
		remove_node_mmap(header);
	}
	else if(header->status == STATUS_ALLOC){
		header->status = STATUS_FREE;
	}
}

void *os_calloc(size_t nmemb, size_t size)
{
	/* TODO: Implement os_calloc */
	//getpagesize()
	void* ptr;
	if(size == 0){
		return NULL;
	}
	if(ALIGN(size * nmemb) + ALIGN(META_SIZE) > (long unsigned int)getpagesize()){
		if(head_map == NULL){
			init_map();
		}
		void* ptr = add_mmap(size * nmemb, STATUS_MAPPED);
		memset(ptr + ALIGN(META_SIZE), 0, size*nmemb);
		return ptr + ALIGN(META_SIZE);
	}
	ptr = os_malloc(size*nmemb);
	memset(ptr, 0, size*nmemb);
	return ptr;
}

void *os_realloc(void *ptr, size_t size)
{
	/* TODO: Implement os_realloc */
	if(size == 0){
		os_free(ptr);
		return NULL;
	}

	if(ptr == NULL){
		return os_malloc(size);
	}
	return NULL;
}
