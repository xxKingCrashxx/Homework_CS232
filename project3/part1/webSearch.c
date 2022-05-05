#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//user defined headers
#include "indexer.h"
#include "crawler.h"



int main(int argc, char** argv)
{
    int MAX_N;
    long seed;

    if(argc < 3 || argc > 4)
    {
        fprintf(stderr, "USAGE: %s [file url] [maximum # of pages] [rand seed]", argv[0]);
        return -1;
    }

    if(argc >= 4)
    {
        seed = atol(argv[3]);
    }
    else
    {
        seed = time(NULL);
    }
    srand(seed);

    MAX_N = atoi(argv[2]);
    if(!MAX_N)
    {
        fprintf(stderr, "Could not recognize commandline argument: [%s]", argv[2]);
        return -2;
    }
    
    FILE* data = fopen(argv[1], "r");
    if(data == NULL)
    {
        fprintf(stderr, "File could not be found: [%s]", argv[1]);
        return -3;
    }

    int n = 0;
    //char url[MAX_ADDR_LENGTH];
    char buffer[MAX_ADDR_LENGTH];
    
    struct listNode* head = NULL;


    printf("indexing...\n");
    while((fgets(buffer, MAX_ADDR_LENGTH, data) != NULL) && n < MAX_N)
    { 
        int hopNum = 0;
        int hopsLim = 0;

        char url[MAX_ADDR_LENGTH];

        sscanf(buffer, "%s %d", url, &hopsLim);
        while(1)
        {
            if(contains(head, url) == 0)
            {
                //modified to insert into the head of the node if the head is Null.
                //also indexes the link assigned to that node and fills in the rest of the values the node contains
                insertBack(&head, url);
                n++;
            }
            hopNum++;

            if(hopNum <= hopsLim && n < MAX_N)
            {
                //char newUrl[MAX_ADDR_LENGTH];
                int res = getLink(url, url, MAX_ADDR_LENGTH);

                if(!res)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    
    printAddresses(head);
    destroyList(head);
    fclose(data);

    
}
