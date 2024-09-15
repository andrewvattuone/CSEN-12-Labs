/*Andrew Vattuone, table.c, 10/18/23
 *Various functions that are used to manipulate an array of void pointers within a structure. The array within the structure can hold any data type of variables. The data is held in hash table within a set structure along with variables that track the number of elements within the array and the maximum number of elements the array can store. Data is inputted into the hash table using a hash function, and an array of characters called flags is used to indicate which slots are filled, which slots are empty, and which slots had a value deleted from them. A compare function and a hash function are both passed in to the set so that the set can compare values of any data type as well as place values at the correct location in the table.   
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

static int search(SET *sp, void *elt, bool *found);

//Definition of the structure set
typedef struct set
{
	void **data;
	int length;
	int count;
	char *flags;
	int (*compare)();
	unsigned (*hash)();
}SET;

/*Creates the set, taking in an argument maxElts that represents the maximum number of words that can be stored in the array data. The set and array data are both allocated the appropriate amount of memory, length is set to maxElts, and count is set to 0. Flags is also allocated the appropriate amount of memory, and a for loop is used to set each value in flags equal to EMPTY. compare and hash are both separate functions that need to be passed in to create set so that elements can be properly compared and assigned using hashing. Assert is used to make sure that none of the pointers used are null. 
Big-O notation: O(n)    where n = maxElts   (max length of the array data)
*/ 
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	SET *setptr = malloc(sizeof(SET));
	assert(setptr != NULL);
	setptr->length = maxElts;
	setptr->data = malloc(sizeof(void*)*maxElts);
	assert(setptr->data != NULL);
	setptr->count = 0;
	setptr->flags = malloc(sizeof(char)*maxElts);
	assert(setptr->flags != NULL);
	assert(compare != NULL && hash != NULL);
	setptr->compare = compare;
	setptr->hash = hash;
	int i; 
	for(i = 0; i < setptr->length; i++)
	{
		setptr->flags[i] = EMPTY;
	}
	return setptr;
}

/*Deallocates the memory given to inputted set sp, first freeing the memory allocated to data and flags and then the memory allocated to the set itself. Doesn't free each element in data this time since it would be too complicated with the various types of pointers. Assert is used to make sure none of the pointers used are null.
 * Big-O notation: O(1)
 */
void destroySet(SET *sp)
{
	assert(sp != NULL);  
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

/*Adds an inputted element elt of any data type into the array data of set sp. Will prevent elt from being added to the array if the array data is already holding its maximum number of elements, or if the value of elt is already present in the array. Assert is used to make sure none of the pointers used are null.
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
void addElement(SET *sp, void *elt)
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
	sp->data[location] = elt;
	sp->flags[location] = FILLED;
	sp->count++;
}

/*Removes inputted element elt from the set sp. Makes sure that elt exists in the set first before removing. Assert is used to make sure none of the pointers used are null.  
 *Big-O notation: O(n)   where n = maxElts (max length of the array data)
 */
void removeElement(SET *sp, void *elt)
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
	
	//Memory isn't deallocated since memory wasn't allocated by any of the functions in table.c to a specific element in the array data. The element's corresponding index in flag is set to deleted so that other functions still know that this value was deleted.
	sp->flags[location] = DELETED; 
	sp->count--; 
}

/*Finds inputted element elt in set sp. Uses search function to find the matching element, and then returns that element. Returns null if element isn't found. Assert is used to make sure no pointers passed in to the method are null. 
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
void *findElement(SET *sp, void *elt)
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

/*Creates a copy of only the filled elements in data and returns this new array. Assert is used to make sure no pointers passed into the method are null and make sure memory is allocated correclty.
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
void *getElements(SET *sp)
{
	assert(sp != NULL);
	//allocates memory to the new array and copies every value from the old array into the new array on the next line
	void **newData = malloc(sizeof(void *)*sp->count);
	assert(newData != NULL);
	int dataIndex;
	int newDataIndex = 0;
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

/*Searches the set sp for a given element elt using sequential search. If the element is found, its index is returned, and if not, the index of where it should be inputted is returned. A boolean is passed by reference into this function, which indicates whether the index being returned is the index where the element is located or the index where the element should be placed in the array. Assert is used to make sure no pointers passed in to method are null. 
 *Big-O notation: O(n)   where n = maxElts  (max length of the array data)
 */
static int search(SET *sp, void *elt, bool *found)
{
	assert(sp != NULL);
	assert(elt != NULL);
	assert(found != NULL);
	int start = (*sp->hash)(elt) % sp->length;
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
		else if(sp->flags[location] == FILLED && (*sp->compare)(sp->data[location], elt) == 0)
		{
			*found = true;
			return location;
		} 
	} 

	*found = false;
	return deleteIndex; 
}
