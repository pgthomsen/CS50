#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("How many minutes do you usually spend in the shower per shower?\n:");
    int minutes = get_int();
    int water = minutes * 12;
    printf("\nYour showers are usually %i minutes long and use the equivalent of", minutes);
    printf(" %i bottles of water!!!\n", water);
}