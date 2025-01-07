/**
 * @file io.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief New functions related to file handling and writing to stdout. 
 * 
 * @copyright BSD-3-Clause
 */

#include <reis/io.h>

/* FILE HANDLING
=================*/

/**
 *	@brief View the next character in stream, doesn't move pointer.
 *	@return character.
 */
char fpeek( FILE *stream )
{ 
		char c = fgetc(stream); 
		ungetc(c, stream); 
		return c; 
}

/**
 *	@brief Same as `fpeek` but for wchar_t.
 *
 */
wchar_t fpeek_wc( FILE *stream )
{ 
		wchar_t c = fgetwc(stream); 
		ungetwc(c, stream); 
		return c; 
}

/**
 *	@brief View a character at position without moving pointer; Peeks a seek.
 *	@return seeked and peeked character.
 */
char fspeek( FILE *stream, long int offset, int position )
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
int frpeek( FILE *stream, char c )
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
int frdpeek( FILE *stream, char d )
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
int fcounts( FILE *stream )
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
 * @brief Copies data from `src` file to `dest` file.
 */
int fcopy( FILE *dest, FILE *src )
{
	int c;
	if(ftell(src) != 0) fseek(src, 0, SEEK_SET); //reset pos
	while((c = fgetc(src)) != EOF) {
		if(fputc(c, dest) == EOF) break;
	}
	return 1;
}


/**
 * @brief Return file extension.
 */
char *fext( const char *filename )
{
		char *dot = strchr(filename, '.');
		if(!dot || dot == filename) return "";
		return dot + 1;
}

/**
 * @brief Return filename.
 */
char *ffilename( const char *path )
{
	char* path_copy = strdup(path);
    const char delim[] = "/";
    char* file;
	char* last;

    file = strtok( path_copy, delim );

    while ( file != NULL )
    {
		last = strdup( file );
        file = strtok( NULL, delim );
    }

    free(path_copy);

	return last;
}



/**
 * @brief Move data from `oldpath` file to `newpath.`
 */
bool fmove( char *oldpath, char *newpath )
{
	filesystem_t* fs = filesystem( oldpath );

	char new_name[256] = {'\0'};
	int copy_num = 1;


	printf("%s\n", fs->filename);



	strcpy(new_name, newpath);
	strncat(new_name, fs->filename, strlen(fs->filename));



	if ( fexists(new_name) ) {

		while ( fexists(new_name) )
		{
			strcpy( new_name, newpath );
			strncat( new_name, fs->stem, strlen(fs->stem) );

			sprintf(
					new_name + strlen(new_name),
					"_%d",
					copy_num++
					);

			strncat( new_name, ".", 2 );
			strncat( new_name, fs->extension, strlen(fs->extension) );
		}

	}

	if ( rename(oldpath, new_name) == 0 ) {
		return true;
	} else {
		perror("fmove() error on attempts to rename");
		return false;
	}
}

/**
 * @brief Move data from `oldpath` file to `newpath.`
 */
bool fexists(const char *file)
{
  if(access(file, F_OK) == 0) {
    return true;
  } else {
    return false;
  }
}

/* STDIN
========*/

// get string, often a buffer, pass length as well
// A safe way to read input that ensures no misc LF or breaks in read string
void sgets(char* str, int n)
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


bool promptYesOrNo(const char *question) 
{
		char response[4];

		while(1) {
				fprintf(stdout, "%s (Y/n): ", question);
				if(fgets(response, sizeof(response), stdin) == NULL)
						continue;
				
				response[strcspn(response, "\n")] = 0;

				if(toupper(response[0]) == 'Y' && response[1] == '\0') {
						return true;
				} else if(toupper(response[0]) == 'N' && response[1] == '\0') {
								return false;
				} else {
						fprintf(stderr, "Invalid input.\n");
				}
		}
}

/* STDOUT
==========*/


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


/* filesystem_t functions
===========*/

filesystem_t* filesystem(char* path)
{
	filesystem_t *fs = (filesystem_t*)malloc( sizeof(filesystem_t) );


	//fs->path = (char*)malloc( sizeof(char) * (strlen(path) + 1) );
	fs->path = strdup( path );

	fs->filename = ffilename( path );

	fs->extension = fext( fs->filename );

	char* stem = strdup( fs->filename );
	fs->stem = strtok( stem, "." );

    return fs;
}

void filesystem_kill(filesystem_t* fs)
{
	free( fs );
}
