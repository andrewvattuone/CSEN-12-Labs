/*Andrew Vattuone, huffman.c, 11/15/23
 *Functions used to take an input text file and perform huffman coding to encode it into a binary file, with more common characters being represented with fewer bits and less common characters bieng represented with more bits. The huffman tree is created using a priority queue, which is then passed into a separate function located in pack.c that converts the text to binary digits. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "pack.h"
#include "pqueue.h"
#include <ctype.h>

#define LENGTH 257

static struct node *mknode(int count, struct node *left_node, struct node *right_node);

int cmp(struct node *t1, struct node *t2);

static int depth(struct node *node);

/*Main function. Takes in an input file and output file. Used to find the number of character occurrences in the input file, store that information in an array, and then create an array of nodes that corresponds to each character in the input file. Also creates a priority queue and adds an entry for each node into the queue. The queue is then used to construct a huffman tree. The number of bits required by each character, the number of occurrences of each character, and the total number of bits used by the file is then printed out. At the end of the function, the reconstructed queue and the input and output files are passed in to the function pack, which encodes the data in the input file into binary and puts it into the output file.
 */ 
int main(int argc, char *argv[])
{
	assert(argc == 3);
	
	FILE *input_file_ptr = fopen(argv[1], "rb");
	assert(input_file_ptr != NULL);

	int counts[LENGTH]={0};
	struct node *nodes[LENGTH]={NULL};	
	
	//reads every character in the file and stores the number of times each character occurs in count
	int c;
	do
	{
		c = getc(input_file_ptr);
		counts[c]++;
	}
	while(c != EOF);
	
	PQ *queue = createQueue(cmp);
	//makes a node for every entry with a nonzero count and adds it to the priority queue
	int i;
	for(i = 0; i < LENGTH; i++)
	{
		if(counts[i] != 0)
		{
			nodes[i] = mknode(counts[i], NULL, NULL);
			addEntry(queue, nodes[i]);
		} 
	}
	
	//makes a node for the EOF character and adds this to the priority queue
	nodes[LENGTH-1] = mknode(0, NULL, NULL);
	addEntry(queue, nodes[LENGTH-1]);

	//completely restructures priority queue to turn it into a huffman tree by removing two nodes, making a new node with them, and adding it back to the queue
	while(numEntries(queue) > 1)
	{
		struct node *temp1 = removeEntry(queue);
		struct node *temp2 = removeEntry(queue);
		struct node *newNode = mknode(temp1->count + temp2->count, temp1, temp2);
		addEntry(queue, newNode);		
	}
	
	//prints out every character in the file, the number of times it occurs, the number of bits each individual character uses, and the total number of bits that each type of character uses
	for(i = 0; i < LENGTH; i++)
	{
		if(nodes[i] != NULL)
		{
			if(isprint(i))
			{
				printf("\'%c\'", i);	
			}
			else
			{
				printf("%03o", i);
			}
		
			int height = depth(nodes[i]);
			printf(": %d x %d bits = %d bits\n", counts[i], height, counts[i]*height);
		}
	}
	//encodes input file into a binary file using the huffman tree created in the code
	pack(argv[1], argv[2], nodes);
	fclose(input_file_ptr);
	return 0;		
}

/*Creates a node using count (which represents the number of times a character appears in a file) as well as what the left and right children of the node will be. Also set the parents fo the left and right children to be the new node.
 *Big-O: O(1)
 */ 
static struct node *mknode(int count, struct node *left_node, struct node *right_node)
{
	struct node *new_node = malloc(sizeof(struct node));
	assert(new_node != NULL);
	new_node->count = count;
	new_node->parent = NULL;
	//makes new_node the parent of the left and right children nodes if they exist
	if(left_node != NULL)
	{
		left_node->parent = new_node;
	}
	if(right_node != NULL)
	{
		right_node->parent = new_node;
	}
	return new_node;	
}

/*Custom comparison function used to compare the counts of each character in 2 inputted nodes. Returns 1 if the first inputted node has a higher character count, -1 if the second inputted node has a higher character count, and 0 if both counts are the same.
 *Big-O: O(1)
 */ 
int cmp(struct node *t1, struct node *t2)
{
	assert(t1 != NULL && t2 != NULL);
	return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
}

/*Calculates and returns how many levels away a particular node is from the root (or its depth) using recursion.
 *Big-O: O(log(n)) where n is the number of nodes in the huffman tree
 */ 
static int depth(struct node *node)
{
	assert(node != NULL);
	return (node->parent != NULL) ? 1 + depth(node->parent) : 0;	
}
