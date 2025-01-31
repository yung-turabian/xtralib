#include "../include/reis.h"

static atomic_size_t totalAllocatedBytes = 0;

/**
 *	@warning Must be freed with `free_s`.
 */
void *reisMalloc(size_t size) {
  size_t *ptr = (size_t *)malloc(size + sizeof(size_t));
  if (ptr == NULL) {
    fprintf(stderr, "Error: Memory allocation failed for size %zu\n", size);
    exit(EXIT_FAILURE);
  }

  *ptr = size;

  atomic_fetch_add(&totalAllocatedBytes, size);

  return (void *)(ptr + 1);
}

/**
 *	@warning Must be freed with `free_s`.
 */
void *reisCalloc(size_t nitems, size_t size) {
  size_t totalSize = nitems * size;

  size_t *ptr = (size_t *)malloc(totalSize + sizeof(size_t));
  if (ptr == NULL) {
    fprintf(stderr, "Error: Memory allocation failed for size %zu\n", size);
    exit(EXIT_FAILURE);
  }

  *ptr = totalSize;

  memset(ptr + 1, 0, totalSize);

  atomic_fetch_add(&totalAllocatedBytes, totalSize);

  return (void *)(ptr + 1);
}

void *reisRealloc(void *ptr, size_t size) {
  size_t *oldSizePtr = (size_t *)ptr - 1;
  size_t oldSize = *oldSizePtr;

  size_t *newPtr = (size_t *)realloc(oldSizePtr, size + sizeof(size_t));
  if (newPtr == NULL) {
    fprintf(stderr, "Error: Memory reallocation failed for size %zu\n", size);
    exit(EXIT_FAILURE);
  }

  *newPtr = size;

  atomic_fetch_sub(&totalAllocatedBytes, oldSize);
  atomic_fetch_add(&totalAllocatedBytes, size);

  return (void *)(newPtr + 1);
}

size_t reisMemGet(void *ptr) {
  size_t *size_ptr = (size_t *)ptr - 1;
  return *size_ptr;
}

size_t reisFree(void *ptr) {
  size_t *size_ptr = (size_t *)ptr - 1;
  size_t size = *size_ptr;

  atomic_fetch_sub(&totalAllocatedBytes, size);

  free(size_ptr);
  return size;
}

size_t reisMemGetAll() { return totalAllocatedBytes; }