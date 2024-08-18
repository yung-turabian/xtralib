#ifndef XTRA_MEMORY_H
#define XTRA_MEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>

#ifndef MALLOC
#define MALLOC(size) mem_alloc(size)
#endif

#ifndef FREE
#define FREE(ptr) mem_free(ptr)
#endif

static atomic_size_t total_allocated_bytes = 0;

void * mem_alloc(size_t size);

void mem_free(void *ptr);


#endif
