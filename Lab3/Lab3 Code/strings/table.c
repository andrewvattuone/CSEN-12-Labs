/*Andrew Vattuone, table.c, 10/11/23
 *Various functions that are used to manipulate an array of strings within a structure. The words of text files are held in a hash table of strings within a set structure along with variables that track the number of elements within the array and the maximum number of elements the array can store. Strings are inserted into the hash table using a hash function, and an array of characters called flags is used to indicate which slots are filled, which are empty, and which had a value deleted from it.   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include <stdbool.h>

#define EMPTY 0
#define FILLED 1
#define DELETED 2

static int search(SET *sp, char *elt, bool *found);
unsigned strhash(char *s);

//definition of the structure set
typedef struct set
{
	char **data;
	int length;
	int count;
	char *flags;
}SET;

/*Creates the set, taking in an argument maxElts that represents the maximum number of words that can be stored in the array data. The set and array data are both allocated the appropriate amount of memory, length is set to maxElts, and count is set to 0. The flags array is allocated the appropriate amount of memory, and each value is initially set to EMPTY using a for loop since no values have been inserted into the hash table at this time. Assert is used to make sure that none of the pointers used are null. 
Big-O notation: O(n)   where n = maxElts  (max length of the array data)
*/ 
SET *createSet(int maxElts)
{
	SET *setptr = malloc(sizeof(SET));
	assert(setptr != NULL);
	setptr->length = maxElts;
	setptr->data = malloc(sizeof(char*)*maxElts);
	assert(setptr->data != NULL);
	setptr->count = 0;
	setptr->flags = malloc(sizeof(char)*maxElts);
	assert(setptr->flags != NULL);
	int i; 
	for(i = 0; i < setptr->length; i++)
	{
		setptr->flags[i] = EMPTY;
	}
	return setptr;
}

/*Deallocates the memory given to inputted set sp. First deallocates the memory of each filled location in data using a for loop, and then deallocates the memory given to the data array itself. Afterwards frees the flags array and frees the memory of the set sp itself. Assert is used to make sure none of the pointers used are null.
 * Big-O notation: O(n)    where n = maxElts  (max length of the array data)
 */
void destroySet(SET *sp)
{
	assert(sp != NULL);
	int i;
	//Frees memory allocated to each filled element in the array data.   
	for(i = 0; i < sp->length; i++)
	{
		if(sp->flags[i] == FILLED)
		{
			free(sp->data[i]);
		}
	}
	//frees memory allocated to the data array itself, the memory allocated to the flags array, and the memory allocated to the set sp
	free(sp->data);
	free(sp->flags); 
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
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
void addElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	//checks to see if the set is already holding the maximum number of strings
	assert(sp->count < sp->length);
	bool found;
	int location = search(sp, elt, &found);
	//checks to see if elt is already in the set
	if(found)
	{
		return;	
	}
	
	//Adds elt to the array data at the correct location by using strdup. 
	char *copy = strdup(elt);
	assert(copy != NULL);
	sp->data[location] = copy;
	assert(sp->data[location] != NULL);
	sp->flags[location] = FILLED;
	sp->count++;
}

/*Removes inputted string elt from the set sp. Makes sure that elt exists in the set first before removing. Assert is used to make sure none of the pointers used are null.  
 *Big-O notation: O(n)  where n = maxElts  (max length of the array data)
 */
void removeElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	//searches set to see if elt is located within the array data
	bool found;
	int location = search(sp, elt, &found);
	//checks to see if elt is in the set, ending the funciton early if it is not in the set
	if(!found)
	{
		return;
	}
	
	//Frees the memory of the value of data at location and sets the corresponding location in flags equal to deleted.  
	free(sp->data[location]);
	sp->flags[location] = DELETED; 
	sp->count--; 
}

/*Finds inputted string elt in set sp. Uses search function to find the matching string, and then returns that string. Returns null if string isn't found. Assert is used to make sure no pointers passed in to the method are null. 
 *Big-O notation: O(n)    where n = maxElts  (max length of the array data)
 */
char *findElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	assert(elt != NULL);
	bool found;
	int location = search(sp, elt, &found);
	//checks if string exists in the set, and returns null if string isn't found in the set
	if(found)
	{
		return sp->data[location];
	}	
	return NULL; 		
}

/*Creates a copy of array data that only contains the filled values in data and none of the empty or deleted values. A for loop is used to find the filled locations and adds the values of those locations to newData. Assert is used to make sure no pointers passed into the method are null and make sure memory is allocated correclty.
 *Big-O notation: O(n)    where n = maxElts  (max length of the array data)
 */
char **getElements(SET *sp)
{
	assert(sp != NULL);
	//allocates memory to the new array and copies every value from the old array into the new array on the next line
	char **newData = malloc(sizeof(char*)*sp->count);
	assert(newData != NULL);
	int dataIndex;
	int newDataIndex = 0;
	//finds the location of each filled value in data, and adds those values to the array newData
	for(dataIndex = 0; dataIndex < sp->length; dataIndex++)
	{
		if(sp->flags[dataIndex] == FILLED)
		{
			newData[newDataIndex] = sp->data[dataIndex];
			newDataIndex++;		
		}		
	}
	return newData;	
}

/*Searches the set sp for a given string elt using sequential search. Returns the index of elt in the array data if found or the index of where the element should be placed if not found. A boolean called found is passed in by reference which represents if the element has been found or not, which is used to differentiate between wehther the returned value is the index where the element was found or is the index of where the element should be inputted. Assert is used to make sure no pointers passed in to method are null. 
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
static int search(SET *sp, char *elt, bool *found)
{
	assert(sp != NULL);
	assert(elt != NULL);
	assert(found != NULL);
	int start = strhash(elt) % sp->length;
	int location;
	int i;
	bool deleteFound = false;
	int deleteIndex = -1; 
	for(i = 0; i < sp->length; i++)
	{
		location = (start + i) % sp->length;
		if(sp->flags[location] == EMPTY)
		{
			*found = false;
			return deleteFound ? deleteIndex : location;  
		}
		else if(sp->flags[location] == DELETED && !deleteFound)
		{
			deleteFound = true;
			deleteIndex = location;				
		}
		else if(strcmp(sp->data[location], elt) == 0)
		{
			*found = true;
			return location;
		} 
	} 

	*found = false;
	return deleteIndex; 
}

/*Hash function used to calculate where an inputted string s should be placed in the hash table.
 * Big-O notation: O(n)  where n is equal to the number of characters in *s   (max length of the inputted string)
 */
unsigned strhash(char *s)
{
	unsigned hash = 0;
	while (*s != '\0')
		hash = 31*hash + *s ++;
	return hash;
}
