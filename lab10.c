#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[26]; // Assuming only lowercase alphabets
    int isEndOfWord;               // Flag to mark end of word
};

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Function to initialize a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) // Check if memory allocation is successful
    {
        pTrie->root = (struct TrieNode *)malloc(sizeof(struct TrieNode));
        if (pTrie->root) // Check if memory allocation for root node is successful
        {
            // Initialize root node
            for (int i = 0; i < 26; i++)
                pTrie->root->children[i] = NULL;
            pTrie->root->isEndOfWord = 0;
        }
        else
        {
            // Memory allocation failed for root node
            free(pTrie); // Free allocated memory for trie structure
            return NULL; // Return NULL to indicate failure
        }
    }
    return pTrie; // Return pointer to the trie structure
}

// Helper function to get index of character 'c'
int getIndex(char c)
{
    return c - 'a'; // Assuming only lowercase alphabets
}

// Inserts the word into the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = getIndex(word[i]);
        if (!current->children[index])
        {
            current->children[index] = (struct TrieNode *)malloc(sizeof(struct TrieNode));
            for (int j = 0; j < 26; j++)
                current->children[index]->children[j] = NULL;
            current->children[index]->isEndOfWord = 0;
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// Helper function to check if node is leaf node
int isLeafNode(struct TrieNode *node)
{
    for (int i = 0; i < 26; i++)
    {
        if (node->children[i])
            return 0;
    }
    return 1;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = getIndex(word[i]);
        if (!current->children[index])
            return 0; // Word not found
        current = current->children[index];
    }
    return current->isEndOfWord;
}

// Helper function to deallocate trie nodes
void deallocateHelper(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < 26; i++)
        {
            deallocateHelper(node->children[i]);
        }
        free(node);
    }
}

// Deallocates the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateHelper(pTrie->root);
        free(pTrie);
    }
}

// Reads the dictionary from file and returns the number of words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char word[100]; // Assuming maximum word length is 100 characters

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = (char *)malloc((strlen(word) + 1) * sizeof(char));
        strcpy(pInWords[numWords], word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    if (numWords == 0)
    {
        printf("No words found in the dictionary\n");
        return 1; // Exit program with failure status
    }
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create trie structure
    struct Trie *pTrie = createTrie();
    if (!pTrie) // Check if creation of trie failed
    {
        printf("Failed to create trie\n");
        return 1; // Exit program with failure status
    }

    // Insert words into trie
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test with some words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate trie
    deallocateTrie(pTrie);

    // Deallocate dynamically allocated memory for words
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}
