/**
 * @file string.c
 * @author yung-turabian
 * @date 4 8 2024
 * @brief New functions and types related to string and string manipulation.
 * 
 * @copyright BSD-3-Clause
 */

#include <xtra/string.h>
#include <xtra/memory.h>


/**
 * @brief Implmentation of 'strdup'.
 * @return copy of string or NULL if something went wrong.
 */
char *
strdupl(const char *str)
{
		size_t len = 1 + strlen(str);
		char *p = (char*)MALLOC(len);

		return p ? strncpy(p, str, len) : NULL;
}

/**
 * @brief Checks if string covers all of charset.
 * @warning will not care if a character not present in the charset is found.
 * @return string on success or NULL on failure.
 */
char * strcov(char *str, const char *charset)
{
		int req = strlen(charset);
		int count = 0;

		char* tmp_charset = (char*)MALLOC(sizeof(char) * strlen(charset));
		strcpy(tmp_charset, charset);
		
		for(size_t i = 0; i < strlen(str); i++) {
				for(size_t j = 0; j < strlen(tmp_charset); j++) 
				{
						if(str[i] == tmp_charset[j]) {
								tmp_charset[j] = SUB;
								count++;
								break;
						}
				}
		}
		
		// cleanup
		FREE(tmp_charset);
		
		return (count == req) ? str : NULL;
}

wchar_t * wcscov(wchar_t *str, const wchar_t *charset)
{
		int req = wcslen(charset);
		int count = 0;

		wchar_t *tmp_charset = (wchar_t*)MALLOC(sizeof(wchar_t) * wcslen(charset));
		wcscpy(tmp_charset, charset);
		
		for(size_t i = 0; i < wcslen(str); i++) {
				for(size_t j = 0; j < wcslen(tmp_charset); j++) 
				{
						if(str[i] == tmp_charset[j]) 
						{
								tmp_charset[j] = SUB;
								count++;
								break;
						}
				}
		}
		
		// cleanup
		FREE(tmp_charset);
		
		return (count == req) ? str : NULL;
}


/**
 * @brief Checks to see if `string` contains only characters in `charset`.
 * @return string if true, NULL if a unexpected character is found.
 */
const char * strset(const char *string, const char *charset)
{
		for(const char *p = string; *p != '\0'; p++)
		{
				int found = 0;
				for(const char *q = charset; *q != '\0'; q++) 
				{
						if(*p == *q) {
								found = 1;
								break;
						}
				}

				if (!found)
						return NULL;
		}
		
		return string;
}

const wchar_t * wcsset(const wchar_t *string, const wchar_t *charset)
{
		for(const wchar_t *p = string; *p != L'\0'; p++)
		{
				int found = 0;
				for(const wchar_t *q = charset; *q != L'\0'; q++) 
				{
						if(*p == *q) {
								found = 1;
								break;
						}
				}

				if (!found)
						return NULL;
		}
		
		return string;
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

wchar_t *
wcspcat(wchar_t *dest, const wchar_t *src, size_t pos)
{
		size_t i = 0;
		for(; i < wcslen(src); i++)
		{
				dest[pos + i] = src[i];
		}
		
		dest[pos + i] = L'\0';
		
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
				dest[len + 1] = '\0';
		}

		return dest;
}

wchar_t *
wccat(wchar_t *dest, const wchar_t src)
{
		size_t len = wcslen(dest);

		dest[len] = src;
		dest[len + 1] = L'\0';

		return dest;
}

/**
 *	@brief Reverses the wide character string.
 *	@return on success return string.
 */
wchar_t * wcsrev(wchar_t *str)
{
		int start = 0;
		int end = wcslen(str) - 1;
		wchar_t temp;

		while(start < end)
		{
				temp = str[start];
				str[start] = str[end];
				str[end] = temp;

				start++;
				end--;
		}

		return str;
}

/**
 *	@brief Prepends a character to a wide character string.
 *	@return on success return string.
 */
wchar_t * wcspre(wchar_t *str, wchar_t chr)
{
		int length = wcslen(str);

		wchar_t *new_str = (wchar_t*)MALLOC(sizeof(wchar_t) * (length + 2));
		if(new_str == NULL) {
				return NULL;
		}

		new_str[0] = chr;

		wcscpy(new_str + 1, str);
		wcscpy(str, new_str);

		FREE(new_str);

		return str;
}

char * strpre(char *str, char chr)
{
		char *newStr = (char*)MALLOC(sizeof(char) * (strlen(str) + 2));
		if(newStr == NULL) {
				return NULL;
		}

		newStr[0] = chr;

		strcpy(newStr + 1, str);
		strcpy(str, newStr);

		FREE(newStr);

		return str;
}

char * strpres(char *str, char *pre)
{
		char *newStr = (char*)MALLOC(sizeof(char) * (strlen(str) + strlen(pre) + 1));
		if(newStr == NULL) {
				return NULL;
		}

		strcpy(newStr, pre);
		strcat(newStr, str);
		newStr[strlen(newStr) + 1] = L'\0';

		strcpy(str, newStr);

		FREE(newStr);

		return str;
}

wchar_t * wcspres(wchar_t *str, wchar_t *pre)
{
		wchar_t *newStr = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(str) + wcslen(pre) + 1));
		if(newStr == NULL) {
				return NULL;
		}

		wcscpy(newStr, pre);
		wcscat(newStr, str);
		newStr[wcslen(newStr) + 1] = L'\0';

		wcscpy(str, newStr);

		FREE(newStr);

		return str;
}

/**
 * @brief Get number of times that `ch` appears in `string`.
 * @return the number of instances.
 */
int strchrn(const char *string, char ch)
{
		unsigned long count = 0;
		unsigned long i = 0;
		for(; i < strlen(string); i++)
				if(string[i] == ch)
						count++;

		return count;
}


/**
 * @brief (Wide-char variant) Get number of times that `ch` appears in `string`.
 * @return the number of instances.
 */
int wcswcn(const wchar_t *string, wchar_t ch)
{
		unsigned long count = 0;
		unsigned long i = 0;
		for(; i < wcslen(string); i++)
				if(string[i] == ch)
						count++;

		return count;
}

/**
 *	@brief Confirms that wcstring is entirely composed on numbers.
 *
 */
wchar_t *iswcsdigit(wchar_t *string) {
		for(wchar_t *wc = string; *wc != L'\0'; wc++) {
				if(!iswdigit(*wc))
						return NULL;
		}
		return string;
}

// useful for switch statements with strings
long hash(const char* str)
{
		long hash = 5381; int c;

		while((c = *str++)) {
				hash = ((hash << 5) + hash) + c;
		}
		return hash;
}
