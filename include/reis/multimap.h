/**
 * @file data.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Includes several data strucutes such as maps, vectors, and matricies.
 *
 * @copyright BSD-3-Clause
 */

#ifndef __REISLIB_MULTIMAP_H__
#define __REISLIB_MULTIMAP_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define MULTIMAP_DEFAULT_SIZE 16
#define MULTIMAP_RESIZE_FACTOR 2

struct MultiMapNode {
		void *value;
		struct MultiMapNode *next;
};
typedef struct MultiMapNode MultiMapNode;

typedef struct {
		wchar_t  	 **keys;
		MultiMapNode **values;
		size_t   size;
		size_t capacity;
} MultiMap;

MultiMap * MultiMapInit();

void MultiMapInsert(MultiMap *m, wchar_t *key, void *value, size_t size);

void ** MultiMapGet(MultiMap *m, wchar_t *key, size_t *numValues);

void MultiMapPrint(MultiMap *m);

void MultiMapFree(MultiMap *m);

#endif
