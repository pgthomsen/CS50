/**
 * fifteen.c
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

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
int tilerow(int tile);
int tilecol(int row, int tile);

int main(int argc, string argv[])
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
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

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
            usleep(5000/*00*/);
        }

        // sleep thread for animation's sake
        usleep(50000/*0*/);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // TODO
    int mvalue = (d*d) - 1;
    for(int c=0; c < d; c++)
    {
        for(int r=0; r<d; r++)
        {
            board[c][r] = mvalue;
            mvalue--;
        }
    }
    // checks if there are an odd numeber of game pieces and switched the 1 and 2 squares if so
    if(((d*d)-1)%2 == 1)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    for(int c=0; c<d; c++)
    {
        for(int r=0; r<d; r++)
        {
            //prints "_" instead of 0 for game display purposes
            if(board[c][r] == 0)
            {
                printf("_ \t");
            }
            else{
            printf("%i \t",board[c][r]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    if(tile>((d*d)-1))
    {
        return false;
    }
    int row = tilerow(tile);
    if(row == 15)
    {
        return false;
    }
    int col = tilecol(row, tile);
    // check same row previous collumn, switching if free space
    if (board[col-1][row] == 0 && col>0)
    {
        board[col-1][row] = board[col][row];
        board[col][row] = 0;
        return true;
    }
    // check same row next collumn, switching if free space
    if (board[col+1][row] == 0 && col<d-1)
    {
        int x = board[col][row];
        board[col+1][row] = x;
        board[col][row] = 0;
        return true;
    }
    // check in space bellow and switching if free
    if (board[col][row-1] == 0 && row>0)
    {
        board[col][row-1] = board[col][row];
        board[col][row] = 0;
        return true;
    }
    // check space above and switching if free
    if (board[col][row+1] == 0 && row<d-1)
    {
        board[col][row+1] = board[col][row];
        board[col][row] = 0;
        return true;
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    // error counter
    int errors = 0;
    // cycle through all squares, ensuring that each is 1 greater than the previous
    for(int c=0; c<d;c++)
    {
        for(int r=1; r<d; r++)
        {
            if(board[c][r] -1 != board[c][r-1])
            {
                errors++;
            }
            // correction for false errors in the previous checker for the last square
            // because it is 0 so it fails that test
            if(board[c][r] == 0 && c == d-1 && r == d-1)
            {
                errors--;
            }
        }
            // check the first number on the line against last number on previous line
            if(board[c][0] != 1+ board[(c-1)][d-1])
            {
                errors++;
            }
    }
    if(errors == 0)
    {
        return true;
    }
    return false;
}

// function to determine the row of the tile piece
int tilerow(int tile)
{
    for(int r=0;r<d;r++)
    {
        for(int c=0;c<d; c++)
        {
            if(board[c][r]==tile)
            {
                return r;
            }
        }
    }
    return 15;
}
// function to determine the collumn of the tile piece
int tilecol(int row, int tile)
{
    for(int c=0;c<d;c++)
    {
        if(board[c][row]==tile)
        {
            return c;
        }
    }
    return 15;
}