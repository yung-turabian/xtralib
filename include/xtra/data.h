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
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define MAX_DATASTRUCT_STRSIZ  100

#define MAP_MAX 100

typedef struct vec2 vec2;
struct vec2 {
		int x;
		int y;
};

vec2 vec2_init(int x, int y);

typedef struct Map Map;
struct Map {
		size_t   size;
		char  	 keys[MAP_MAX][MAX_DATASTRUCT_STRSIZ];
		int      values[MAP_MAX];
};

Map * map_init();

int map_getIndex(Map *m, char key[]);

void map_insert(Map *m, char key[], int value);

int map_get(Map *m, char key[]);

void map_print(Map *m);

// TODO: Not finished
void map_free(Map *m);


typedef struct wcs_dynarr_t wcs_dynarr_t;
struct wcs_dynarr_t {
		wchar_t **items;
		size_t size;
		size_t allocated_size;
};

wcs_dynarr_t * wcs_dynarr_init();
void wcs_dynarr_kill(wcs_dynarr_t *arr);

void wcs_dynarr_push(wcs_dynarr_t *arr, wchar_t *item);

#endif
