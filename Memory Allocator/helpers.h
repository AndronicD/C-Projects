/* SPDX-License-Identifier: BSD-3-Clause */

#pragma once

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ALIGNMENT 8 // must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define META_SIZE (sizeof(struct block_meta))
#define MMAP_THRESHOLD	(128 * 1024)

#define DIE(assertion, call_description)						\
	do {										\
		if (assertion) {							\
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);		\
			perror(call_description);					\
			exit(errno);							\
		}									\
	} while (0)

/* Structure to hold memory block metadata */
struct block_meta {
	size_t size;
	int status;
	struct block_meta *next;
};

/* Block metadata status values */
#define STATUS_FREE   0
#define STATUS_ALLOC  1
#define STATUS_MAPPED 2

struct block_meta *head_heap = NULL;
struct block_meta *head_map = NULL;

void init_heap() {
    head_heap = NULL;
}

void init_map(){
	head_map = NULL;
}

struct block_meta* try_expand(){
	if (head_heap == NULL) {
        return NULL;
    }

	struct block_meta *current = head_heap;
    while (current->next != NULL) {
        current = current->next;
    }

	if(current->status == STATUS_FREE){
		return current;
	}
	return NULL;
}

void *add_brk(size_t size, int status) {
	if(try_expand() != NULL){
		struct block_meta* ptr = try_expand();
		sbrk(ALIGN(size) - ALIGN(ptr->size));
		ptr->status = status;
		ptr->size = size;
		return ptr;
	}

	size_t blk_size = ALIGN(size) + ALIGN(META_SIZE);
	void *ptr = sbrk(blk_size);

	struct block_meta *new_node = ptr;
    new_node->size = size;
    new_node->status = status;
    new_node->next = NULL;

	if (head_heap == NULL) {
        head_heap = new_node;
        return new_node;
    }

	struct block_meta *current = head_heap;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
	return new_node;
}

void *add_mmap(size_t size, int status) {
	size_t blk_size = ALIGN(size) + ALIGN(META_SIZE);
	//'0', '204824', 'PROT_READ | PROT_WRITE', 'MAP_PRIVATE | MAP_ANON', '-1', '0'
	void *ptr = mmap(NULL, blk_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

	struct block_meta *new_node = ptr;
    new_node->size = size;
    new_node->status = status;
    new_node->next = NULL;

	if (head_map == NULL) {
        head_map = new_node;
        return new_node;
    }

	struct block_meta *current = head_map;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
	return new_node;
}

void remove_node_mmap(struct block_meta *node_to_remove) {
    /* If the node to remove is the head, update the head */
    if (node_to_remove == head_map) {
        head_map = node_to_remove->next;
        munmap(node_to_remove, ALIGN(META_SIZE) + ALIGN(node_to_remove->size));
        return;
    }

    struct block_meta *current = head_map;
    while (current != NULL && current->next != node_to_remove) {
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    current->next = node_to_remove->next;
	munmap(node_to_remove, ALIGN(META_SIZE) + ALIGN(node_to_remove->size));
}

void coal(struct block_meta *node_1, struct block_meta *node_2){
	node_1->size = ALIGN(node_1->size) + ALIGN(META_SIZE) + node_2->size;
	if(node_2->next != NULL){
		node_1->next = node_2->next;
		return;
	}
	node_1->next = NULL;
}
