/**
 * @file trie.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Trie (N-ary tree) structure and functions.
 *
 * @copyright BSD-3-Clause
 */

#ifndef XTRA_TRIE_H
#define XTRA_TRIE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define TRIE_LETTERS_N 26

typedef struct TrieNode TrieNode;
struct TrieNode {
    wchar_t data;
    TrieNode* children[TRIE_LETTERS_N];
    int is_leaf;
};

TrieNode * trienode_init(wchar_t data);
void trienode_free(TrieNode *node);

TrieNode * trie_insert(TrieNode *root, wchar_t *word);
TrieNode * trie_delete(TrieNode *root, wchar_t *word);

int trie_search(TrieNode *root, wchar_t *word);

void trie_print(TrieNode *root);
void trie_print_search(TrieNode *root, wchar_t *word);

#endif
