#ifndef __REISLIB_HASH_H__
#define __REISLIB_HASH_H__

#include <reis/common.h>
#include <string.h>
#include <wchar.h>

#define REISLIB_HASHTABLE_INITIAL_CAPACITY 16
#define REISLIB_HASH_FNV_OFFSET 14695981039346656037UL
#define REISLIB_HASH_FNV_PRIME 1099511628211UL

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
hashtable_t * HashTable_Init();

void HashTable_Terminate(hashtable_t *table);

// return NULL if key not found
void * HashTableGet_str(hashtable_t *table, const char *key);
void * HashTableGet_wcs(hashtable_t *table, const wchar_t *key);
#define HashTable_Get(table,key) _Generic((key), \
				char *: HashTableGet_str, \
				const char *: HashTableGet_str, \
				wchar_t *: HashTableGet_wcs \
)(table, key)

// If not already present in table, key is copied to newly allocated memory
// Return NULL if out of memory
const char * HashTableSet_str(hashtable_t *table, const char *key, void *value);
const wchar_t * HashTableSet_wcs(hashtable_t *table, const wchar_t *key, void *value);
#define HashTable_Set(table,key,value) _Generic((key), \
				char *: HashTableSet_str, \
				const char *: HashTableSet_str, \
				wchar_t *: HashTableSet_wcs \
)(table, key, value)

size_t HashTable_Length(hashtable_t *table);

/*TODO Add the iterators.*/
typedef struct {
		const char *key;
		void *value;

		hashtable_t *table;
		size_t it;
} hashtable_iterator_t;

hashtable_iterator_t HashTable_IteratorInit(hashtable_t *table);

bool HashTable_Next(hashtable_iterator_t it);

//short names
#ifdef REISHASH_ABBR
#define HT_Init() HashTable_Init()
#define HT_Term(...) HashTable_Terminate(__VA_ARGS__)
#define HT_Get(...) HashTable_Get(__VA_ARGS__)
#define HT_Set(...) HashTable_Set(__VA_ARGS__)
#define HT_Length(...) HashTable_Length(__VA_ARGS__)
#define HT_Terminate(...) HashTable_Terminate(__VA_ARGS__)
#endif

#endif
