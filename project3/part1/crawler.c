#include "crawler.h"

int contains(const struct listNode *pNode, const char *addr){
  // TODO: complete this
  if(pNode == NULL)
  {
    return 0;
  }
  else if(isMatch(pNode->addr, addr) == 1)
  {
    return 1;
  }
  return contains(pNode->next, addr);
}

struct listNode* createListNode(const char* url)
{
    struct listNode *nextNode = malloc(sizeof(struct listNode)*1);
    nextNode->next = NULL;
    nextNode->root = NULL;
    nextNode->totalTerms = 0;
    
    strncpy(nextNode->addr, url, MAX_ADDR_LENGTH);

    return nextNode;

}

int isMatch(const char *str1, const char *str2)
{
  int str1Len, str2Len, counter = 0;
  str1Len = strlen(str1);
  str2Len = strlen(str2);
  
  if(str1Len != str2Len)
    return 0;
  
  while(str1[counter] != '\0' || str2[counter] != '\0')
  {
      if(str1[counter] != str2[counter])
        return 0;
    counter++;
  }
  return 1;
}
    
void insertBack(struct listNode **pNode, const char *addr)
{
  // TODO: complete this
  if(*pNode == NULL)
  {
    *pNode = createListNode(addr);
    if(*pNode == NULL)
      return;

    (*pNode)->root = indexPage(addr, &((*pNode)->totalTerms));
    if((*pNode)->root == NULL)
      return;
  }
  else if((*pNode)->next == NULL)
  {
    struct listNode *nextNode = createListNode(addr);
    if(nextNode == NULL)
      return;

    nextNode->root = indexPage(addr, &(nextNode->totalTerms));
    if(nextNode->root == NULL)
      return;
    (*pNode)->next = nextNode;
  }
  else
  {
    insertBack(&(*pNode)->next, addr);
  }

}

void printAddresses(const struct listNode *pNode){
  if(pNode == NULL)
        return;
    printf("%s\n", pNode->addr);
    printTrieContents(pNode->root);

    printAddresses(pNode->next);
}

void destroyList(struct listNode *pNode){
  if(pNode == NULL)
    return;
  freeTrieMemory(pNode->root);
  destroyList(pNode->next);
  free(pNode);
}

int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}