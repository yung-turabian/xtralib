#ifndef XTRA_IO
#define XTRA_IO

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;


/* FILE HANDLING
=================*/

/**
 *	@brief View the next character in stream, desn't move cursor pointer.
 */
char 
fpeek(FILE *stream) 
{ 
		char c = fgetc(stream); 
		ungetc(c, stream); 
		return c; 
}

/**
 *	@brief View a character at position without moving pointer; 
 *	Peeks a seek.
 */
char 
fspeek(FILE *stream, long int offset, int position) 
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

// a recursive peek that goes to end of line or EOF to get # of occurences
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

// a recursive peek that goes to delimter d
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

// character count of given line
int 
fcounts(FILE *stream) 
{
	int ch, count; 
	fpos_t original_position; 
	fgetpos(stream, &original_position);
	
	count = 0;
	for (;;) {
		ch = fgetc(stream);
		++count;
		if(ch == EOF || ch =='\n') break;
	}

	fsetpos(stream, &original_position);
	
	return count;
}

int 
fcopy(FILE *fp, FILE *tmp) 
{
	int c;
	if(ftell(tmp)!=0) fseek(tmp, 0, SEEK_SET); //reset pos
	while((c=fgetc(tmp)) != EOF) {
		if(fputc(c, fp) == EOF) break;
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
// Print a number in pinary format
void 
printb(void const * const ptr) 
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

#ifdef __cplusplus
}
#endif

#endif
