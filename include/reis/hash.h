#ifndef __REISLIB_HASH_H__
#define __REISLIB_HASH_H__

#include <reis/common.h>
#include <string.h>
#include <wchar.h>

#define REISLIB_HASHTABLE_INITIAL_CAPACITY 16
#define REISLIB_HASH_FNV_OFFSET 14695981039346656037UL
#define REISLIB_HASH_FNV_PRIME 1099511628211UL

typedef struct {
		enum XStringType {
				E_STRING,
				E_WCSTRING
		} type;

		union XStringValue {
				const char *str;
				const wchar_t *wcs;
		} value;
} XString;


typedef struct {
		XString *key;
		void *value;
} HashTableEntry;

typedef struct {
		HashTableEntry *entries;
		size_t capacity;
		size_t length;
} HashTable;


// long names
HashTable * HashTable_Init();

void HashTable_Terminate(HashTable *table);

// return NULL if key not found
void * HashTableGet_str(HashTable *table, const char *key);
void * HashTableGet_wcs(HashTable *table, const wchar_t *key);
#define HashTable_Get(table,key) _Generic((key), \
				char *: HashTableGet_str, \
				const char *: HashTableGet_str, \
				wchar_t *: HashTableGet_wcs \
)(table, key)

// If not already present in table, key is copied to newly allocated memory
// Return NULL if out of memory
const char * HashTableSet_str(HashTable *table, const char *key, void *value);
const wchar_t * HashTableSet_wcs(HashTable *table, const wchar_t *key, void *value);
#define HashTable_Set(table,key,value) _Generic((key), \
				char *: HashTableSet_str, \
				const char *: HashTableSet_str, \
				wchar_t *: HashTableSet_wcs \
)(table, key, value)

size_t HashTable_Length(HashTable *table);

/*TODO Adding the iterators.*/
typedef struct {
		const char *key;
		void *value;

		HashTable *table;
		size_t it;
} HashTableIterator;

HashTableIterator HashTable_IteratorInit(HashTable *table);

bool HashTable_Next(HashTableIterator it);

//short names
#ifdef REISLIB_HASHTABLES_SHORT_NAMES

#define HT_Init() HashTable_Init()
#define HT_Term(...) HashTable_Terminate(__VA_ARGS__)
#define HT_Get(...) HashTable_Get(__VA_ARGS__)
#define HT_Set(...) HashTable_Set(__VA_ARGS__)
#define HT_Length(...) HashTable_Length(__VA_ARGS__)

#endif

#endif
