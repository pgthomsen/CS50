#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("Let's make a half pyramid!! To start, please provide a height for the pyramid that is between 0 and 23! Your number can also be 0 or 23 if you want!\n:");
    int height = get_int();
    while (height > 23)
    {
        printf("please enter a number within the range\n:");
        height = get_int();
    }
    while (height < 0)
    {
        printf("please enter a number greater than or equal to 0\n:");
        height = get_int();
    }
    int hashes = 2;
    while (height > 0)
    {
        int spaces = height - 1;
        while (spaces > 0)
        {
            printf(" ");
            spaces--;
        }
        int hashes1 = hashes;
        while (hashes1 > 0)
        {
            printf("#");
            hashes1--;
        }
        printf("\n");
        height--;
        hashes++;
    }
}