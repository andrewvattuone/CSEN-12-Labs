/*Andrew Vattuone, list.c, 11/7/23
 *Various functions used to manipulate a linked list of arrays of any data type. As items are added to the list, the size of each new node is doubled using a variable stored in the list to allow for the list to not have a fixed size and be traversable using indices (also saves time on memory allocation). Functions include deleting the list, adding elements to the front of the list, adding elements to the end of the list, removing elements from the front of the list, removing elements from the end of the list, returning the item at a given index, and changing an item at a given index. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

#define MULTIPLIER 2
#define INITIAL_LENGTH 10

typedef struct node
{
	void **data;
	struct node *next;
	struct node *prev;
	int first;
	int count;
	int length;
}NODE;

typedef struct list
{
	int count;
	struct node *head;
	int currLength;
}LIST;

/*Creates a list lp and initializes all of its values. Count stores the number of elements stored within the list (the total number of elements stored in all of the arrays, not the number of nodes) and currLength is initialized to 10, which represents what the size of the first array will be. Also stores a dummy node called head, with lp->head->next representing the first node in the list and lp->head->prev representing the last node in the list. 
 *Big-O: O(1)
 */ 
LIST *createList(void)
{
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->count = 0;
	//lp->currLength used to store the length that newly created nodes will have, initially set to 10
	lp->currLength = INITIAL_LENGTH;
	//creation of dummy node
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	lp->head->data = NULL;
	//pointer used to point to the first node
	lp->head->next = lp->head;
	//pointer used to point to the last node
	lp->head->prev = lp->head;
	return lp;		
}

/*Destroys every node within the list lp. When deleting each node, the memory is first deallocated from the array first, and then the node itself. After the memory for each node had been deallocated, the memory of the list itself is deallocated.
 *Big-O: O(n^2)   where n is the total number of elements in the list
 */ 
