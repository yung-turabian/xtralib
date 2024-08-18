/**
 * @file trie.c
 * @author yung-turabian
 * @date 4 6 2024
 * @brief Trie (N-ary tree) structure and functions.
 *
 * @copyright BSD-3-Clause
 */

#include <xtra/trie.h>

TrieNode *
trienode_init(wchar_t data)
{
    TrieNode *node = (TrieNode*)calloc(1, sizeof(TrieNode));
    for(int i = 0; i < TRIE_LETTERS_N; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    node->data = data;

    return node;
}

void
trienode_free(TrieNode *node)
{
    for(int i = 0; i < TRIE_LETTERS_N; i++)
    {
        if(node->children[i] != NULL)
            trienode_free(node->children[i]);
        else
            continue;
    }
    free(node);
}

/**
 *  @brief Inserts word onto the trie
 *  @warning Assumes word only has lower case characters.
 */
TrieNode *
trie_insert(TrieNode *root, wchar_t *word)
{
    TrieNode *temp = root;

    for(int i = 0; word[i] != L'\0'; i++)
    {
        int idx = (int) word[i] - L'a';
        if(temp->children[idx] == NULL)
        {
            // if doesn't exist, create child
            temp->children[idx] = trienode_init(word[i]);
        }
        else
        {
            // Node already exists.
        }
        // Go down a level, to child refrences by idx
        temp = temp->children[idx];
    }
    // At end of word, mark node as a leaf node
    temp->is_leaf = 1;
    return root;
}

/**
 *  @brief Deletes words from trie.
 *  Will try to delete the word sequence from trie only if
 *  it ends up in a leaf node.
 */
TrieNode *
trie_delete(TrieNode *root, wchar_t *word)
{
    if(!root)
        return NULL;
    if(!word || word[0] == L'\0')
        return root;

    // TODO

    // If node corresponding to the mathch is not a leaf node, stop
    //if(!is_leaf_node(root, word))
    //    return root;

}


int
trie_search(TrieNode *root, wchar_t *word)
{
    TrieNode *temp = root;

    for(int i = 0; word[i] != L'\0'; i++)
    {
        int position = word[i] - 'a';
        if(temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if(temp != NULL && temp->is_leaf == 1)
        return 1;
    return 0;
}

void
trie_print(TrieNode *root)
{
    if(!root)
        return;
    TrieNode *temp = root;
    printf("%c -> ", temp->data);
    for(int i = 0; i < TRIE_LETTERS_N; i++)
        trie_print(temp->children[i]);
}

void
trie_print_search(TrieNode *root, wchar_t *word)
{
    printf("Searching for %ls: ", word);
    if(trie_search(root, word) == 0)
        printf("Not found\n");
    else
        printf("Found!\n");
}
