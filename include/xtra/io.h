/**
 * @file io.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief New functions related to file handling and writing to stdout. 
 * 
 * @copyright BSD-3-Clause
 */

#ifndef __XTRA_IO_H__
#define __XTRA_IO_H__

#include "common.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>


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


char fpeek(FILE *stream);
wchar_t fpeek_wc(FILE *stream);

char fspeek(FILE *stream, long int offset, int position);

int frpeek(FILE *stream, char c);

int frdpeek(FILE *stream, char d) ;

int fcounts(FILE *stream);

int fcopy(FILE *dest, FILE *src);

int fexists(const char *file);

int fmove(const char *oldpath, char *newpath);


/* STDIN
========*/

void sgets(char* str, int n);

bool promptYesOrNo(const char *question);


/* STDOUT
==========*/

// WIP
void printb(void const * const ptr);


CHECK_PRINTF_FMT(1, 2) void eprintf(const char *fmt, ...);

CHECK_PRINTF_FMT(1, 3) void cprintf(const char * color, const char * fmt, ...);


#ifdef __cplusplus
}
#endif

#endif
