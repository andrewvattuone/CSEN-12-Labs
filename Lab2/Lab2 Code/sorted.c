/*Andrew Vattuone, sorted.c, 10/4/23
 *Various functions that are used by files unique.c and parity.c to perform several functions that analyze text. Functions that analyze text include finding the number of unique words in a text, removing words from the set if a second text is inputted and printing out the number of remaining words, listing out these remaining words, and finding the number of words that occur an odd number of times in a text. The words of text files are held in an array of strings within a set structur along with variables that track the number of elements within the array and the maximum nmber of elements the array can store. The array in this file is sorted in alphabetical order, so functions are modified from the previous file to deal with sorted arrays rather than unsorted ones.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include <stdbool.h>


static int search(SET *sp, char *elt, bool *found);

//definition of the structure set
typedef struct set
{
	char **data;
	int length;
	int count;
}SET;


/*Creates the set, taking in an argument maxElts that represents the maximum number of words that can be stored in the array data. The set and array data are both allocated the appropriate amount of memory, length is set to maxElts, and count is set to 0. Assert is used to make sure that none of the pointers used are null. 
 *Big-O notation: O(1) 
 */
SET *createSet(int maxElts)
{
        SET *setptr = malloc(sizeof(SET));
        assert(setptr != NULL);
        setptr->length = maxElts;
        setptr->data = malloc(sizeof(char*)*maxElts);
        assert(setptr->data != NULL);
        setptr->count = 0;
        return setptr;
}

/*Deallocates memory from inputted set sp, first freeing every memory location in data and then freeing the memory of the set sp itself. Assert is used to make sure none of the pointers used are null.
 * Big-O notation: O(n)
 */
void destroySet(SET *sp)
{
        assert(sp != NULL);
        int i;
        //Goes through every element in the array data and frees the memory of each element. Memory of sp is freed afterwards since sp is needed to access the array data.  
        for(i = 0; i < sp->count; i++)
        {
        	free(sp->data[i]);  
        }
        free(sp);                                                                               
}

/*Finds and returns the number of elements currently stored in the set by accessing the variable count. Assert is used to make sure none of the pointers used are null. 
 *Big-O notation: O(1)
 */
int numElements(SET *sp)
{
        assert(sp != NULL);
        return sp->count;
}

/*Adds an inputted string elt into the array data of set sp, in the correct alphabetical order. Will prevent elt from being added to the array if the array data is already holding its maximum number of elements, or if the value of elt is already present in the array. Assert is used to make sure none of the pointers used are null.
 *Big-O notation: O(n)
 */
void addElement(SET *sp, char *elt)
{
        assert(sp != NULL);
        assert(elt != NULL);
        //checks to see if the set is already holding the maximum number of strings
        assert(sp->count <= sp->length);
	
	bool found;
	int elementIndex = search(sp, elt, &found);
	//checks to see if elt is already in the set
        if(found)
        {
		return;
	}
	
	int i;
	//shifts every value between the end of the array and element index so that the new element can be put into the array correctly
	for(i = sp->count; i >= elementIndex; i--)
	{	
		sp->data[i] = sp->data[i-1];
	}
	//Adds elt to the array data by using strdup at correct alphabetical order location elementIndex. 
	sp->data[elementIndex] = strdup(elt);
     	assert(sp->data[elementIndex] != NULL);
	sp->count++;
}


/*Removes inputted string elt from the set sp. Makes sure that elt exists in the set first before removing. Assert is used to make sure none of the pointers used are null.  
 *Big-O notation: O(n)
 */
void removeElement(SET *sp, char *elt)
{
        assert(sp != NULL);
        assert(elt != NULL);
        bool found;
	int elementIndex = search(sp, elt, &found);
        //checks to see if elt is in the set, ending the funciton if it is not in the set
        if(!found)
        {
        	return;
        }
	
	//frees the memory of the element at elementIndex to prevent a memory leak
	free(sp->data[elementIndex]);
	//starting at elementIndex, sets each value in the array equal to the value ahead of it
	int i;
	for(i = elementIndex; i < sp->count-1; i++)
	{
		sp->data[i] = sp->data[i+1];	
	}
	sp->count--;
}

/*Finds inputted string elt in set sp. Uses search function to find the matching string, and then returns that string. 
 *Big-O notation: O(log(n))
 */
char *findElement(SET *sp, char *elt)
{
        assert(sp != NULL);
        assert(elt != NULL);
	bool found; 
        int elementIndex = search(sp, elt, &found);
        //checks if string exists in the set, and returns null if string isn't found in the set
        return found ? sp->data[elementIndex] : NULL;
}

/*Creates a copy of array data and returns the new array. All values in data are the same in the newData array.
 *Big-O notation: O(n)
 */
char **getElements(SET *sp)
{
        assert(sp != NULL);
        //allocates memory to the new array and copies every value from the array data into the new array newData on the next line
        char **newData = malloc(sizeof(char*)*sp->count);
	assert(newData != NULL);
        memcpy(newData, sp->data, sizeof(char*)*sp->count);
	return newData;
}
/*Searches the array data in the set sp for a given element elt. Returns the index elt is located at, or the index where it should be located in the array based off of its alphabetical ordering. Uses a boolean value passed in by reference to indicate whether the returned value is the index of the element, or the index of where the element should be located. Uses binary search since array is sorted.  
 *Big-O: O(log(n))
 */
static int search (SET *sp, char *elt, bool *found)
{
	assert(sp != NULL);
	assert(elt != NULL);
	assert(found != NULL);
	int low = 0;
	int high = sp->count-1;
	int mid = 0;	
	
	//loop used to conduct binary search
	while(low <= high)
	{
		mid = (high+low)/2;
		if(strcmp(sp->data[mid],elt) == 0)
		{
			*found = true;
			return mid;
		}
		else if(strcmp(sp->data[mid],elt) <  0)
		{
			low = mid + 1;			
		}
		else
		{
			high = mid - 1;
		}		
	}
		
	*found = false;
	//After the loop, the location of where elt should be must be greater than high, but less than the previous value of low. Therefore, the location of where elt would be located in the array is equal to the new value of low (mid+1), 	      so low is returned.     
	return low;
}
