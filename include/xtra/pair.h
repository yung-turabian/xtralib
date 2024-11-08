/**
 *
 *
 *
 */
#ifndef __XTRA_PAIR_H__
#define __XTRA_PAIR_H__

#include <stdlib.h>
#include <xtra/common.h>
#include <wchar.h>

typedef struct {
		wchar_t *first;
		wchar_t *second;
} Pair;


Pair * PairCreate(wchar_t *first, wchar_t *second);
void PairFree(Pair *p);

#endif
