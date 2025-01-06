/**
 * @file string.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief New functions and types related to string and string manipulation.
 * 
 * @copyright BSD-3-Clause
 */
#ifndef __REISLIB_STRING_H__
#define __REISLIB_STRING_H__

#include <string.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <wchar.h>
#elif defined(__linux__)
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#endif

#define MAX_CHARSET_SIZE 256
#define SUB 0x1A // an empty char as not to interfere with '\0'

#ifdef __cplusplus
extern "C" {
#endif

char * strdupl(const char *str);

char * strcov(char *str, const char *charset);
wchar_t * wcscov(wchar_t *str, const wchar_t *charset);


const char * strset(const char *string, const char *charset);
const wchar_t * wcsset(const wchar_t *string, const wchar_t *charset);


char * strpcat(char *dest, const char *src, size_t pos);
wchar_t * wcspcat(wchar_t *dest, const wchar_t *src, size_t pos);

char * chrcat(char *dest, const char src);
wchar_t * wccat(wchar_t *dest, const wchar_t src);

int strchrn(const char *string, char ch);
int wcswcn(const wchar_t *string, wchar_t ch);


wchar_t * wcsrev(wchar_t *str);


char * strpre(char *str, char chr);
wchar_t * wcspre(wchar_t *str, wchar_t chr);


char * strpres(char *str, char* pre);
wchar_t * wcspres(wchar_t *str, wchar_t *pre);


char *strisdigit(char *str);
wchar_t *wcsisdigit(wchar_t *str);


long hash(const char* str);


#ifdef __cplusplus
}
#endif

#endif
