/**
 * @file data.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Includes several data strucutes such as maps, vectors, and matricies.
 *
 * @copyright BSD-3-Clause
 */

#include <xtra/data.h>

// VEC2

vec2
vec2_create(int x, int y)
{
		vec2 v;
		v.x = x; v.y = y;
		return v;
}

// MAP

Map *
map_init()
{
		Map *m = NULL;
		m = (Map*)malloc(sizeof(Map));
		m->size = 0;

		return m;
}

int
map_getIndex(Map *m, char key[])
{
		for(size_t it = 0; it < m->size; it++)
				if(strcmp(m->keys[it], key) == 0)
						return it;
		return -1;
}

void
map_insert(Map *m, char key[], int value)
{
		int index = map_getIndex(m, key);
		if(index != -1)
				m->values[index] = value;
		else {
				strcpy(m->keys[m->size], key);
				m->values[m->size] = value;
				m->size++;
		}
}

int
map_get(Map *m, char key[])
{
		int index = map_getIndex(m, key);
		if(index != -1)
				return m->values[index];
		else
				return -1;
}

void
map_print(Map *m)
{
		for(size_t it = 0; it < m->size; it++)
				fprintf(stdout, "%s: %d\n", m->keys[it], m->values[it]);
}


// TODO: Not finished
void
map_free(Map *m)
{
		free(m);
}

// DYN-ARRAY

wcs_dynarr_t *
wcs_dynarr_init()
{
		wcs_dynarr_t *a = NULL;
		a = (wcs_dynarr_t*)malloc(sizeof(wcs_dynarr_t));

		a->items = (wchar_t**)malloc(sizeof(wchar_t*));
		a->size = 0;
		a->allocated_size = 1;

		return a;
}

void
wcs_dynarr_kill(wcs_dynarr_t *a)
{
		for(size_t i = 0; i < a->size; i++)
		{
				free(a->items[i]);
				a->items[i] = NULL;
		}

		free(a);
		a = NULL;
}

void
wcs_dynarr_push(wcs_dynarr_t *a, wchar_t *item)
{
		if(a->size >= a->allocated_size) {
						a->allocated_size *= 2;
						a->items = (wchar_t**)realloc(a->items,
										sizeof(wchar_t*) * a->allocated_size);
		}

		a->items[a->size] = (wchar_t*)malloc(MAX_DATASTRUCT_STRSIZ * sizeof(wchar_t));
		a->items[a->size] = wcsdup(item);

		a->size++;
}
