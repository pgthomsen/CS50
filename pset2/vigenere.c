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
    // create string k from user input and verify that it contains only letters
    string keyword = argv[1];
    
    for(int i = 0; i < strlen(keyword); i++)
    {
        if (isalpha(keyword[i]) == false)
        {
            printf("Input Error\n");
            return 1;
        }
        
    }
    // promt for plaintext and store in string plaintext
    printf("plaintext:");
    string plaintext = get_string();
    printf("ciphertext:");
    // create array of size length of keyword and insert the 0-25 value of
    // each letter of the keyword into its corresponding location
    // in the array
    int karray [strlen(keyword)];
    for (int i = 0; i < strlen(keyword); i++)
    {
        if (isupper(keyword[i]))
        {
            int kupper = keyword[i] - 65;
            karray[i] = kupper;
        }
        else
        {
            int klower = keyword[i] - 97;
            karray[i] = klower;
        }
    }
    // initialize j and k so that they exist beyond their usage in
    // the upcoming for loop
    int j = 0;
    int k = 0;
    // go through each location in plaintext and, if it is a letter, gain a 
    // corresponding k value to rotate by and increase the k
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // check if the character is a letter, if so change the location 
        // inside the array karray to be used for rotation, 
        // otherwise leave the location in the array the same
        if (isalpha(plaintext[i]))
        {
            int k_location = j % strlen(keyword);
            k = karray[k_location];
            j++;
        }
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