/**
 * @file reis.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief Shared typedefs and functions.
 */
#ifndef __REISLIB_COMMMON_H__
#define __REISLIB_COMMMON_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <syslog.h>
#include <time.h>
#include <fcntl.h>
#include <stdatomic.h>

#if defined(__APPLE__)
	#include <wchar.h>
#elif defined(__linux__)
	#include <wchar.h>
	#include <wctype.h>
	#include <ctype.h>
#endif

/* GENERAL SETTINGS */
#define REISLIB_INT_MAX 2147483647
#define CHARSET "abcdefghijklmnopqrstuvwxyz"
#define CHARSET_LEN 26
#define MAX_CHARSET_SIZE 256
#define SUB 0x1A // an empty char as not to interfere with '\0'


/* DATA STRUCTURES */
#define MAX_DATASTRUCT_STRSIZ  100
#define MAP_MAX 100

#define MULTIMAP_DEFAULT_SIZE 16
#define MULTIMAP_RESIZE_FACTOR 2

#define REISLIB_HASHTABLE_INITIAL_CAPACITY 16
#define REISLIB_HASH_FNV_OFFSET 14695981039346656037UL
#define REISLIB_HASH_FNV_PRIME 1099511628211UL

/* UNIX */
#define BD_NO_CHDIR           01 // dont chdir("/")
#define BD_NO_CLOSE_FILES     02 // dont close all open files
#define BD_NO_REOPEN_STD_FDS  04 // dont reopen stdin/out/err

#define BD_NO_UMASK0          010 // dont do umask(0)
#define BD_MAX_CLOSE          8192 // max fds

/* IO */
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


typedef uint8_t  	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;
typedef uint64_t 	u64;
typedef int8_t   	s8;
typedef int16_t 	s16;
typedef int32_t  	s32;
typedef int64_t  	s64;
typedef float    	f32;
typedef double      f64;
typedef long double f128;

