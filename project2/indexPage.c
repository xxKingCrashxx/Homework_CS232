/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define CHAR_SIZE 26
#define BUFF_SIZE 300000

/* TODO: structure definitions */
typedef struct TrieNode
{
  //number of times specific char value is seen
  int charOccurance;

  //boolean value determining if the node is the end of a word.  
  //The last char in a word being added to the trienode will be flagged as root -> isCompleteWord = 1;
  int isCompleteWord;

  //ptr array of TrieNode structures holds a maximum of 26 nodes
  //converting letters in lowercase should be handled in the index function.
  //the index where the children are located act as the index where the letters are.
  //children['char value' - a] is the index of char c
  struct TrieNode *children[CHAR_SIZE];

}TrieNode;

TrieNode* indexPage(const char *url);

/*
  creates a TrieNode object in the heap memory and returns a pointer of the memory location on the heap.
*/
TrieNode* createNode(void);

/*
  adds a string to the TrieNode structure.
*/
int addWordOccurrence(const char* word, TrieNode * root);

void addWordOccurrence_R(char *str, TrieNode* root, int index);

char* toLowerCase(char* str, int wordLen);

/*
  prints out the words contained in the node and displays the number of times the word of seen
*/
void printTrieContents(TrieNode *ptr);

void printTrieContents_R(TrieNode* root, char* prefix, int len);

/*
  function that free's the TrieNode and all of its connected children
*/
int freeTrieMemory(TrieNode *ptr);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  
  if(argc != 2)
  {
    fprintf(stderr, "%s: Not enough commandline arguments\n", argv[0]);
    return -1;
  }
  
  TrieNode* root = indexPage(argv[1]);
  printTrieContents(root);
  freeTrieMemory(root);

  return 0;
}

TrieNode* createNode(void){

  TrieNode *pNode = malloc(sizeof(TrieNode) * 1);
  if(pNode != NULL)
  {
    pNode -> isCompleteWord = 0;
    pNode -> charOccurance = 0;

    for (int i = 0; i < CHAR_SIZE; i++)
    {
      pNode -> children[i] = NULL;
    }
  }
  
  return pNode;
}

TrieNode* indexPage(const char* url)
{

  //char array which will store the char contents of the web as one big string.
  char rawData[BUFF_SIZE];
  int byteCount = getText(url, rawData, BUFF_SIZE);

  TrieNode* root = createNode();

//removing all special chars and spaces and replaceing them with '.'
  for(int i= 0; i < byteCount; i++)
  {
    char currentChar = rawData[i];

    if(isspace(currentChar) || isdigit(currentChar) || ispunct(currentChar))
    {
      rawData[i] = '.';
    }
  }

  char* token = NULL;
  char* ptrData = rawData;
  char **saveData = &ptrData;

//tokenizes string and sends each word to addWordOccurence
  printf("%s\n", url);
  while((token = strtok_r(ptrData, ".", saveData)))
  {
      printf("\t%s\n", token);
      addWordOccurrence(token, root);
  }

  return root;
}

char* toLowerCase(char* str, int wordLen)
{
  char* lowerStr = malloc(sizeof(char)*wordLen + 1);

    //convert word into lowercase version
    int counter = 0;
    while(str[counter] != '\0')
    {
        if(isupper(str[counter]))
            lowerStr[counter] = (char)(tolower(str[counter]));
        else
            lowerStr[counter] = str[counter];
        counter++;
    }
    lowerStr[wordLen + 1] = '\0';
    return lowerStr;
}

int addWordOccurrence(const char* word, TrieNode *root)
{
    int wordLen = strlen(word);
    char* lowerStr;

    //convert word into lowercase version
    lowerStr = toLowerCase(word, wordLen);

    //recursively goes down each node and adds the character of the string.
    addWordOccurrence_R(lowerStr, root, 0);
    free(lowerStr);
  
}

void addWordOccurrence_R(char *str, TrieNode* root, int index)
{
    if(str[index] == '\0')
    {
        root->isCompleteWord = 1;
        root->charOccurance++;
        return;
    }

    if(root->children[(int)str[index] - (int)'a'] == NULL)
    {
        root->children[(int)str[index] - (int)'a'] = createNode();
    }  
    addWordOccurrence_R(str, root->children[(int)str[index] - (int)'a'], index + 1);
    
}

//work in progress
void printTrieContents(TrieNode *root)
{
  if(root == NULL)
    {
        printf("end of trie\n");
        return;
    }
    printTrieContents_R(root, NULL, 0);
}

void printTrieContents_R(TrieNode* root, char* prefix, int len)
{
  char newPrefix[len+2];
  strncpy(newPrefix, prefix, len);
  newPrefix[len+1] = '\0';

  if(root->isCompleteWord)
  {
    printf("%s: %d\n", prefix, root->charOccurance);
  }

  for (int i = 0; i < CHAR_SIZE; i++)
  {
    if(root->children[i] != NULL)
    {
      newPrefix[len] = (i + 'a');
      printTrieContents_R(root->children[i], newPrefix, len + 1);
    }
  }

}

int freeTrieMemory(TrieNode *root)
{
  if(root == NULL)
    {
        //nothing to free
        return 0;
    }

    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if(root->children[i] != NULL)
        {   
            freeTrieMemory(root->children[i]);
        }
    }
    free(root);
    //successfully freed the root node and all of its children nodes and its children...etc.
    return 1;  

}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
