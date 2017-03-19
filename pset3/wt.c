#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

void init(void);
void draw(void);
bool won(void);
int d;
int board[9][9];

int main(void)
{
    d = 3;
    init();
    draw();
    if(won() == true)
    {
        printf("Victory!");
    }
    else{ printf("defeat");}
    return 0;
}

void init(void)
{
    // TODO
    int mvalue = 1;
    for(int c=0; c < d; c++)
    {
        for(int r=0; r<d; r++)
        {
            board[c][r] = mvalue;
            mvalue++;
        }
    }
    // checks if there are an odd numeber of game pieces and switched the 1 and 2 squares if so
    /*if(((d*d)-1)%2 == 1)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }*/
    board[d-1][d-1] = 0;
}

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

bool won(void)
{
    // TODO
    int errors = 0;
    for(int c=0; c<d;c++)
    {
        for(int r=1; r<d; r++)
        {
            if(board[c][r] -1 != board[c][r-1])
            {
                errors++;
            }
            if(board[c][r] == 0 && c == d-1 && r == d-1)
            {
                errors--;
            }
        }
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