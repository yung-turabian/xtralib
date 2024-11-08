/**
 * @file data.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Includes several data strucutes such as maps, vectors, and matricies.
 *
 * @copyright BSD-3-Clause
 */

#include <xtra/data.h>
#include <xtra/memory.h>

// VEC2

vec2
vec2_create(int x, int y)
{
		vec2 v;
		v.x = x; v.y = y;
		return v;
}

// MAP

Map * MapInit()
{
		Map *m = (Map*)MALLOC(sizeof(Map));

		m->size = 0;

		return m;
}

static int _MapGetIndex(Map *m, char key[])
{
		for(size_t it = 0; it < m->size; it++)
				if(strcmp(m->keys[it], key) == 0)
						return it;
		return -1;
}

void MapInsert(Map *m, char key[], int value)
{
		int index = _MapGetIndex(m, key);
		if(index != -1)
				m->values[index] = value;
		else {
				strcpy(m->keys[m->size], key);
				m->values[m->size] = value;
				m->size++;
		}
}

int MapGet(Map *m, char key[])
{
		int index = _MapGetIndex(m, key);
		if(index != -1)
				return m->values[index];
		else
				return -1;
}

void MapPrint(Map *m)
{
		for(size_t it = 0; it < m->size; it++)
				fprintf(stdout, "%s: %d\n", m->keys[it], m->values[it]);
}


// TODO: Not finished
void MapFree(Map *m)
{
		FREE(m);
}

// DYN-ARRAY

wcs_dynarr_t * wcs_dynarr_init()
{
		wcs_dynarr_t *a = NULL;
		a = (wcs_dynarr_t*)MALLOC(sizeof(wcs_dynarr_t));

		a->items = (wchar_t**)MALLOC(sizeof(wchar_t*));
		a->size = 0;
		a->allocated_size = 1;

		return a;
}

void wcs_dynarr_kill(wcs_dynarr_t *a)
{
		for(size_t i = 0; i < a->size; i++)
		{
				FREE(a->items[i]);
				a->items[i] = NULL;
		}

		FREE(a->items);
		FREE(a);
		a = NULL;
}

void wcs_dynarr_push(wcs_dynarr_t *a, wchar_t *item)
{
		if(a->size >= a->allocated_size) {
						a->allocated_size *= 2;
						a->items = (wchar_t**)REALLOC(a->items,
										sizeof(wchar_t*) * a->allocated_size);
		}

		a->items[a->size] = (wchar_t*)MALLOC(MAX_DATASTRUCT_STRSIZ * sizeof(wchar_t));
		wcscpy(a->items[a->size], item);

		a->size++;
}