void destroyList(LIST *lp)
{
	assert(lp != NULL);
	NODE *curr = lp->head->next;
	while(curr != lp->head)
	{
		free(lp->head->data);
		NODE *temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(lp);
}

/*Returns the total number of elements stored within the list lp (this includes every element stored in each element and each node, and isn't just the number of nodes in the list).
 *Big-O: O(1)
 */ 
int numItems(LIST *lp)
{
	assert(lp != NULL);
	return lp->count;
}

/*Adds an element to the first slot in the array located within the first node. If the array of the first node is full or if there are no nodes in the list, then a new node is created with the element being inserted as its first element. The variable first is always updated so that the first node in the list is correctly recorded.
 *Big-O: O(1)
 */ 
void addFirst(LIST *lp, void *item)
{
	assert(lp != NULL);
	//checks to see if there are no nodes in the list or if the current node is empty, and if so a new node is created
	if(lp->count == 0 || lp->head->next->count == lp->head->next->length)
	{
		NODE *newNode = malloc(sizeof(NODE));
		assert(newNode != NULL);
		newNode->next = lp->head->next;
		newNode->prev = lp->head;
		newNode->count = 0;
		newNode->first = 0;
		newNode->length = lp->currLength;
		//doubles lp->currLength to make memory allocated less often, which helps runtime when many elements are inputted into the list
		lp->currLength *= MULTIPLIER;
		newNode->data = malloc(sizeof(void *)*newNode->length);
		assert(newNode->data != NULL);
		lp->head->next->prev = newNode;
		lp->head->next = newNode;
	}
	//updates the location of the first element of the first node
	lp->head->next->first = (lp->head->next->first + lp->head->next->length - 1) % lp->head->next->length;
	//adds the new item to the array
	lp->head->next->data[lp->head->next->first] = item;
	lp->head->next->count++; 
	lp->count++;
}

/*Adds an element to the last slot in the array within the last node. If the array in the last node is full or if there are no nodes in the list, then a new node is created and the element is inserted into it. 
 *Big-O: O(1)
 */ 
void addLast(LIST *lp, void *item)
{
	assert(lp != NULL);
	//checks to see if there are no nodes in the list or if the current node is empty, and if so a new node is created
	if(lp->count == 0 || lp->head->prev->count == lp->head->prev->length)
 	{
 		NODE *newNode = malloc(sizeof(NODE));
 		assert(newNode != NULL);
		newNode->next = lp->head;
		newNode->prev = lp->head->prev;
		newNode->count = 0;
		newNode->first = 0;
		newNode->length = lp->currLength;
		//doubles lp->currLength to make memory allocated less often, which helps runtime when many elements are inputted into the list
		lp->currLength *= MULTIPLIER;
 		newNode->data = malloc(sizeof(void *)*newNode->length);
 		assert(newNode->data != NULL);
 		lp->head->prev->next = newNode;
 		lp->head->prev = newNode;
	}
	//adds the new item to the array
	lp->head->prev->data[(lp->head->prev->first + lp->head->prev->count) % lp->head->prev->length] = item;
	lp->head->prev->count++;
	lp->count++; 		
}

/*Removes the first element in the array in the first node of the list. First checks to see if the first node is empty, and if so it deallocates all the memory from the first node and makes the node after the first node the new first node. After this it removes the first element by increasing the location of first by 1 and decreases the count of both the node and the list by 1. Then returns the value that was removed.
 *Big-O: O(1)
 */ 
void *removeFirst(LIST *lp)
{
	assert(lp != NULL);
	//Checks to see if the first node is empty and if so, removes it and makes the next node the new first node
	if(lp->head->next->count == 0)
	{
		free(lp->head->next->data);
		NODE *temp = lp->head->next;
		lp->head->next->next->prev = lp->head;
		lp->head->next = lp->head->next->next;
		free(temp);		
	}
	assert(lp->count > 0);
	void *item = lp->head->next->data[lp->head->next->first];
	//removes first item from the array by changing values of first and count to make it inaccessible
	lp->head->next->first = (lp->head->next->first + 1) % lp->head->next->length;
	lp->head->next->count--;
	lp->count--;
	return item;
}

/*Removes the last element in the array in the last node of the list. First checks to see if the last node is empty, and if so it deallocates all the memory from the last node and makes the node before the last node the new last node. After this it removes the last element in the array by decreasing the count of both the node and the list by 1. Then returns the value that was removed.
 *Big-O: O(1)
 */ 
void *removeLast(LIST *lp)
{
	assert(lp != NULL);
	//checks to see if the last node is empty and if so, removes it and makes the previous node the new last node
	if(lp->head->prev->count == 0)
	{
		free(lp->head->prev->data);
		NODE *temp = lp->head->prev;
		lp->head->prev->prev->next = lp->head;
		lp->head->prev = lp->head->prev->prev;
		free(temp);
	}
	assert(lp->count > 0);
	void *item = lp->head->prev->data[lp->head->prev->first + lp->head->prev->count];
	//removes last item from the array by changing values of count to make it inaccessible
	lp->head->prev->count--;
	lp->count--;
	return item;
}

/*Finds and returns the item located at the inputted index in the list. The value of the index location of an item is equal to the location the item would be at if the list was one large array instead of several arrays split up between nodes. Inputted index must be within the range of the list. 
 *Big-O: O(n^2)   where n is the total number of elements in the list
 */ 
void *getItem(LIST *lp, int index)
{
	assert(lp != NULL);
	assert(index < lp->count);
	NODE *curr = lp->head->next;
	while(index > curr->count)
	{
		index -= curr->count;
		curr = curr->next;
	}
	//at this point the index must be within the range of the array of the current node
	return curr->data[(curr->first + index) % curr->length];		
}

/*Find an element located at the inputted index in the list and changes the value of that element to the value of the inputted item. The value of the index location of an item is equal to the location the item would be at if the list was one large array instead of several arrays split up between nodes. Inputted index must be within the range of the list.
 *Big-O: O(n^2)  where n is the total number of elements in the list
 */ 
void setItem(LIST *lp, int index, void *item)
{
	assert(lp != NULL);
	assert(index < lp->count);
	NODE *curr = lp->head->next;
	while(index > curr->count)
	{
		index -= curr->count;
		curr = curr->next;
	}
	//value at index is changed to be equal to item
	curr->data[(curr->first + index) % curr->length] = item;
}
