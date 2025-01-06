/**
 * @file hash.c
 * @author yung-turabian
 * @date 8 25 2024
 * @brief HashTable data structure and related functions.
 *
 * @copyright BSD-3-Clause
 */

#include <reis/hash.h>
#include <reis/memory.h>

static const char * HashTableSetEntry_str(HashTableEntry *entries, size_t capacity, const char *key, void *value, size_t *plength);
static const wchar_t * HashTableSetEntry_wcs(HashTableEntry *entries, size_t capacity, const wchar_t *key, void *value, size_t *plength);
static bool HashTableExpand(HashTable *table);

static u64 HashKey_str(const char *key);
static u64 HashKey_wcs(const wchar_t *key);

#include <stdio.h>

HashTable * HashTableInit()
{
		HashTable *table = MALLOC(sizeof(HashTable));
		if(table == NULL)
				return NULL;

		table->length = 0;
		table->capacity = REISLIB_HASHTABLE_INITIAL_CAPACITY;


		// alloc (zero'd) space for empty buckets
		table->entries = CALLOC(table->capacity, sizeof(HashTableEntry));
		if(table->entries == NULL) {
				FREE(table);
				return NULL;
		}

		return table;
}

void HashTableTerminate(HashTable *table)
{
		for(size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL)
        {
						switch(table->entries[i].key->type) {
								case E_STRING:
										free(table->entries[i].key->value.str);
										break;
								case E_WCSTRING:
										free(table->entries[i].key->value.wcs);
						}
            FREE(table->entries[i].key);
        }
		}

		FREE(table->entries);
		FREE(table);
}

// Real simple and fast
static u64 HashKey_str(const char *key)
{
		u64 hash = REISLIB_HASH_FNV_OFFSET;
		for(const char *p = key; *p; p++) {
				hash ^= (u64)(unsigned char)(*p);
				hash *= REISLIB_HASH_FNV_PRIME;
		}

		return hash;
}

static u64 HashKey_wcs(const wchar_t *key)
{
		u64 hash = REISLIB_HASH_FNV_OFFSET;
		for(const wchar_t *p = key; *p; p++) {
				hash ^= (u64)(wchar_t)(*p);
				hash *= REISLIB_HASH_FNV_PRIME;
		}

		return hash;
}

void * HashTableGet_str(HashTable *table, const char *key)
{
		u64 hash = HashKey_str(key);
		size_t index = (size_t)(hash & (u64)(table->capacity - 1));

		while(table->entries[index].key != NULL)
		{
				if(strcmp(key, table->entries[index].key->value.str) == 0)
						return table->entries[index].value;

				// linear probe
				index++;
				if(index >= table->capacity)
						index = 0; // Wrap around
		}
		return NULL;
}
void * HashTableGet_wcs(HashTable *table, const wchar_t *key)
{
		u64 hash = HashKey_wcs(key);
		size_t index = (size_t)(hash & (u64)(table->capacity - 1));

		while(table->entries[index].key != NULL)
		{
				if(wcscmp(key, table->entries[index].key->value.wcs) == 0)
						return table->entries[index].value;

				// linear probe
				index++;
				if(index >= table->capacity)
						index = 0; // Wrap around
		}
		return NULL;
}


const char * HashTableSet_str(HashTable *table, const char *key, void *value)
{
		if(value == NULL)
				return NULL;

		if(table->length >= table->capacity / 2) {
				if(!HashTableExpand(table))
						return NULL;
		}

		return HashTableSetEntry_str(table->entries, table->capacity, key, value,
														 &table->length);
}

const wchar_t * HashTableSet_wcs(HashTable *table, const wchar_t *key, void *value)
{
		if(value == NULL)
				return NULL;

		if(table->length >= table->capacity / 2) {
				if(!HashTableExpand(table))
						return NULL;
		}

		return HashTableSetEntry_wcs(table->entries, table->capacity, key, value,
														 &table->length);

}

static const char * HashTableSetEntry_str(HashTableEntry *entries, 
																				  size_t capacity,
																				  const char *key, void *value, 
																					size_t *plength)
{
		u64 hash = HashKey_str(key);
		size_t index = (size_t)(hash & (u64)(capacity - 1));

		// search for empty entry
		while(entries[index].key != NULL)
		{
				if(strcmp(key, entries[index].key->value.str) == 0) {
						// found key (if already exists), updating value...
						entries[index].value = value;
						return entries[index].key->value.str;
				}

				index++;
				if(index >= capacity)
						index = 0;
		}


		// didn't find key, allocate and copy
		if(plength != NULL) {
				key = strdup(key);
				if(key == NULL)
						return NULL;

				(*plength)++;
		}



		entries[index].key = (XString*)MALLOC(sizeof(XString));
		entries[index].key->value.str = (char*)key;
		entries[index].key->type = E_STRING;
		entries[index].value = value;

		return key;
}

static const wchar_t * HashTableSetEntry_wcs(HashTableEntry *entries, 
																						 size_t capacity,
																						 const wchar_t *key, void *value, 
																						 size_t *plength)
{
		u64 hash = HashKey_wcs(key);
		size_t index = (size_t)(hash & (u64)(capacity - 1));

		// search for empty entry
		while(entries[index].key != NULL)
		{
				if(wcscmp(key, entries[index].key->value.wcs) == 0) {
						// found key (if already exists), updaring value...
						entries[index].value = value;
						return entries[index].key->value.wcs;
				}

				index++;
				if(index >= capacity)
						index = 0;
		}

		// didn't find key, allocate and copy
		if(plength != NULL) {
				key = wcsdup(key);
				if(key == NULL)
						return NULL;

				(*plength)++;
		}

		entries[index].key = (XString*)MALLOC(sizeof(XString));
		entries[index].key->value.wcs = (wchar_t*)key;
		entries[index].key->type = E_WCSTRING;
		entries[index].value = value;
		return key;
}



static bool HashTableExpand(HashTable *table)
{
		size_t newCapacity = table->capacity * 2;
		if(newCapacity < table->capacity)
				return false;

		HashTableEntry *newEntries = CALLOC(newCapacity, sizeof(HashTableEntry));
		if(newEntries == NULL)
				return false;

		// move all non-empty entries to new table
		for(size_t i = 0; i < table->capacity; i++) {
				HashTableEntry entry = table->entries[i];
				if(entry.key != NULL) {
						if(entry.key->type == E_WCSTRING)
								HashTableSetEntry_wcs(newEntries, newCapacity, 
																      entry.key->value.wcs, entry.value, NULL);
						else if(entry.key->type == E_STRING)
								HashTableSetEntry_str(newEntries, newCapacity, 
														          entry.key->value.str, entry.value, NULL);
				}
		}

		FREE(table->entries);
		table->entries = newEntries;
		table->capacity = newCapacity;
		return true;
}

size_t HashTableLength(HashTable *table) { return table->length; }



HashTableIterator HashTableIteratorInit(HashTable *table);

bool HashTableNext(HashTableIterator it);
