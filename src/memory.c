#include <xtra/memory.h>

void * mem_alloc(size_t size)
{
    size_t *ptr = (size_t*)malloc(size + sizeof(size_t));
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for size %zu\n", size);
        exit(EXIT_FAILURE);
    } 

		*ptr = size;

    atomic_fetch_add(&total_allocated_bytes, size);

    return (void*)(ptr + 1);
}

void mem_free(void *ptr)
{
		if(ptr == NULL) {
				fprintf(stderr, "Error: Attempt to free invalid or already freed pointer\n");
        return;	
		}

		size_t *size_ptr = (size_t*)ptr - 1;
		size_t size = *size_ptr;

    fprintf(stdout, "FREED: %zu bytes\n", size);
    atomic_fetch_sub(&total_allocated_bytes, size);
		
		free(size_ptr);
}
