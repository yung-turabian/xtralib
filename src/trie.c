/**
 * @file trie.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Trie (N-ary tree) structure and functions.
 *
 * @copyright BSD-3-Clause
 */

#include <xtra/trie.h>
#include <xtra/memory.h>

TrieNode * TrieNodeInit(wchar_t data, int numOfLetters)
{
    TrieNode *node = (TrieNode*)CALLOC(1, sizeof(TrieNode) + sizeof(TrieNode*) * xtTrieNumOfLetters);
		// Skipping error checking, Unsafe
		xtTrieNumOfLetters = numOfLetters;
    for(int i = 0; i < xtTrieNumOfLetters; i++)
        node->children[i] = NULL;

    node->is_leaf = false;
    node->data = data;

    return node;
}

void TrieNodeFree(TrieNode *node)
{
    for(int i = 0; i < xtTrieNumOfLetters; i++)
    {
        if(node->children[i] != NULL)
            TrieNodeFree(node->children[i]);
        else
            continue;
    }
    FREE(node);
}

/**
 *  @brief Inserts word onto the trie
 *  @warning Assumes word only has lower case characters.
 */
TrieNode * TrieInsert(TrieNode *root, wchar_t *word)
{
    TrieNode *temp = root;

    for(int i = 0; word[i] != L'\0'; i++)
    {
        int idx = (int) word[i] % xtTrieNumOfLetters;
        if(temp->children[idx] == NULL)
        {
            // if doesn't exist, create child
            temp->children[idx] = TrieNodeInit(word[i], xtTrieNumOfLetters);
        }
        else
        {
            // Node already exists.
        }
        // Go down a level, to child refrences by idx
        temp = temp->children[idx];
    }
    // At end of word, mark node as a leaf node
    temp->is_leaf = true;
    return root;
}

/**
 *  @brief Deletes words from trie.
 *  Will try to delete the word sequence from trie only if
 *  it ends up in a leaf node.
 */
TrieNode * TrieDelete(TrieNode *root, wchar_t *word)
{
    if(!root)
        return NULL;
    if(!word || word[0] == L'\0')
        return root;

    // TODO

    // If node corresponding to the mathch is not a leaf node, stop
    //if(!is_leaf_node(root, word))
    //    return root;
		printf("NOT IMPLEMENTED\n");
		return NULL;
}

/**
 *	@brief Searches for a given word.
 */
bool TrieSearch(TrieNode *root, wchar_t *word)
{
    TrieNode *temp = root;

    for(int i = 0; word[i] != L'\0'; i++)
    {
        int position = word[i] % xtTrieNumOfLetters;
        if(temp->children[position] == NULL)
            return false;
        temp = temp->children[position];
    }
    if(temp != NULL && temp->is_leaf)
        return true;
    return false;
}

void TriePrint(TrieNode *root)
{
    if(!root)
        return;
    TrieNode *temp = root;
    printf("%c -> ", temp->data);
    for(int i = 0; i < xtTrieNumOfLetters; i++)
        TriePrint(temp->children[i]);
}

bool TriePrintSearch(TrieNode *root, wchar_t *word)
{
    printf("Searching for %ls: ", word);
		bool ret = true;
    if(TrieSearch(root, word) == 0) {
        printf("Not found\n");
				ret = false;
		}
    else
        printf("Found!\n");

		return ret;
}
