/*Andrew Vattuone, count.c, 9/20/23
 *User needs to input a file name on compile time. Then opens the
 *file, scans each word and the file, and adds 1 to wordCount whenever a word 
 *is scanned. wordCount is then displayed at the end of the program, which 
 *represents the number of words in the text, and the program ends.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 30
/*
 * Driver function. Runs the program. 
 */
int main(int argc, char *argv[])
{
	//returns an error and ends the program if the user doesn't enter a file name
	if(argc <= 1)
	{
		printf("Error. The file name is missing. Please select an existing file!\n");
		return 0;
	}
	
	FILE *fptr;
	fptr = fopen(argv[1], "r");

	//returns an error and ends the program if the file wasn't successfully opened 
	if(fptr == NULL)
	{
		printf("Error. File could not be opened successfully. Please try again.\n");
		return 0;
	}
	int wordCount = 0;
	char word[MAX_WORD_LENGTH];
	//scans each word in the text and adds 1 to wordCount until the end of  the file
	while(fscanf(fptr, "%s", word) == 1)
	{
		wordCount++;
	}

	printf("%d total words\n", wordCount);
	fclose(fptr);
	return 0;
}