#ifdef __cplusplus
extern "C" {
#endif
// Ensure no conflicts if bool is already defined
#if !defined(__cplusplus)
		#if !defined(bool) 
				typedef enum __attribute__((__packed__)) {false, true} bool;
		#endif
#endif

#define fnname(name) #name


/* PAIR */
#if !defined(Pair) 
typedef struct {
		wchar_t *first;
		wchar_t *second;
} Pair;

Pair * reisPairCreate( wchar_t *first, wchar_t *second );
void reisPairFree( Pair *p );
#endif

/* VECTOR */
#if !defined(Pair) 
typedef struct Vec2 Vec2;
struct Vec2 {
	int x;
	int y;
};

Vec2* reisNewVec2( int x, int y );
void  reisDelVec2( Vec2* v );
#endif

/* MAP */
#if !defined(Map) 
typedef struct Map Map;
struct Map {
		size_t   size;
		char  	 keys[MAP_MAX][MAX_DATASTRUCT_STRSIZ];
		int      values[MAP_MAX];
};

Map * reisMapNew();
void reisMapInsert(Map *m, char key[], int value);
int reisMapGet(Map *m, char key[]);
void reisMapPrint(Map *m);
// TODO: Not finished
void reisMapDel(Map *m);
#endif


typedef struct wcs_dynarr_t wcs_dynarr_t;
struct wcs_dynarr_t {
		wchar_t **items;
		size_t size;
		size_t allocated_size;
};

/* MULTIMAP
=========== */
struct MultiMapNode {
		void *value;
		struct MultiMapNode *next;
};
typedef struct MultiMapNode MultiMapNode;

typedef struct {
		wchar_t  	 **keys;
		MultiMapNode **values;
		size_t   size;
		size_t capacity;
} MultiMap;

MultiMap * reisNewMultiMap();
void reisMultiMapInsert(MultiMap *m, wchar_t *key, void *value, size_t size);
void** reisMultiMapGet(MultiMap *m, wchar_t *key, size_t *numValues);
void reisMultiMapPrint(MultiMap *m);
void reisDelMultiMap(MultiMap *m);

wcs_dynarr_t * wcs_dynarr_init();
void wcs_dynarr_kill(wcs_dynarr_t *arr);
void wcs_dynarr_push(wcs_dynarr_t *arr, wchar_t *item);


/* TRIE
 ======= */

// Default to English but designed to be changed.
static int xtTrieNumOfLetters = 26;

typedef struct TrieNode TrieNode;
struct TrieNode {
    wchar_t data;
    bool is_leaf;
    TrieNode* children[];
};

TrieNode * TrieNodeInit(wchar_t data, int numOfLetters);
void TrieNodeFree(TrieNode *node);

TrieNode * TrieInsert(TrieNode *root, wchar_t *word);
TrieNode * TrieDelete(TrieNode *root, wchar_t *word);

bool TrieSearch(TrieNode *root, wchar_t *word);

void TriePrint(TrieNode *root);
bool TriePrintSearch(TrieNode *root, wchar_t *word);

/* HASHTABLE (i.e. hashmap)
  ========================== */
// TODO Introduce this schema elsewhere.
typedef struct {
    enum string_type {
	E_STRING,
	E_WCSTRING
    } type;

    union string_content {
        const char *str;
	const wchar_t *wcs;
    } value;

    u32 length;
} string_t;


typedef struct {
    string_t *key;
    void *value;
} hashtable_entry_t;

typedef struct {
    hashtable_entry_t *entries;
    size_t capacity;
    size_t length;
} hashtable_t;


// long names
hashtable_t * reisNewHashTable();
void reisDelHashTable(hashtable_t *table);

// return NULL if key not found
void * HashTableGet_str(hashtable_t *table, const char *key);
void * HashTableGet_wcs(hashtable_t *table, const wchar_t *key);
#define reisHashTableGet(table,key) _Generic((key), \
				char *: HashTableGet_str, \
				const char *: HashTableGet_str, \
				wchar_t *: HashTableGet_wcs \
)(table, key)

// If not already present in table, key is copied to newly allocated memory
// Return NULL if out of memory
const char * HashTableSet_str(hashtable_t *table, const char *key, void *value);
const wchar_t * HashTableSet_wcs(hashtable_t *table, const wchar_t *key, void *value);
#define reisHashTableSet(table,key,value) _Generic((key), \
				char *: HashTableSet_str, \
				const char *: HashTableSet_str, \
				wchar_t *: HashTableSet_wcs \
)(table, key, value)

size_t reisHashTableLength(hashtable_t *table);

/*TODO Add the iterators.*/
typedef struct {
		const char *key;
		void *value;

		hashtable_t *table;
		size_t it;
} hashtable_iterator_t;

hashtable_iterator_t reisNewHashTableIterator(hashtable_t *table);

bool reisHashTableIteratorNext(hashtable_iterator_t it);

//short names
#ifdef REISHASH_ABBR
#define HT_New() reisNewHashTable()
#define HT_Del(...) reisDelHashTable(__VA_ARGS__)
#define HT_Get(...) reisHashTableGet(__VA_ARGS__)
#define HT_Set(...) reisHashTableSet(__VA_ARGS__)
#define HT_Length(...) reisHashTableLength(__VA_ARGS__)
#endif


/* UNIX CALLS */

// 0 on success; -1 on error
int reisDaemonize(const char *name, char* path, int flags);

/* RANDOM NUMBER GENERATION */

/**
 * @brief The random seed generator for most functions.
 * A pseudo-random generator, older solution, a Linear Congruential Generator
 * @warning haven't tested on windows. And it needs to be seeded than gen.
 */
static inline size_t rng(size_t lb, size_t ub) {
#if defined(__linux__) || defined(__APPLE__)
	struct timespec spec;
	timespec_get(&spec, TIME_UTC);
	long long ns;
	ns = spec.tv_nsec; //fractional
	int seed = lb + ((22695477 * ns) % 4294967296) % (ub - lb + 1);
	return seed;
#else
	return lb + ub * -1;
#endif
}

