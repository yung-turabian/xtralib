/**
 * @file io.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief New functions related to file handling and writing to stdout. 
 * 
 * @copyright BSD-3-Clause
 */

#ifndef XTRA_IO_H
#define XTRA_IO_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>


#define ANSI_RED "\x1b[31m" // ANSI code used for color printing to stdout
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ASNI_WHITE "\x1b[37m"

#define ANSI_RESET "\x1b[0m" // ANSI reset needed to be called after color change

#if defined(__GNUC__) || defined(__clang__)
		#define CHECK_PRINTF_FMT(a, b) __attribute__ ((format (printf, a, b)))
#else 
		#define CHECK_PRINTF_FMT(...)
#endif


//#define cprintfb(fg, bg, format, ...) fprintf(stdout, fg bg format ANSI_RESET, \
//				"\x1b[%dm\x1b[%dm%s%s", 30+fg, 40+bg,text,ANSI_RESET)

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;


/* FILE HANDLING
=================*/

/**
 *	@brief View the next character in stream, doesn't move pointer.
 *	@return character.
 */
static inline char 
fpeek(FILE *stream) 
{ 
		char c = fgetc(stream); 
		ungetc(c, stream); 
		return c; 
}

/**
 *	@brief View a character at position without moving pointer; Peeks a seek.
 *	@return seeked and peeked character.
 */
static inline char 
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

/**
 * @brief A recursive peek that goes to end of line or EOF
 * to get # of occurences.
 */
static inline int 
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
static inline int 
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
static inline int 
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
static inline int 
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
static inline void 
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

static inline char *
xtee(size_t size)
{
		int shmid = shmget(IPC_PRIVATE, size + 1, 0660 | IPC_CREAT);
		int pipe_fds[2];
		pipe(pipe_fds);

		switch(fork())
		{
				case -1:
						perror("fork");
						exit(EXIT_FAILURE);
				case 0: {
						char *out = (char*)shmat(shmid, 0, 0), c;
						size_t it = 0;
						out[0] = 0;
						dup2(pipe_fds[0], 0);  // redirects pipe to child's stdin
						setvbuf(stdout, 0, _IONBF, 0);
						while(read(0, &c, 1) == 1 && it < size) {
								printf("%c", c);
								out[it++] = c;
								out[it] = 0;
						}

						_exit(EXIT_SUCCESS); // exit "immediatly"
				}

				default: //parent
						dup2(pipe_fds[1], 1); // replace stdout with output to child
						setvbuf(stdout, 0, _IONBF, 0);
						return (char*)shmat(shmid, 0, 0); // return child's capture buffer
		}
}

/* STDOUT
==========*/

// WIP
// Print a number in pinary format
static inline void 
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



/**
 * @brief Makes writing error to stderr slightly simpler.
 */
CHECK_PRINTF_FMT(1, 2) static inline void 
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
CHECK_PRINTF_FMT(1, 3) static inline void
cprintf(const char * color, const char * fmt, ...)
{
		printf("%s", color);
		va_list vargs;
		va_start(vargs, fmt);
		vfprintf(stdout, fmt, vargs);
		va_end(vargs);
		printf(ANSI_RESET);
}



#ifdef __cplusplus
}
#endif

#endif
