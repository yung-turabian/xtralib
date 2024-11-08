#ifndef XTRA_MEMORY_H
#define XTRA_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdatomic.h>

#ifndef MALLOC
#define MALLOC(size) malloc_s(size)
#endif

#ifndef CALLOC
#define CALLOC(nitems,size) calloc_s(nitems, size)
#endif

#ifndef REALLOC
#define REALLOC(ptr,size) realloc_s(ptr, size)
#endif

#ifndef MGET
#define MGET(ptr) mget_s(ptr)
#endif

#ifndef FREE
#define FREE(ptr) free_s(ptr)
#endif

void * malloc_s(size_t size);

void * calloc_s(size_t nitems, size_t size);

void * realloc_s(void *ptr, size_t size);

size_t mget_s(void *ptr);

size_t free_s(void *ptr);

size_t mget_all();


#endif
