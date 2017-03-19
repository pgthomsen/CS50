#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Prompt User For name and receive input in string name
    string name = get_string();
    // verify that the string received is not null and reprompt if it is
    while (name == NULL)
    {
        name = get_string();
    }
    // print the first letter of the name in upper case letters
    printf("%c", toupper(name[0]));
    // progress through each character of the name, printing out
    // the next character anytime a space is reached and 
    // continuing until the end of the string
    for(int i=0; i < strlen(name); i++)
    {
        string space = " ";
        int s = space[0];
        int letter = name[i];
        if (letter == s)
        {
            // printing the character after the space in upper case
            printf("%c",toupper(name[i + 1]));
        }
    }
    // new line at the end of the program
    printf("\n");
}