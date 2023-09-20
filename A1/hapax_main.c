#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "word_tally.h"
#include "word_extractor.h"
#include "LLNode.h"
#include <ctype.h>

/** function to be passed in to llFree to delete allocated keys */
void deleteKey(LLNode *node, void *userdata)
{
	if (node->key != NULL) {
		free(node->key);
	}
}


/** Delete the contents of all of the word lists
 *
 * This function assumes that both the nodes and
 * the keys within the nodes have been allocated
 * using malloc() and need to be freed.
 */
void deleteWordLists(LLNode **wordListHeads, int maxLen)
{
	int i;

	for (i = 0; i < maxLen; i++) {
		if (wordListHeads[i] != NULL) {
			llFree(wordListHeads[i], deleteKey, NULL);
			wordListHeads[i] = NULL;
		}
	}
}


/** print out all of the data in a word list */
int printData(char *filename, LLNode *wordListHeads[], int maxLen)
{
	LLNode *node;
	int i;

	printf("All word count data from file '%s':\n", filename);

	/**
	 * For each length, if the list is not null, print out
	 * the values in the list
	 */
	for (i = 0; i <= maxLen; i++) {
		node = wordListHeads[i];
		if (node != NULL) {
			printf("Length %d:\n", i);
			while (node != NULL) {
				printf("    '%s' %d\n", node->key, node->value);
                node = node->next;
			}
		}
	}
	return 1;
}

/** TODO: print out only the hapax legomena in a word list */
int printHapax(char *filename, LLNode *wordListHeads[],
		int maxLen, int hapaxLength)
{
	
	LLNode *node;
	// Add your code here
	if (hapaxLength == -1){
		for (int i = 0; i < maxLen; i++){
		node =  wordListHeads[i];

			while(node!= NULL){
				if (node->value == 1){
					printf("\t%s\n", node->key);
				}
				node = node->next;

			}
		
	}
	}else{
		node = wordListHeads[hapaxLength];
			while(node!= NULL){
				if (node->value == 1){
					printf("\t%s\n", node->key);
				}
				node = node->next;

			}
		
		
	}
	
	

	//use tally 
	return 1;
}

/* print out the command line help */
void
usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Find and print the hapax legomena in one or more files.\n");
	fprintf(stderr, "A \"hapax legomenon\" is a word that occurs only once in the file\n");

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    hapax [<options>] <datafile> [ <datafile> ...]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "-d     : print out all data loaded before printing hapax legomena.\n");
	fprintf(stderr, "-h     : this help.  You are looking at it.\n");
	fprintf(stderr, "-l <N> : only print hapax legomena of length <N>.\n");
	fprintf(stderr, "       : If no -l option is given, all hapax legomena are printed.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Sample command line:\n");
	fprintf(stderr, "    hapax -l5 smalldata.txt");
	fprintf(stderr, "\n");
	fprintf(stderr, "This example would print all words of length 5 that exist in the\n");
	fprintf(stderr, "file \"smalldata.txt\".\n");
	fprintf(stderr, "\n");

	// exit the program
	exit(1);
}


/**
 * Program mainline
 */

// define the maximum length of word we will look for, and by extension,
// the number of entries that must be in the array of word lists
#define	MAX_WORD_LEN	24

int main(int argc, char **argv)
{
	int i, shouldPrintData = 0, didProcessing = 0, printHapaxLength = -1;
	char N[100];

	/** TODO: allocate an array of list heads of the required size */
	LLNode *wordListHeads[MAX_WORD_LEN + 1];

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			// TODO: Put your code to process the command line options here
			if (strcmp(argv[i], "-d") == 0) {
				shouldPrintData = 1;
				printHapaxLength = -1;
			}
			else if (strcmp(argv[i], "-l") == 0){
				if (i + 1 < argc) {
					strcpy(N, argv[i + 1]);
					if (isdigit(N[0])) {
						printHapaxLength = atoi(N);
						i++; // Move to the next argument (length) since we've processed it
					} else {
						fprintf(stderr, "Error: Invalid length argument after '-l'\n");
						usage();
						return 1;
					}
				} else {
					fprintf(stderr, "Error: Missing length argument after '-l'\n");
					usage();
					return 1;
				}
			}
			else if (strcmp(argv[i], "-h") == 0){
				usage();
				return 0;
			}
			else{
				printf("command does not exist.\n");
				return 0;
			}



        } else {
//			// Once you have set up your array of word lists, you
//			// should be able to pass them into this function

			//tally words in file initalizes the heads so data doesn't interfere
			if (tallyWordsInFile(argv[i], wordListHeads, MAX_WORD_LEN) == 0) {
				fprintf(stderr, "Error: Processing '%s' failed -- exitting\n",
						argv[i]);
				return 1;
			}
			printf("Tally Loaded:\n");
			
		

			//continue if passed 
	
			didProcessing = 1;

			/**
			 * conditionally print out all the words loaded, based
			 * on the command line option
			 */

			if (shouldPrintData) {
				// this should also work once you have allocated the
				// array of lists properly
				printData(argv[i], wordListHeads, MAX_WORD_LEN);
			}
			printf("Hapax Legomena from file: %s\n",argv[i]);
			/** print out all the hapax legomena that we have found */
			printHapax(argv[i], wordListHeads, MAX_WORD_LEN, printHapaxLength);

			// TODO: clean up any memory that we have allocated in this loop
			llFree(*wordListHeads,deleteKey,NULL);
			
		}
	}

	if ( ! didProcessing ) {
		fprintf(stderr, "No data processed -- provide the name of"
				" a file on the command line\n");
		usage();
		return 1;
	}

	// TODO: clean up any remaining memory that we have allocated
	deleteWordLists(wordListHeads,MAX_WORD_LEN);


	return 0;
}

