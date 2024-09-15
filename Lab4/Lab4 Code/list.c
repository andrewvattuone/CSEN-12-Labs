/*Andrew Vattuone, list.c, 10/25/23
 *Various functions used to manipulate elements in a circular doubly-linked list. Functions include adding elements to either the start or end of the list, removing elements from the start or end of the list, returning the first and last nodes, removing an item in the list, finding a specific item in the list, and creating an array with the same data values as the nodes in the list and returning that array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

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

/*Creates the list, allocating the appropriate amount of memory and initializing the variables to their appropriate values. A dummy node is created that points to itself. A separate compare function needs to be passed into this function so that the data of the list can be properly compared. 
 * Big-O notation: O(1)
 */
LIST *createList(int (*compare)())
{
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->count = 0;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	//Creation of dummy node. head->next points to the first node in the list, head-prev points to the last node in the list. Both are initially set to point to head.
	lp->head->data = NULL;
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	lp->compare = compare;
	return lp;
} 

/*Frees memory allocated to every node in the list lp, and then frees memory allocated to lp itself. 
 * Big-O notation: O(n)   where n is equal to the number of elements in the list
 */
void destroyList(LIST *lp)
{
	assert(lp != NULL);
	NODE *curr = lp->head->next;
	while(curr != lp->head)
	{
		NODE *pDel = curr;
		curr = curr->next;
		free(pDel);
	}
	free(lp);
}

/*Returns the number of items stored in the list lp.
 *Big-O notation: O(1);
 */
int numItems(LIST *lp)
{
	assert(lp != NULL);
	return lp->count;
}

/*Creates a node for an inputted item and adds it to the first spot in the list. 
 *Big-O notation: O(1)
 */
void addFirst(LIST *lp, void *item)
{
	assert(lp != NULL);
	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);
	newNode->data = item;
	newNode->next = lp->head->next;
	newNode->prev = lp->head;
	//Updates the pointer to the first node and the pointer of the last node to the next node (which will point to the first node) to the correct values (pointer to newNode) after inserting the new node into the list. 
	lp->head->next->prev = newNode;
	lp->head->next = newNode;
	lp->count++;
}

/*Creates a node for an inputted item and adds it to the last spot in the list.
 *Big-O notation: O(1)
 */
void addLast(LIST *lp, void *item)
{
	assert(lp != NULL);
	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);
	newNode->data = item;
	newNode->next = lp->head;
	newNode->prev = lp->head->prev;
	//Updates the pointer to the last node and the pointer of the first node to the previous node (which will point to the last node) to the correct values (pointer to newNode) after inserting the new node into the list.
	lp->head->prev->next = newNode;
	lp->head->prev = newNode;
	lp->count++;
}	

/*Removes the first node in the list and returns the value of the data that was stored in that node.
 * Big-O notation: O(1)
 */
void *removeFirst(LIST *lp)
{
	assert(lp != NULL);
	//Updates the prev pointer of the second node in the list to point to head, as this node will be the new first node.
	lp->head->next->next->prev = lp->head;
	NODE *temp = lp->head->next;
	void *item = lp->head->next->data;
	//Updates the head->next pointer to point to the new first node in the list
	lp->head->next = lp->head->next->next;
	free(temp);
	lp->count--;
	return item;		
}

/*Removes the last node in the list and returns the value of the data that was stored in that node.
 *Big-O notation: O(1)
 */
void *removeLast(LIST *lp)
{
	assert(lp != NULL);
	//Updates the next pointer of the second to last node in the list to point to head, as this node will be the new last node.
	lp->head->prev->prev->next = lp->head;
	NODE *temp = lp->head->prev;
	void *item = lp->head->prev->data;
	//Updates the head->prev pointer to point to the new last node in the list.
	lp->head->prev = lp->head->prev->prev;
	free(temp);
	lp->count--;
	return item;
}

/*Returns the value of the data held in the first node of the list.
 *Big-O notation: O(1)
 */
void *getFirst(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->next->data;
}

/*Returns the value of the data held in the last node of the list.
 *Big-O notation: O(1)
 */
void *getLast(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->prev->data;
}

/*Removes a node holding the value of an inputted item from the list lp. 
 *Big-O notation: O(n)   where n is the number of nodes in the list
 */
void removeItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	assert(lp->compare != NULL);
	NODE *curr = lp->head->next;
	while(curr != lp->head)
	{
		if((*lp->compare)(curr->data, item) == 0)
		{
			//sets pointers that originaly pointed to the node that will be removed to the correct values so that the linked list functions properly
			curr->next->prev = curr->prev;
			curr->prev->next = curr->next;
			free(curr);
			lp->count--;
			return;
		}
		curr = curr->next;
	}	
}

/*Finds and returns an inputted item in the list lp. Returns NULL if item is not found in the list.
 *Big-O notation: O(n)    where n is the number of nodes in the list
 */
void *findItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	assert(lp->compare != NULL);
	NODE *curr = lp->head->next;
	while(curr != lp->head)
	{
		if((*lp->compare)(curr->data, item) == 0)
		{
			return curr->data;		
		}
		curr = curr->next;	
	}
	return NULL;
}

/*Creates and allocates memory to an array called newArray. Sets each value of newArray equal to the corresponding value stored in the nodes of the list lp, and then returns newArray.
 *Big-O notation: O(n)    where n is the number of nodes in the list
 */ 
void *getItems(LIST *lp)
{
	assert(lp != NULL);
	void **newArray = malloc(sizeof(void *)*lp->count);
	assert(newArray != NULL);
	NODE *curr = lp->head->next;
	int i;
	for(i = 0; i < lp->count; i++)
	{
		newArray[i] = curr->data;
		curr = curr->next;	
	}
	return newArray;
} 
