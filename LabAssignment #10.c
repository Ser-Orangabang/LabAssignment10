#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_CHARS 256

struct Trie *createTrie();

// Trie structure
struct Trie
{	
  struct Trie *children[NUM_CHARS];
  bool terminal;
  int count;
};

// Inserts the word to the trie structure
bool insert(struct Trie **root, char *word)
{
  if (*root == NULL)
  {
    *root = createTrie();
  }

  unsigned char *text = (unsigned char *)word;
  struct Trie *tmp = *root;
  int length = strlen(word);

  for (int i = 0; i < length; i++)
    {
      if(tmp->children[text[i]] == NULL)
      {
        //Create a new node
        tmp->children[text[i]] = createTrie();
      }
      tmp = tmp->children[text[i]];
    }
  if (tmp->terminal)
  {
    tmp->count++; // Increment the count by 1 if the word is already in the trie
    return false;
  }
  else
  {
    tmp->terminal = true;
    tmp->count++;
    return true;
  }
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    unsigned char *text = (unsigned char *)word;
    struct Trie *tmp = pTrie;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if(tmp->children[text[i]] == NULL)
        {
            return 0;
        }
        tmp = tmp->children[text[i]];
    }

    if (tmp != NULL && tmp->terminal) {
        return tmp->count;
    } else {
        return 0;
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
  //if root trie is null
  if (pTrie == NULL){
    return NULL;
  }
  for (int i = 0 ; i < NUM_CHARS ; i++){
    if (pTrie->children[i] == NULL){
      continue;
    }
    pTrie->children[i] = deallocateTrie(pTrie->children[i]); 
  }

  //free the memory allocated for the current trie
  free(pTrie);
  //return NULL
  return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
  struct Trie *newnode = malloc(sizeof *newnode);

  for (int i=0; i<NUM_CHARS; i++)
    {
      newnode -> children[i] = NULL;
    }
  newnode -> terminal = false;
  newnode -> count = 0;
  return newnode;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
  FILE* inFile = fopen(filename, "r");
  if (inFile == NULL) return -1;

  int count = 0;

  fscanf(inFile, "%d\n", &count);
  char word[100];
  for (int i = 0; i < count; i++)
    {
      fscanf(inFile, "%s\n", word);
      pInWords[i] = (char*)calloc(strlen(word), sizeof(char));
      strcpy(pInWords[i], word);
    }

  return count;
}

int main(void)
{
  char *inWords[256];

  //read the number of the words in the dictionary
  int numWords = readDictionary("dictionary.txt", inWords);
  for (int i=0;i<numWords;++i)
  {
    printf("%s\n",inWords[i]);
  }

  struct Trie *pTrie = createTrie();
  for (int i=0;i<numWords;i++)
  {
    insert(&pTrie, inWords[i]);
  }
  // parse lineby line, and insert each word to the trie data structure
  char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
  for (int i=0;i<5;i++)
  {
    printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
  }
  pTrie = deallocateTrie(pTrie);
  if (pTrie != NULL)
    printf("There is an error in this program\n");
  return 0;
}