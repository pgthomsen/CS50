/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // check that n is positive, return false if not
    if (n < 0)
    {
        return false;
    }
    // check if either the nth or n/2th term in the array are the desired value
    if(value == values[n]||value == values[n/2]){
        return true;
    }
    // ensure that the search has not extended beyond the size of the array
    // because if it has that indicates the value is not present
    // return false if this is the case
    else if(values[n] == 0 && values[n/2] == 0)
    {
        return false;
    }
    // check if the values is less than the middle term, if so send back to search()
    // but telling the function only to treate the array as if it contains the first
    // lesser half of its contents, return true if value present in this half, false if not
    else if(value < values[n/2])
    {
        if (search(value, values, n/2) == true)
        {
            return true;
        }
        return false;
    }
    // if the value is greater than the n/2 term in the array, send to the array back into search()
    // but this time telling it to search through the array assuming it to be twice the previously
    // assumed maximum size, helping to further narrow the possible range
    else if(value > values[n/2])
    {
        if (search(value, values, (n*2)-1) == true)
        {
            return true;
        }
        return false;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int c;
    // implement bubble sort
    do {
        // check if any changes have been made, continuing on only when no changes are made
        c = 0;
        for(int i=0; i<n-1; i++)
        {
            //check if value[i] is greater than the next value in the array, if so, switch
            // the two values and increase count of changes made
            if (values[i] > values[i+1])
            {
                int x = values[i];
                values[i] = values[i+1];
                values[i+1] = x;
                c++;
            }
        }
        //check if changes have been made, repeating loop while changes made
    } while (c != 0);
    return;
}