#include "indexer.h"

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

TrieNode* indexPage(const char* url, int* totalNumTerms)
{
  //char array which will store the char contents of the web as one big string.
  char rawData[BUFF_SIZE];
  //char cpyRawData[BUFF_SIZE];
  int bytesRead = getText(url, rawData, BUFF_SIZE);

  TrieNode* root = createNode();

//removing all special chars and spaces and replaceing them with '.'
  for(int i= 0; i <= bytesRead; i++)
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
  while((token = strtok_r(ptrData, ".", saveData)))
  {
      addWordOccurrence(token, root);
  }
  //printTrieContents(root);

  *totalNumTerms = getTrieWordCount(root);
  return root;
}

void toLowerCase(char* str, char* lowerStr, int wordLen)
{
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
}

void addWordOccurrence(char* word, TrieNode *root)
{
    int wordLen = strlen(word);
    char lowerStr[wordLen + 1];

    //convert word into lowercase version
    toLowerCase(word, lowerStr, wordLen);

    //recursively goes down each node and adds the character of the string.
    addWordOccurrence_R(lowerStr, root, wordLen, 0); 
}
void addWordOccurrence_R(char *str, TrieNode* root, int wordLen, int index)
{
    if(index >= wordLen)
    {
      root->isCompleteWord = 1;
      root->charOccurance++;
      return;
    }    

    if(root->children[(int)str[index] - (int)'a'] == NULL)
    {
        root->children[(int)str[index] - (int)'a'] = createNode();
    }
    addWordOccurrence_R(str, root->children[(int)str[index] - (int)'a'], wordLen, index + 1);
    
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
    printf("\t%s\n", prefix);
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

int getTrieWordCount(TrieNode *root)
{
    int total = 0;
    
    if(root->charOccurance > 0)
        total += root->charOccurance;
        
    for(int i = 0; i < CHAR_SIZE; i++)
    {
        if(root->children[i] != NULL)
             total += getTrieWordCount(root->children[i]);
    }
    return total;  
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