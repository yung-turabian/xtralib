/**
 * @file data.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Includes several data strucutes such as maps, vectors, and matricies.
 * 
 * @copyright BSD-3-Clause
 */

#ifndef XTRA_DATASTRUCTURES_H
#define XTRA_DATASTRUCTURES_H

#include <string.h>

#define STRSIZ  100
#define MAP_MAX 100


typedef struct vec2 {
		int x;
		int y;
} vec2;

vec2
vec2_create(int x, int y)
{
		vec2 v;
		v.x = x; v.y = y;
		return v;
}

typedef struct Map {
		size_t   size;
		char  	 keys[MAP_MAX][STRSIZ];
		int      values[MAP_MAX];
} Map;

int
map_getIndex(Map *m, char key[])
{
		size_t it = 0;
		for(;it < m->size; it++)
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
		int it = 0;
		for(;it < m->size;it++)
				fprintf(stdout, "%s: %d\n", m->keys[it], m->values[it]);
}

Map
*map_init()
{
		Map *m = NULL;
		m = (Map*)malloc(sizeof(Map));
		m->size = 0;

		return m;
}

// TODO: Not finished
void
map_free(Map *m)
{
		free(m);
}


#endif 
