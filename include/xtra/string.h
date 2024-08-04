/**
 * @file string.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief Functions and types for xtra/string.
 *
 */


#ifndef XTRA_STRING_H
#define XTRA_STRING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SUB 0x1A // an empty char as not to interfere with '\0'

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t u64;
typedef int64_t  s64;

typedef int_fast64_t fs64;


#define CSTRING_SLICE_TYPE(TYPE)   \
		struct									       \
		{												       \
				s64  len;                  \
				TYPE *buf;						     \
		}
typedef CSTRING_SLICE_TYPE(char) slice;

#define CSTRING_SLICE_INIT(BUF, LEN) \
		{ 															 \
				.buf = (BUF), .len = (LEN)   \
		}


/** @struct
 * @brief An new revision of the string.
 *
 *
 *
 */
typedef struct xstring {
		char *ptr;
		u64 len;
} xstring;


/**
 * @brief Implmentation of 'strdup'.
 *
 * @return Copy of string or NULL if something went wrong.
 */
char
*strdup(const char *str)
{
		size_t len = 1 + strlen(str);
		char *p = (char*)malloc(len);

		return p ? strncpy(p, str, len) : NULL;
}

/** 
 * @brief Checks if string covers all of charset, will not care
 * if a character not present in charset is found.
 * @return Returns string on success or NULL on failure.
 */
char
*strcov(char *str, const char *charset)
{
		int req = strlen(charset);
		int count = 0;

		char* tmp_charset = strdup(charset);
		
		size_t i = 0;
		for(; i < strlen(str); i++)
		{
				size_t j = 0;
				for(; j < strlen(tmp_charset); j++) 
				{
						if(str[i] == tmp_charset[j]) {
								tmp_charset[j] = SUB;
								count++;
								break;
						}
				}
		}
		
		// cleanup
		free(tmp_charset);
		
		return (count == req) ? str : NULL;
}


/** @fn char* strpcat(char *dest, const char *src, size_t pos)
 * @brief concatenate but at a give 'p' position
 * @return Returns the 'dest' string.
 */
char
*strpcat(char *dest, const char *src, size_t pos)
{
		size_t i = 0;
		for(; i < strlen(src); i++)
		{
				dest[pos + i] = src[i];
		}
		
		dest[pos + i] = '\0';
		
		return dest;
}

/**
 * @brief concat a char on to the end of a string
 */
char *chrcat(char *dest, const char src) 
{
		size_t len = strlen(dest);

		if(len == 0) {
				dest[0] = src;
		}
		else {
				dest[len] = src;
				dest[len+1] = '\0';
		}

		return dest;
}

// useful for switch statements with strings
long 
hash(const char* str)
{
		long hash = 5381; int c;

		while((c = *str++)) {
				hash = ((hash << 5) + hash) + c;
		}
		return hash;
}

#ifdef __cplusplus
}
#endif

#endif
