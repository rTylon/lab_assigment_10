#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Trie *createTrie();
// Trie structure
typedef struct Trie
{	
    char data;
    int is_End_Of_String;
    struct Trie* children[26];
    int count;
}Trie;

// Inserts the word to the trie structure
void insert(Trie *pTrie, char *word)
{
    Trie *temp = pTrie;
    for (int i = 0; i < strlen(word); i++)
    {
        if (temp->children[word[i] - 'a'] == NULL)
            temp->children[word[i] - 'a'] = createTrie();
        temp = temp->children[word[i] - 'a'];
    }
    temp->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(Trie *pTrie, char *word)
{
    Trie* curr = pTrie;
    for (int i = 0; word[i]; i++)
    {
        if (curr->children[word[i] - 'a'] == NULL)
        {
            return 0;
        }
        curr = curr->children[word[i] - 'a'];
    }
    return curr->count;
}

// deallocate the trie structure
Trie *deallocateTrie(Trie *pTrie)
{
    if (pTrie == NULL)
        return NULL;

    for (int i = 0; i < 26; i++)
    {
        if (pTrie->children[i] != NULL)
            pTrie->children[i] = deallocateTrie(pTrie->children[i]);
    }

    free(pTrie);
    return NULL;
}


// Initializes a trie structure
Trie *createTrie(void)
{
    Trie *pNode = malloc(sizeof(Trie));
    pNode->count = 0;
    if (pNode)
    {
        for (int i = 0; i < 26; i++)
            pNode->children[i] = NULL;
    }
    pNode->is_End_Of_String = 0;
    return pNode;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char ***pInWords)
{
    FILE *inFile = fopen(filename, "r");
    int dictSize = 0;
    char c;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dictSize);
        *pInWords = malloc(sizeof(char *) * dictSize);

        if (*pInWords == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (int i = 0; i < dictSize; ++i)
        {
            char word[100];
            fscanf(inFile, "%s", word);

            (*pInWords)[i] = malloc(strlen(word) + 1);
            if ((*pInWords)[i] == NULL)
            {
                printf("Cannot allocate memory\n");
                exit(-1);
            }
            strcpy((*pInWords)[i], word);
        }
        fclose(inFile);
    }
    return dictSize;
}

int main(void)
{
    char **inWords = NULL;
    int numWords = readDictionary("dictionary.txt", &inWords);
    
    // Read the number of words in the dictionary
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create and populate the trie structure
    Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    //parse lineby line
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
        for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    // Deallocate the trie
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}
