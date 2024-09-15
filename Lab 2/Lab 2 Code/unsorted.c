/*Andrew Vattuone, unsorted.c, 9/27/23
 *Various functions that are used by files unique.c and parity.c to perform several functions that analyze text. Functions that analyze text include finding the number of unique words in a text, removing words from the set if a second text is inputted text and printing out the number of remaining words, listing out these remaining words, and finding the number of words that occur an odd number of times in a text. The words of text files are held in an array of strings within a set structure along with variables that track the number of elements within the array and the maximum nmber of elements the array can store. The array is unsorted in this file. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"

static int search(SET *sp, char *elt);

//definition of the structure set
typedef struct set
{
	char **data;
	int length;
	int count;
}SET;

/*Creates the set, taking in an argument maxElts that represents the maximum number of words that can be stored in the array data. The set and array data are both allocated the appropriate amount of memory, length is set to maxElts, and count is set to 0. Assert is used to make sure that none of the pointers used are null. 
Big-O notation: O(1) */ 
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

/*Deallocates the memory given to inputted set sp, first freeing every memory location in data and then freeing the memory of the set sp itself. Assert is used to make sure none of the pointers used are null.
 * Big-O notation: O(n)
 */
void destroySet(SET *sp)
{
	assert(sp != NULL);
	int i;
	//Frees memory allocated to each element of the array. Memory allocated to the set sp is freed afterwards since sp is needed to free the elements in data.  
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

/*Adds an inputted string elt into the array data of set sp. Will prevent elt from being added to the array if the array data is already holding its maximum number of elements, or if the value of elt is already present in the array. Assert is used to make sure none of the pointers used are null.
 *Big-O notation: O(n)
 */
void addElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	//checks to see if the set is already holding the maximum number of strings
	assert(sp->count <= sp->length);
	
	//checks to see if elt is already in the set
	if(search(sp, elt) != -1)
	{
		return;	
	}
	
	//Adds elt to the array data by using strdup. Elt is always added to the end of the array since order doesn't matter. 
	sp->data[sp->count] = strdup(elt);
	assert(sp->data[sp->count] != NULL);
	sp->count++;
}

/*Removes inputted string elt from the set sp. Makes sure that elt exists in the set first before removing. Assert is used to make sure none of the pointers used are null.  
 *Big-O notation: O(n)
 */
void removeElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	//searches set to see if elt is located within the array data
	int elementIndex = search(sp, elt);
	//checks to see if elt is in the set, ending the funciton early if it is not in the set
	if(elementIndex == -1)
	{
		return;
	}
	
	//Sets the location currently storing elt equal to the final value in the array. Done this way since order doesn't matter in the array. 
	free(sp->data[elementIndex]);
	sp->data[elementIndex] = sp->data[sp->count-1];
	sp->count--; 
}

/*Finds inputted string elt in set sp. Uses search function to find the matching string, and then returns that string. Returns null if string isn't found. Assert is used to make sure no pointers passed in to the method are null. 
 *Big-O notation: O(n)
 */
char *findElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	int elementIndex = search(sp, elt);
	//checks if string exists in the set, and returns null if string isn't found in the set
	if(elementIndex != -1)
	{
		return sp->data[elementIndex];
	}	
	return NULL; 		
}

/*Creates a copy of array data and returns the new array. All values in data are the same in the newData array. Assert is used to make sure no pointers passed into the method are null and make sure memory is allocated correclty.
 *Big-O notation: O(n)
 */
char **getElements(SET *sp)
{
	assert(sp != NULL);
	//allocates memory to the new array and copies every value from the old array into the new array on the next line
	char **newData = malloc(sizeof(char*)*sp->count);
	assert(newData != NULL);
	memcpy(newData, sp->data, sizeof(char*)*sp->count);
	return newData;	
}

/*Searches the set sp for a given string elt using sequential search. Returns the index of elt in the array data if found, returns -1 if not found. Assert is used to make sure no pointers passed in to method are null. 
 *Big-O notation: O(n)
 */
static int search(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	int i;
	//Searches data sequentially to find the index of elt
	for(i = 0; i < sp->count; i++)
	{
		if(strcmp(sp->data[i], elt) == 0)
		{
			return i;			
		} 		
	}
	return -1; 
}
