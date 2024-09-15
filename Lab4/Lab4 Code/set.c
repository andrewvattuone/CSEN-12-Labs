/*Andrew Vattuone, set.c, 11/1/23
 *Various functions that are used to manipulate a set of any data type. The structure holds an array of lists, with each list containing nodes that hold the data values. Each sublist is a circular, doubly-linked list with a dummy node that points to the head and tail of the list. Elements are added to the set by using a hash function to determine what list they belong in, and then are added to the start of the list. List elements must be unique and no copies are allowed. A compare function and a hash function are both passed in to the set so that the set can compare values of any data type as well as hash elements to the correct list.   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "list.h" 
#include <stdbool.h>


#define ALPHA 20

//Need these structure definitions so that they can be properly used later in the code
typedef struct node
{
        void *data; 
        struct node *next;
        struct node *prev;
}NODE;


typedef struct list
{
	int count;
	struct node *head;
	int (*compare)();
}LIST;

typedef struct set
{
	LIST **lists;
	int length;
	int count;
	int (*compare)();
	unsigned (*hash)();
}SET;



/*Creates the set, taking in an argument maxElts that represents the maximum number of elements that will be stored in the set. The set is allocated the appropriate amount of memory and length, which represents the number of lists in the  st, is initialized to maxElts/ALPHA, where ALPHA is a constant equal to 20. compare and hash are both separate functions that need to be passed in to create set so that elements can be properly compared and assigned using hashing. Count is set to 0 and a number of lists equal to length are created, which will later be helpful in resolving hashing collisions. Assert is used to make sure that none of the pointers used are null. 
Big-O notation: O(m)    where m is the maximum number of elements in the set (m is divided by 20 in this case though)
*/ 
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->length = maxElts/ALPHA;
	sp->count = 0;
	assert(compare != NULL && hash != NULL);
	sp->compare = compare;
	sp->hash = hash;
	sp->lists = malloc(sizeof(LIST*)*sp->length);
	assert(sp->lists != NULL);
	int i; 
	for(i = 0; i < sp->length; i++)
	{
		sp->lists[i] = createList(compare);
	}
	return sp;
}

/*Deallocates the memory given to inputted set sp, first freeing the memory allocated to each list and then the memory allocated to the set itself. Doesn't free each element in data this time since it would be too complicated with the various types of pointers. Assert is used to make sure none of the pointers used are null.
 * Big-O notation: O(m)       where m is the maximum number of elements in the set
 */
void destroySet(SET *sp)
{
	assert(sp != NULL);
	int i;
	for(i = 0; i < sp->length; i++)
	{
		destroyList(sp->lists[i]);
	}   
	free(sp->lists); 
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

/*Adds an inputted element elt of any data type into the array data of set sp. Will prevent elt from being added to the array if the value of elt is already present in the array. Always adds elements at the start of the array since order doesn't matter. Assert is used to make sure none of the pointers used are null.
 *Big-O notation: O(n)     where n is the number of elements in the list sp->lists[location]
 */
void addElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	//uses hashing to find the list in the set that the element would be stored in or is stored in
	int location = (*sp->hash)(elt) % sp->length;	
	//checks to see if elt is already in the list, and adds elt to the list if it isn't found
	if(!findItem(sp->lists[location], elt))
	{
		addFirst(sp->lists[location], elt);
		sp->count++;	
	}
}

/*Removes inputted element elt from the set sp. Makes sure that elt exists in the set first before removing. Assert is used to make sure none of the pointers used are null.  
 *Big-O notation: O(n)   where n is the number of elements in the list sp->lists[location]
 */
void removeElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	//uses hashing to find the list the element is located in
	int location = (*sp->hash)(elt) % sp->length;
	//checks to see if elt is in the list, and removes the node containing elt if it is found
	if(findItem(sp->lists[location], elt))
	{
		removeItem(sp->lists[location], elt);
		sp->count--;
	}
}

/*Finds inputted element elt in set sp. Uses hashing to find the matching element, and then returns that element. Assert is used to make sure no pointers passed in to the method are null. 
 *Big-O notation: O(n)   where n is the number of elements in the list sp->lists[location]
 */
void *findElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	//finds the array elt is located in or would be located in in the set sp
	int location = (*sp->hash)(elt) % sp->length;
	//returns the element if found, and NULL if not found
	return findItem(sp->lists[location], elt);		
}

/*Creates an array and copies every value in the set into the array. Then returns the array. Assert is used to make sure no pointers passed into the method are null and make sure memory is allocated correclty.
 *Big-O notation: O(m)   where m is the maximum number of elements in the array (although this value is divided by 20)
 */
void *getElements(SET *sp)
{
	assert(sp != NULL);
	void **newData = malloc(sizeof(void *)*sp->count);
	assert(newData != NULL);
	//need two indexes in order to copy each value in each list into the array correctly	
	int i;
	int j = 0;
	for(i = 0; i < sp->length; i++)
	{
		memcpy(newData + j, getItems(sp->lists[i]), sizeof(void *)*numItems(sp->lists[i]));
		j += numItems(sp->lists[i]);
	}
	
	return newData;	
}
