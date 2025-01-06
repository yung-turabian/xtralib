/**
 * @file trie.h
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Trie (N-ary tree) structure and functions.
 *
 * @copyright BSD-3-Clause
 */

#ifndef __REISLIB_TRIE_H__
#define __REISLIB_TRIE_H__

// Default to English
static int xtTrieNumOfLetters = 26;

#include <reis/common.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

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

#endif
