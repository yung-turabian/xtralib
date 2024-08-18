/**
 * @file string.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief New functions and types related to string and string manipulation.
 * 
 * @copyright BSD-3-Clause
 */


#ifndef XTRA_STRING_H
#define XTRA_STRING_H

#include <string.h>
#include <wchar.h>
#include <stdlib.h>

#define MAX_CHARSET_SIZE 256
#define SUB 0x1A // an empty char as not to interfere with '\0'

#ifdef __cplusplus
extern "C" {
#endif

#define CSTRING_SLICE_TYPE(TYPE) \
		struct	\
		{	\
				unsigned long long  len; \
				TYPE *buf; \
		}
typedef CSTRING_SLICE_TYPE(char) slice;

#define CSTRING_SLICE_INIT(BUF, LEN) \
		{ \
				.buf = (BUF), .len = (LEN) \
		}


/** @struct
 * @brief An new revision of the string.
 */
typedef struct xstring {
		char *ptr;
		unsigned long long len;
} xstring;


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


long hash(const char* str);


#ifdef __cplusplus
}
#endif

#endif
