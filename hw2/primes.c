/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int isPrime(int);

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1){
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
  
    {
        /* TODO: fill in the code that outputs the prime numbers */
        /*   in the range [lowerBound,upperBound] in ascending order */
        for(int i = lowerBound; i <= upperBound; i++){

            if(i == 2)
                printf("%d\n", i);
            else if( i == 3)
                printf("%d\n", i);
            else if(isPrime(i) == 1)
            {
                printf("%d\n", i);
            }
        }

    }

    return 0;
}

int isPrime(int num)
{
    if(num <= 1)
        return 0;

    for(int i = 2; i <= num - 1; i++){
        if(num % i == 0)
            return 0;
    }
    return 1;
}
