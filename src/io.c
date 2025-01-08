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
const char *ExtractFileExtension( const char *filename )
{
	const char *ext;
	int it, n;

	if ( filename == NULL ) return NULL;

	n = strlen( filename );
	it = n - 1;

	while ( (it > 0) && (filename[it] != '.') && (filename[it] != '/') && (filename[it] != '\\') ) {
        it--;
    }

	if ( (it > 0) && (it < n - 1) && (filename[it] == '.') && (filename[it - 1] != '/') && (filename[it - 1] != '\\') ) {
        ext = filename + it;
    } else {
        ext = filename + n;
    }


	return ext;
}

/**
 * @brief Return filename.
 */
const char *ExtractFileName( const char *path )
{
	const char *filename;
	int it, n;

	if ( path == NULL ) return NULL;

	n = strlen( path );
	it = n - 1;

	while ( (it > 0) && (path[it] != '/') && (path[it] != '\\') ) {
        it--;
    }


	if ( (it > 0) && (it < n - 1) && ((path[it] == '/') || (path[it] == '\\')) ) {
        filename = path + it + 1;
    } else {
        filename = path + n;
    }


	return filename;
}



/**
 * @brief Move data from `oldpath` file to `newpath.`
 */
bool fmove( char *oldpath, char *newpath )
{
	filesystem_t* fs = FS_Create( oldpath );

	char new_name[256] = {'\0'};
	int copy_num = 1;

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

			strncat( new_name, fs->extension, strlen(fs->extension) );
		}

	}


	if ( rename(oldpath, new_name) == 0 ) {
		FS_Destroy( fs );
		return true;
	} else {
		perror("fmove() error on attempts to rename");
		FS_Destroy( fs );
		return false;
	}
}

/**
 * @brief Checks if file exists.
 */
bool fexists( const char *file )
{
  if(access(file, F_OK) == 0) {
    return true;
  } else {
    return false;
  }
}

/**
 *
 */
bool dexists( const char *path )
{
	struct stat sb;
	if ( stat(path, &sb) == 0 && S_ISDIR(sb.st_mode) ) {
		return true;
	} else {
		if ( errno == ENOENT ) {
			return false;
		} else {
			fprintf( stderr, "Error checking directory: %s\n", path );
			return false;
		}
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


bool PromptYesOrNo(const char *question)
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

filesystem_t* FS_Create( char* path )
{
	filesystem_t *fs = (filesystem_t*)MALLOC( sizeof(filesystem_t) );

	fs->path = strdupl( path );

	fs->filename = strdupl( ExtractFileName( fs->path ) );

	fs->extension = strdupl( ExtractFileExtension( fs->filename ) );

	char* tmp_filename = strdupl( fs->filename );
	char* stem = strtok( tmp_filename, "." );

	fs->stem = strdupl( stem );

	FREE( tmp_filename );

    return fs;
}

void FS_Destroy( filesystem_t* fs )
{
	FREE( fs->path );
	FREE( fs->filename );
	FREE( fs->extension );
	FREE( fs->stem );
	FREE( fs );
}
