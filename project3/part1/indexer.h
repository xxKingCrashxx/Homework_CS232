
#ifndef INDEXER_H_
#define INDEXER_H_
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

TrieNode* indexPage(const char *url, int* totalNumTerms);

int char_to_index(char c);

/*
  creates a TrieNode object in the heap memory and returns a pointer of the memory location on the heap.
*/
TrieNode* createNode(void);

/*
  returns a char * to the heap that contains the string in lowercase form.
  pointer needs to be freed after use.
*/
void toLowerCase(char* str, char* lowerStr, int wordLen);

/*
  adds a string to the TrieNode structure.
*/
void addWordOccurrence(char* word, TrieNode * root);

/*
  helper function that adds the word recursively
*/
void addWordOccurrence_R(char*, TrieNode*, int, int);

/*
  prints out the words contained in the node and displays the number of times the word of seen
*/
void printTrieContents(TrieNode *ptr);
void printTrieContents_R(TrieNode *root, char* prefix, int len);

/*
  function that free's the TrieNode and all of its connected children
*/
int freeTrieMemory(TrieNode *ptr);

/*
  goes down the root trienode and looks for all nodes marked as leafnodes
  adds 1 to the total and recursively adds one until all leafnodes have been found.
*/
int getTrieWordCount(TrieNode *root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

#endif