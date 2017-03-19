/**
 * Implements a dictionary's functionality.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHTABLESIZE 5000

// declaration of hash function
int hashf(const char *str);
int wordnum;

// initialize hashtable array
node2 hashtable[HASHTABLESIZE];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // create new storage for word that is all lower case letters
    char dword[LENGTH+1];
    int j = 0;
    while(word[j] != '\0')
    {
        dword[j] = tolower(word[j]);
        j++;
    }
    // insert end of word character
    dword[j] = '\0';
    // hash word
    int hashed = hashf(dword);
    int i = 0;
    // locate hash within the hashtable
    while(hashtable[i].hash != hashed && i<HASHTABLESIZE)
    {
        i++;
    }
    // if the end of the hash table is reached and the hash does not match the last hash
    // the word must not be in the hash table so it must be mispelled
    if (hashed != hashtable[i].hash)
    {
        return false;
    }
    // otherwise search by stepping through each node, return true if word match is found
    else if (search(dword, hashtable[i].next) == true)
    {
        return true;
    }
    
        return false;
}

// recursive search function that allows searching by stepping through the nodes
bool search(const char *dword, node *wordnode)
{
    if(strcmp(dword, wordnode -> word)== 0)
    {
        return true;
    }
    else if(wordnode->next != NULL)
    {
        if(search(dword, wordnode->next) == true)
        {
            return true;
        }
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary, return error if unable to open
    FILE *dict = fopen(dictionary, "r");
    
    if (dict == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    wordnum = 0;
    char nodename[11];
    char dword[LENGTH + 1];
    int arrayloc = 0;
    
    // go through the dictionary one word at a time
    while (fscanf(dict, "%s", dword) != EOF)
    {
        // hash the word
        int hashed = hashf(dword);
        
        // go through the hashtable, if the hash is present at a given location insert the new node in the section, otherwise
        // if there is no hash present at the location insert the hash and then the node
        for (int i=0; i <= arrayloc; i++)
        {
            if (hashtable[i].hash == hashed)
            {
                sprintf(nodename, "node%06i", wordnum);
                node *nodename = malloc(sizeof(node));
        
                if(nodename == NULL)
                {
                    unload();
                    return false;
                }
        
                strcpy(nodename->word, dword);
                nodename -> next = hashtable[i].next;
                hashtable[i].next = nodename;
        
            }
            else if (hashtable[i].hash == 0 && hashtable[i].next == NULL)
            {
                hashtable[i].hash = hashed;
                hashtable[i].next = NULL;
                
                sprintf(nodename, "node%06i", wordnum);
                node *nodename = malloc(sizeof(node));
        
                if(nodename == NULL)
                {
                    unload();
                    return false;
                }
                
                strcpy(nodename->word, dword);
                nodename -> next = hashtable[i].next;
                hashtable[i].next = nodename;
                
                arrayloc++;
                break;
            }
        }
        
        // keep track of the number of words
        wordnum++;
    }
    // close the dictionary
    fclose(dict);
    return true;
}

// djb2 hash from http://www.cse.yorku.ca/~oz/hash.html
int hashf(const char *str)
    {
        unsigned long hash = 5381;
        char c;
        for (int i=0; i < strlen(str); i++)
        {
            c = str[i];
            hash = ((hash << 5) + hash) + c;
        }
        hash = (hash % HASHTABLESIZE);
        return hash;
    }

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordnum;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // go through the entire hashtable freeing the associated nodes
    for(int i = 0; i < HASHTABLESIZE; i++)
    {
        if(hashtable[i].next != NULL)
        {
            if(freemem(hashtable[i].next) == false)
                {
                    return false;
                }
        }
    }
    return true;
}

// recursively free memory by freeing the current node after sending the next node to be freed until the next node is null
bool freemem(node *curnode)
{
    if(curnode->next == NULL)
    {
        free(curnode);
        return true;
    }
    else{
        freemem(curnode->next);
        free(curnode);
    }
    return true;
}
