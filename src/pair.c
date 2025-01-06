#include <reis/pair.h>

#include <reis/memory.h>

Pair * PairCreate(wchar_t *first, wchar_t *second)
{
		Pair *p = (Pair*)MALLOC(sizeof(Pair));
		if(p == NULL)
				return NULL;

		p->first = MALLOC(sizeof(wchar_t) * (wcslen(first) + 1));
		p->second = MALLOC(sizeof(wchar_t) * (wcslen(second) + 1));
		if(p->first == NULL || p->second == NULL)
				return NULL;

		wcscpy(p->first, first);
		wcscpy(p->second, second);
		return p;
}

void PairFree(Pair *p) 
{
		FREE(p->first);
		FREE(p->second);
		FREE(p);
}
