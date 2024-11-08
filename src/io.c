/**
 * @file io.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief New functions related to file handling and writing to stdout. 
 * 
 * @copyright BSD-3-Clause
 */

#include <xtra/io.h>

/* FILE HANDLING
=================*/

/**
 *	@brief View the next character in stream, doesn't move pointer.
 *	@return character.
 */
char fpeek(FILE *stream) 
{ 
		char c = fgetc(stream); 
		ungetc(c, stream); 
		return c; 
}

/**
 *	@brief Same as `fpeek` but for wchar_t.
 *
 */
wchar_t fpeek_wc(FILE *stream) 
{ 
		wchar_t c = fgetwc(stream); 
		ungetwc(c, stream); 
		return c; 
}

/**
 *	@brief View a character at position without moving pointer; Peeks a seek.
 *	@return seeked and peeked character.
 */
char fspeek(FILE *stream, long int offset, int position) 
{
	char c;
	fpos_t original_position;
	fgetpos(stream, &original_position);
	
	fseek(stream, offset, position);
	c = fgetc(stream);
	ungetc(c, stream);

	fsetpos(stream, &original_position);

	return c;
}

/**
 * @brief A recursive peek that goes to end of line or EOF
 * to get # of occurences.
 */
int 
frpeek(FILE *stream, char c) 
{
	int count = 0;
	char cc = c;
	fpos_t original_position;
	fgetpos(stream, &original_position);

	while (cc == c) {
		if(cc == EOF || cc =='\n') break;
		cc = fgetc(stream);
		++count;
	}

	fsetpos(stream, &original_position);
	
	return count;
}

/** 
 * @brief A recursive peek that goes till the delimter `d`.
 */
int 
frdpeek(FILE *stream, char d) 
{
	int count = 0;
	char c = '\0';
	fpos_t original_position;
	fgetpos(stream, &original_position);

	while (c != d) {
		if(c == EOF || c =='\n') {
			count = -1;
			break;
		}
		c = fgetc(stream);
		++count;
	}

	fsetpos(stream, &original_position);
	
	return count;
}

/**
 * @brief Character count of current line of buffer.
 */
int 
fcounts(FILE *stream) 
{
	int ch, count; 
	fpos_t original_position; 
	fgetpos(stream, &original_position);
	
	count = 0;
	while((ch = fgetc(stream) != EOF || ch != '\n'))
		++count;

	fsetpos(stream, &original_position);
	
	return count;
}

/**
 * @brief Copies data from `src` file tp `dest` file.
 */
int 
fcopy(FILE *dest, FILE *src) 
{
	int c;
	if(ftell(src) != 0) fseek(src, 0, SEEK_SET); //reset pos
	while((c = fgetc(src)) != EOF) {
		if(fputc(c, dest) == EOF) break;
	}
	return 1;
}

/* STDIN
========*/

// get string, often a buffer, pass length as well
// A safe way to read input that ensures no misc LF or breaks in read string
void 
sgets(char* str, int n)
{
		char* str_read = fgets(str, n, stdin);
		if(!str_read) return;

		int i=0;
		while(str[i] != '\n' && str[i] != '\0')
		{
				i++;
		}

		if(str[i] == '\n') str[i] = '\0';
}

/* STDOUT
==========*/

// WIP
// Print a number in binary format
void printb(void const * const ptr) 
{
	u8 *b = (u8*) ptr;
	u8 byte;
	int i, j;
	
  for (i = 0; i >= 0; i--) {
      for (j = 7; j >= 0; j--) {
          byte = (b[i] >> j) & 1;
          printf("%u", byte);
      }
  }
}



/**
 * @brief Makes writing error to stderr slightly simpler.
 */
CHECK_PRINTF_FMT(1, 2) void 
eprintf(const char *fmt, ...)
{
		va_list vargs;
		va_start(vargs, fmt);
		vfprintf(stderr, fmt, vargs);
		va_end(vargs);
}

/**
 * @brief Easy color printing.
 */
CHECK_PRINTF_FMT(1, 3) void
cprintf(const char * color, const char * fmt, ...)
{
		printf("%s", color);
		va_list vargs;
		va_start(vargs, fmt);
		vfprintf(stdout, fmt, vargs);
		va_end(vargs);
		printf(ANSI_RESET);
}

