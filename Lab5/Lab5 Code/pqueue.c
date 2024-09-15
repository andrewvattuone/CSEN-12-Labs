/*Andrew Vattuone, pqueue.c, 11/6/23
 *Various functions used to manipulate a priority queue. The priority queue contains an array that stores data of generic pointer type. Elements are assigned to a specific location in the array using several calculations to create a min heap in the shape of a binary tree. Any element can be added to the tree, but the only element that can be removed from the tree is the root (upon which the tree will be rearranged to have a new root). The array has a set size but it can be increased as more elements are added.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "pqueue.h"

//functions used to calculate the location of the parent, left child, and right child of a specific element in the heap
#define p(x) (((x)-1)/2)
#define l(x) ((x)*2 + 1)
#define r(x) ((x)*2 + 2)
#define SIZE 10

typedef struct pqueue
{
	int count;
	int length;
	void **data;
	int (*compare)();
}PQ;

/*Creates and initializes appropriate variables in the priority queue pq. Count is set to 0, length is set to a variable called size, and data is allocated the appropriate amount of memory using length. A separate compare function needs to be inputted so that the elements in data can be properly compared with each other. 
 *Big-O: O(1)
 */ 
PQ *createQueue(int (*compare)())
{
	PQ *pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	pq->count = 0;
	pq->length = SIZE;
	pq->data = malloc(sizeof(void*)*pq->length);
	assert(pq->data != NULL);
	assert(compare != NULL);
	pq->compare = compare;
	return pq;
}

/*Frees all the memory in data, and then frees the memory allocated to pq to delete the entire priority queue.
 *Big-O: O(1)
 */ 
void destroyQueue(PQ *pq)
{
	assert(pq != NULL);
	free(pq->data);
	free(pq);				
}

/*Returns the number of elements stored in the priority queue.
 *Big-O: O(1)
 */ 
int numEntries(PQ *pq)
{
	assert(pq != NULL);
	return pq->count;
}

/*Adds element called entry to the priority queue. First checks to see if the array is already full, and if so then doubles the size of the array. Entry is added to the correct sopot in the list using a heap-up function that compares entry with each element above entry to put it in the correct spot in the min heap tree. 
 *Big-O: O(log(n)) where n is the total number of elements in the priority queue
 */ 
void addEntry(PQ *pq, void *entry)
{
	assert(pq != NULL && entry != NULL && pq->compare != NULL);
	
	//doubles the memory allocated to the array if the array is already full
	if(pq->count == pq->length)
	{
		pq->length = pq->length * 2;
		pq->data = realloc(pq->data, sizeof(void*)*pq->length);
		assert(pq->data != NULL);
	}
	
	//Heaps up to put entry in the correct space. Compares entry with its parent, and if the parent is greater than entry, it goes on to the next parent until the parent is less than entry, upon which entry is inserted into the location direclty below the parent and all the other elements are shifted accordingly
	int i = pq->count;
	while(i > 0 && (*pq->compare)(pq->data[p(i)], entry) > 0)
	{
		pq->data[i] = pq->data[p(i)]; 
		i = p(i);		
	}
	pq->data[i] = entry;
	pq->count++;		
}

/*Removes the smallest entry in the tree, which will always be the root since its a min heap. The data is then shifted so that the heap maintains its correct shape. 
 *Big-O: O(log(n)) where n is the total number of elements in the priority queue
 */ 
void *removeEntry(PQ *pq)
{
	assert(pq != NULL && pq->compare != NULL);
	assert(pq->count > 0);
	int i = 0;
	void *min = pq->data[0];
	pq->data[0] = pq->data[--pq->count];
	//If l(i) >= pq->count, then no left child exists in the heap, meaning that the function has reached the bottom layer and should stop
	while(l(i) < pq->count)
	{
		int smallerLoc;
		//checks to see if right child exists
		if(r(i) < pq->count)
		{	
			//compares the left and right child to see which one is smaller
			if((*pq->compare)(pq->data[l(i)], pq->data[r(i)]) < 0)
			{
				smallerLoc = l(i);
			}
			else
			{
				smallerLoc = r(i);	
			}
		}
		else
		{
			smallerLoc = l(i);
		}
		
		//if the current element is smaller than the largest element in the heap, the current element is swapped with its smallest child
		if((*pq->compare)(pq->data[smallerLoc], pq->data[pq->count]) < 0)
		{
			pq->data[i] = pq->data[smallerLoc];
			i = smallerLoc;	
		}
		else
		{
			break;
		}
	}
	//sets the final value in the heap equal to the largest value in the heap
	pq->data[i] = pq->data[pq->count];	
	return min;
}
