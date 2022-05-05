#ifndef CRAWLER_H_
#define CRAWLER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "indexer.h"

#define MAX_ADDR_LENGTH 1000

/*
 * a node in our linked-list of web addresses
 * contains a trienode structure of all the words on the page
 */
struct listNode{
  char addr[MAX_ADDR_LENGTH];
  int totalTerms;
  struct listNode *next;
  struct TrieNode *root;
};

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr);

/*
  creates a listnode structure and returns a pointer to the structure
  pointing to the heap.
*/
struct listNode* createListNode(const char* url);

/*
 * inserts new nodes at the end of the linked list with the address.
 * inserts at the head of the linked list if the head is null
 * 
 */
void insertBack(struct listNode **pNode, const char *addr);

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode);

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode);
  
/*
 * srcAddr should be a web address (e.g., http://www.yahoo.com).
 * link should point to an array of maxLinkLength characters.
 * getLink returns 1 if a link was found and 0 otherwise.
 * If a link was found, "link" will be filled in with the web address.
 */
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

/*
*isMatch compares two string:
*returns 1 if both strings match.
*returns 0 if the strings do not match.
*/
int isMatch(const char*, const char*);

#endif