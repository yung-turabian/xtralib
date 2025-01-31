/**
 * @file data.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Includes several data strucutes such as maps, vectors, and matricies.
 *
 * @copyright BSD-3-Clause
 */

#include "../include/reis.h"

// VEC2
/**
 * @brief Create a new Vec2 datatype instance.
 *
 * @param x
 * @param y
 * @return Vec2*
 */
Vec2 *reisNewVec2(int x, int y) {
  Vec2 *v = MALLOC(sizeof(Vec2));
  v->x = x;
  v->y = y;
  return v;
}

/**
 * @brief Destroy a Vec2 instance.
 * @param v The vector.
 */
void reisDelVec2(Vec2 *v) { FREE(v); }

// PAIR
Pair *reisPairCreate(wchar_t *first, wchar_t *second) {
  Pair *p = (Pair *)MALLOC(sizeof(Pair));
  if (p == NULL)
    return NULL;

  p->first = MALLOC(sizeof(wchar_t) * (wcslen(first) + 1));
  p->second = MALLOC(sizeof(wchar_t) * (wcslen(second) + 1));
  if (p->first == NULL || p->second == NULL)
    return NULL;

  wcscpy(p->first, first);
  wcscpy(p->second, second);
  return p;
}

void reisPairFree(Pair *p) {
  FREE(p->first);
  FREE(p->second);
  FREE(p);
}

// MAP
Map *reisMapNew() {
  Map *m = (Map *)MALLOC(sizeof(Map));

  m->size = 0;

  return m;
}

static int _MapGetIndex(Map *m, char key[]) {
  for (size_t it = 0; it < m->size; it++)
    if (strcmp(m->keys[it], key) == 0)
      return it;
  return -1;
}

void reisMapInsert(Map *m, char key[], int value) {
  int index = _MapGetIndex(m, key);
  if (index != -1)
    m->values[index] = value;
  else {
    strcpy(m->keys[m->size], key);
    m->values[m->size] = value;
    m->size++;
  }
}

int reisMapGet(Map *m, char key[]) {
  int index = _MapGetIndex(m, key);
  if (index != -1)
    return m->values[index];
  else
    return -1;
}

void reisMapPrint(Map *m) {
  for (size_t it = 0; it < m->size; it++)
    fprintf(stdout, "%s: %d\n", m->keys[it], m->values[it]);
}

// TODO: Not finished
void reisMapDel(Map *m) { FREE(m); }

MultiMap* reisNewMultiMap() {
  MultiMap *m = (MultiMap *)MALLOC(sizeof(MultiMap));
  m->keys = (wchar_t **)CALLOC(MULTIMAP_DEFAULT_SIZE, sizeof(wchar_t *));
  m->values =
      (MultiMapNode **)CALLOC(MULTIMAP_DEFAULT_SIZE, sizeof(MultiMapNode *));
  m->size = 0;
  m->capacity = MULTIMAP_DEFAULT_SIZE;
  return m;
}

static int _MultiMapGetIndex(MultiMap *m, wchar_t *key) {
  for (size_t it = 0; it < m->size; it++)
    if (wcscmp(m->keys[it], key) == 0)
      return it;
  return -1;
}

