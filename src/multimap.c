/**
 * @file multimap.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Not optimized at all.
 * 
 * @todo All pretty unsafe, same with the hashtable, next step is making a 
 * homebrew Template system, or just porting code to C++ and start using that
 * anyway.
 *
 * @copyright BSD-3-Clause
 */

#include <xtra/multimap.h>
#include <xtra/memory.h>

MultiMap * MultiMapInit()
{
		MultiMap *m = (MultiMap*)MALLOC(sizeof(MultiMap));
		m->keys = (wchar_t**)CALLOC(MULTIMAP_DEFAULT_SIZE, sizeof(wchar_t*));
		m->values = (MultiMapNode**)CALLOC(MULTIMAP_DEFAULT_SIZE, sizeof(MultiMapNode*));
		m->size = 0;
		m->capacity = MULTIMAP_DEFAULT_SIZE;
		return m;
}

static int _MultiMapGetIndex(MultiMap *m, wchar_t *key)
{
		for(size_t it = 0; it < m->size; it++)
				if(wcscmp(m->keys[it], key) == 0)
						return it;
		return -1;
}

static void _MultiMapResize(MultiMap *m)
{
		size_t newCapacity = m->capacity * MULTIMAP_RESIZE_FACTOR;
		wchar_t **newKeys = (wchar_t**)CALLOC(newCapacity, sizeof(wchar_t*));
		MultiMapNode **newValues = (MultiMapNode**)CALLOC(newCapacity, sizeof(MultiMapNode*));

    if (!newKeys || !newValues) {
        fprintf(stderr, "Failed to allocate memory for resizing.\n");
        exit(EXIT_FAILURE);
    }

		for (size_t i = 0; i < m->size; i++) {
				newKeys[i] = m->keys[i];
				newValues[i] = (MultiMapNode*)MALLOC(sizeof(MultiMapNode));
				MultiMapNode *newNode = newValues[i];
				MultiMapNode *oldNode = m->values[i];
				while (oldNode != NULL) {
						newNode->value = oldNode->value;
						if (oldNode->next != NULL) {
								newNode->next = (MultiMapNode*)MALLOC(sizeof(MultiMapNode));
						} else {
								newNode->next = NULL;
						}
						newNode = newNode->next;
						oldNode = oldNode->next;
				}
		}

		// Free old memory
    FREE(m->keys);
    FREE(m->values);

    m->keys = newKeys;
    m->values = newValues;
    m->capacity = newCapacity;
}

void MultiMapInsert(MultiMap *m, wchar_t *key, void *value, size_t size)
{
		if(m->size >= m->capacity)
				_MultiMapResize(m);

		int index = _MultiMapGetIndex(m, key);
		if(index != -1) {
				MultiMapNode *newNode = m->values[index];
				while(newNode->next != NULL) {
						newNode = newNode->next;
				}

        newNode->next = (MultiMapNode*)MALLOC(sizeof(MultiMapNode));
				newNode->next->value = MALLOC(size);
        memcpy(newNode->next->value, value, size);
        newNode->next->next = NULL;
		}
		else {

				m->keys[m->size] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(key) + 1));
				wcscpy(m->keys[m->size], key);
				m->values[m->size] = (MultiMapNode*)MALLOC(sizeof(MultiMapNode));
        m->values[m->size]->value = MALLOC(size);
        memcpy(m->values[m->size]->value, value, size);
        m->values[m->size]->next = NULL;
				m->size++;
		}
}

void ** MultiMapGet(MultiMap *m, wchar_t *key, size_t *numValues)
{
    int index = _MultiMapGetIndex(m, key);
		if(index == -1) {
				*numValues = 0;
				return NULL;
		}

    MultiMapNode *node = m->values[index];
		size_t count = 0;
		while(node) {
				count++;
				node = node->next;
		}

		void **valuesArray = (void**)CALLOC(count, sizeof(void*));
		if(!valuesArray) {
				*numValues = 0;
				return NULL;
		}


		node = m->values[index];
		size_t i = 0;
		while(node) {
				valuesArray[i++] = node->value;
				node = node->next;
		}

		*numValues = count;
		return valuesArray;
}

void MultiMapPrint(MultiMap *m)
{
    for(size_t it = 0; it < m->size; it++) {
        MultiMapNode *node = m->values[it];
        fprintf(stdout, "%ls: ", m->keys[it]);
        while (node) {
            fprintf(stdout, "%ls ", (wchar_t*)node->value); // Assuming values are pointers
            node = node->next;
        }
        fprintf(stdout, "\n");
    }
}


void MultiMapFree(MultiMap *m)
{
    for(size_t it = 0; it < m->size; it++) {
        MultiMapNode *node = m->values[it];

				while(node != NULL) {
						MultiMapNode *next = node->next;
						
						FREE(node->value);

						FREE(node);
						node = next;
				}
				FREE(m->keys[it]);
    }
    FREE(m->keys);
		FREE(m->values);
    FREE(m);
}
