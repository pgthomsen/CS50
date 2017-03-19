#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("How much change is owed?\n:");
    float chng = get_float();
    while (chng<0)
    {
        printf("Please provide a positive change value\n:");
        chng = get_float();
    }
    chng = chng*100;
    int change = round(chng);
    int coins = 0;
    while (change>24)
    {
        change = change - 25;
        coins++;
    }
    while (change>9)
    {
        change = change - 10;
        coins++;
    }
    while (change>4)
    {
        change = change - 5;
        coins++;
    }
    while (change>0)
    {
        change = change-1;
        coins++;
    }
    printf("%i\n",coins);
}