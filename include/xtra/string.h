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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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


/**
 * @brief Implmentation of 'strdup'.
 * @return copy of string or NULL if something went wrong.
 */
char *
strdup(const char *str)
{
		size_t len = 1 + strlen(str);
		char *p = (char*)malloc(len);

		return p ? strncpy(p, str, len) : NULL;
}

/**
 * @brief Checks if string covers all of charset.
 * @warning will not care if a character not present in the charset is found.
 * @return string on success or NULL on failure.
 */
char *
strcov(char *str, const char *charset)
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


/**
 * @brief concatenate but at a give 'p' position.
 * @return `dest` string.
 */
char *
strpcat(char *dest, const char *src, size_t pos)
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
 * @brief Concatenate a char on to the end of a string.
 * @return `dest` string
 */
char *
chrcat(char *dest, const char src) 
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
