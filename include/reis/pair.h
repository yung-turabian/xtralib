/**
 *
 *
 *
 */
#ifndef __REISLIB_PAIR_H__
#define __REISLIB_PAIR_H__

#include <stdlib.h>
#include <reis/common.h>
#include <wchar.h>

typedef struct {
		wchar_t *first;
		wchar_t *second;
} Pair;


Pair * PairCreate(wchar_t *first, wchar_t *second);
void PairFree(Pair *p);

#endif
