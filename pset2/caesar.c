#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
   // verify that the input is acceptable and return an error message and 1 if
   // anything other than one command line argument is given
    while (argc != 2)
    {
        printf("Input Error\n");
        return 1;
    }
    // promt for plaintext and store in string plaintext
    printf("plaintext:");
    string plaintext = get_string();
    printf("ciphertext:");
    // create int variable k from the command line input 
    int k = atoi(argv[1]);
    // for loop goes through every part of plaintext cheching if 
    // what is at the array location i is an upper case letter, 
    // a lower case letter, or anything else
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // check for upper case letters, create integer if it is an
        // upper case letter of that letter on a 0-25 scale to make modulating
        // with the k mod 26 give a 0-25 number which can be 
        // retranslated into an upper case letter by adding 65
        if (isupper(plaintext[i]))
        {
            int letter = plaintext[i] - 65;
            letter = (letter + k) % 26;
            letter = letter + 65;
            printf("%c", letter);
        }
        // check for a lower case letter and, if it is one, translate it into an
        // integer between 0-25, compute its new value with k mod 26, and
        // retranslate into a lower case number by adding 97
        else if (islower(plaintext[i]))
        {
            int letter = plaintext[i] - 97;
            letter = (letter + k) % 26;
            letter = letter + 97;
            printf("%c", letter);
        }
        // if the value of plaintext[i] is not a letter, simply print the value
        else 
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}