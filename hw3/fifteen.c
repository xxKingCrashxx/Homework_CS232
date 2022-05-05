/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void greet(void);
void init(void);
void draw(void);
short move(int tile);
short won(void);
int get_int();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Get an non-negative integer from user input
 * If the input is not non-negative integer, return INT_MAX
 */
 int get_int()
 {
    int input = 0;
    short invalid = 0;

    char c = getchar();
    if (c == '\n')
        return INT_MAX;

    while (c != '\n') 
    {
        if ((c >= '0') && ( c <= '9'))
        {
            input = input * 10 + (c - '0');
        } else {
            invalid = 1;
        }

        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else 
        return input;
 }

/**
 * Greets player.
 */
void greet(void)
{    
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    //TODO
    /*
    * i = row
    * j = column
    */
    int dimensionArea = d*d;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = dimensionArea - 1;
            dimensionArea--;

            if(d % 2 == 0 && dimensionArea == 1)
            {
                board[i][j] = 2;
                board[i][j - 1] = 1;
            }
            //bottom right of 2d array
            if(board[i][j] == 0)
            {
                board[i][j] = (int)'_';
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {
            if( board[row][col] == (int)'_')
            {
                printf("|%2c|", (char)board[row][col]);
            }
            else
                printf("|%02d|", board[row][col]);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns 1, else
 * returns 0.
 */
short move(int tile)
{
    
    // TODO
    //row
    for(int row = 0; row < d; row++)
    {
        //col
        for(int col = 0; col < d; col++)
        {
            //if '_' is found below tile
            if(board[row][col] == tile && board[row + 1][col] == (int)'_')
            {
                
                board[row][col] = board[row + 1][col];
                board[row + 1][col] = tile;
                return 1;
            }
            //if '_' is found above tile
            else if(board[row][col] == tile && board[row - 1][col] == (int)'_')
            {
                board[row][col] = board[row - 1][col];
                board[row - 1][col] = tile;
                return 1;
            }
            //if '_' is found to the right of tile
            else if(board[row][col] == tile && board[row][col + 1] == (int)'_')
            {
                board[row][col] = board[row][col + 1];
                board[row][col + 1] = tile;
                return 1;
            }
            //if '_' is found to the left of tile
            else if(board[row][col] == tile && board[row][col - 1] == (int)'_')
            {
                board[row][col] = board[row][col - 1];
                board[row][col - 1] = tile;
                return 1;
            }

        }
    }
    return 0;
}

/**
 * Returns 1 if game is won (i.e., board is in winning configuration),
 * else 0.
 */
short won(void)
{
    //
    int dimensionArea = 1;
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {
            if(board[row][col] != dimensionArea)
                return 0;
            else
                dimensionArea++;
            
            if(dimensionArea == d*d)
            {
                break;
            }
        }
    }
    return 1;
}