static inline size_t unix_rng(size_t lb, size_t ub) {
	int fd = open("/dev/urandom", O_RDONLY);
	if(fd < 0) {
			perror("open");
			return -1;
	}

	unsigned int random;
	ssize_t result = read(fd, &random, sizeof(random));
	close(fd);

	if(result != sizeof(random)) {
			perror("read");
			return -1;
	}
	return lb + (random % (ub - lb + 1));
}

/**
 * @brief Uses the defined INT_MAX instead of passing bounds
 */
inline int randi() {
	return (int)rng(0, REISLIB_INT_MAX-1);
}

inline int randib(int lb, int ub) {
	return (int)rng(lb, ub);
}

// generate random char
inline char randc() {
	return CHARSET[rng(0, CHARSET_LEN - 1)];
}

// generate random string of desired length, MUST FREE STR!
inline char* rands(size_t len) {
	char *str = (char*)malloc(len + 1);

	size_t i;
	for(i=0;i<len;i++) {
		str[i] = randc();
	}
	str[len+1] = '\0';

	return str;
}


/* MATH */
int isNum(const char *str);

/* IO */
typedef struct {
    char* path;
    char* filename;
    char* extension;
    char* stem;
} filesystem_t;

filesystem_t* FS_Create( char* path );
void          FS_Destroy(filesystem_t* fs);

/* FILE HANDLING
=================*/
char    	   fpeek( FILE *stream );
wchar_t 	   fpeek_wc( FILE *stream );
char    	   fspeek( FILE *stream, long int offset, int position );
int     	   frpeek( FILE *stream, char c );
int     	   frdpeek( FILE *stream, char d );
int     	   fcounts( FILE *stream );
void     	   fcopy( FILE *dest, FILE *src );
bool    	   fexists( const char *file );
bool    	   fmove( char *oldpath, char *newpath );
const char*  ExtractFileName( const char *path );
const char*  ExtractFileExtension( const char *filename );
bool    	   dexists( const char *path );

/* STDIN
========*/
void   			 sgets(char* str, int n);
bool   			 PromptYesOrNo(const char *question);


/* STDOUT
==========*/
CHECK_PRINTF_FMT(1, 2) void eprintf(const char *fmt, ...);
CHECK_PRINTF_FMT(1, 3) void cprintf(const char * color, const char * fmt, ...);


/* STRING MANIPULATION */
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
wchar_t * wcsrev(wchar_t *str);
char * strpre(char *str, char chr);
wchar_t * wcspre(wchar_t *str, wchar_t chr);
char * strpres(char *str, char* pre);
wchar_t * wcspres(wchar_t *str, wchar_t *pre);
char *strisdigit(char *str);
wchar_t *wcsisdigit(wchar_t *str);
long hash(const char* str);


/* MEMORY
  ======= */
#ifndef MALLOC
#define MALLOC(size) reisMalloc(size)
#endif

#ifndef CALLOC
#define CALLOC(nitems,size) reisCalloc(nitems, size)
#endif

#ifndef REALLOC
#define REALLOC(ptr,size) reisRealloc(ptr, size)
#endif

#ifndef MGET
#define MGET(ptr) reisMemGet(ptr)
#endif

#ifndef FREE
#define FREE(ptr) reisFree(ptr)
#endif

void * reisMalloc(size_t size);
void * reisCalloc(size_t nitems, size_t size);
void * reisRealloc(void *ptr, size_t size);
size_t reisMemGet(void *ptr);
size_t reisFree(void *ptr);
size_t reisMemGetAll();

/* SORTS */
void reisInsertionSort( int arr[], int len );
void reisMergeSort( int arr[], int len );

#ifdef __cplusplus
}
#endif

#endif 