static void _MultiMapResize(MultiMap *m) {
  size_t newCapacity = m->capacity * MULTIMAP_RESIZE_FACTOR;
  wchar_t **newKeys = (wchar_t **)CALLOC(newCapacity, sizeof(wchar_t *));
  MultiMapNode **newValues =
      (MultiMapNode **)CALLOC(newCapacity, sizeof(MultiMapNode *));

  if (!newKeys || !newValues) {
    fprintf(stderr, "Failed to allocate memory for resizing.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < m->size; i++) {
    newKeys[i] = m->keys[i];
    newValues[i] = (MultiMapNode *)MALLOC(sizeof(MultiMapNode));
    MultiMapNode *newNode = newValues[i];
    MultiMapNode *oldNode = m->values[i];
    while (oldNode != NULL) {
      newNode->value = oldNode->value;
      if (oldNode->next != NULL) {
        newNode->next = (MultiMapNode *)MALLOC(sizeof(MultiMapNode));
      } else {
        newNode->next = NULL;
      }
      newNode = newNode->next;
      oldNode = oldNode->next;
    }
  }

  // Free old memory
  FREE(m->keys);
  FREE(m->values);

  m->keys = newKeys;
  m->values = newValues;
  m->capacity = newCapacity;
}

void reisMultiMapInsert(MultiMap *m, wchar_t *key, void *value, size_t size) {
  if (m->size >= m->capacity)
    _MultiMapResize(m);

  int index = _MultiMapGetIndex(m, key);
  if (index != -1) {
    MultiMapNode *newNode = m->values[index];
    while (newNode->next != NULL) {
      newNode = newNode->next;
    }

    newNode->next = (MultiMapNode *)MALLOC(sizeof(MultiMapNode));
    newNode->next->value = MALLOC(size);
    memcpy(newNode->next->value, value, size);
    newNode->next->next = NULL;
  } else {

    m->keys[m->size] = (wchar_t *)MALLOC(sizeof(wchar_t) * (wcslen(key) + 1));
    wcscpy(m->keys[m->size], key);
    m->values[m->size] = (MultiMapNode *)MALLOC(sizeof(MultiMapNode));
    m->values[m->size]->value = MALLOC(size);
    memcpy(m->values[m->size]->value, value, size);
    m->values[m->size]->next = NULL;
    m->size++;
  }
}

void** reisMultiMapGet(MultiMap *m, wchar_t *key, size_t *numValues) {
  int index = _MultiMapGetIndex(m, key);
  if (index == -1) {
    *numValues = 0;
    return NULL;
  }

  MultiMapNode *node = m->values[index];
  size_t count = 0;
  while (node) {
    count++;
    node = node->next;
  }

  void **valuesArray = (void **)CALLOC(count, sizeof(void *));
  if (!valuesArray) {
    *numValues = 0;
    return NULL;
  }

  node = m->values[index];
  size_t i = 0;
  while (node) {
    valuesArray[i++] = node->value;
    node = node->next;
  }

  *numValues = count;
  return valuesArray;
}

void reisMultiMapPrint(MultiMap *m) {
  for (size_t it = 0; it < m->size; it++) {
    MultiMapNode *node = m->values[it];
    fprintf(stdout, "%ls: ", m->keys[it]);
    while (node) {
      fprintf(stdout, "%ls ",
              (wchar_t *)node->value); // Assuming values are pointers
      node = node->next;
    }
    fprintf(stdout, "\n");
  }
}

void reisDelMultiMap(MultiMap *m) {
  for (size_t it = 0; it < m->size; it++) {
    MultiMapNode *node = m->values[it];

    while (node != NULL) {
      MultiMapNode *next = node->next;

      FREE(node->value);

      FREE(node);
      node = next;
    }
    FREE(m->keys[it]);
  }
  FREE(m->keys);
  FREE(m->values);
  FREE(m);
}

// DYN-ARRAY
wcs_dynarr_t *wcs_dynarr_init() {
  wcs_dynarr_t *a = NULL;
  a = (wcs_dynarr_t *)MALLOC(sizeof(wcs_dynarr_t));

  a->items = (wchar_t **)MALLOC(sizeof(wchar_t *));
  a->size = 0;
  a->allocated_size = 1;

  return a;
}

void wcs_dynarr_kill(wcs_dynarr_t *a) {
  for (size_t i = 0; i < a->size; i++) {
    FREE(a->items[i]);
    a->items[i] = NULL;
  }

  FREE(a->items);
  FREE(a);
  a = NULL;
}

void wcs_dynarr_push(wcs_dynarr_t *a, wchar_t *item) {
  if (a->size >= a->allocated_size) {
    a->allocated_size *= 2;
    a->items =
        (wchar_t **)REALLOC(a->items, sizeof(wchar_t *) * a->allocated_size);
  }

  a->items[a->size] =
      (wchar_t *)MALLOC(MAX_DATASTRUCT_STRSIZ * sizeof(wchar_t));
  wcscpy(a->items[a->size], item);

  a->size++;
}
