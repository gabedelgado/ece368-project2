#include <stdio.h>
#include <stdlib.h>
#include "huff.h"

int main(int argc, char ** argv){

	printf("starting...\n");	
	chartree tree[256]; 
	int count = 0;
	while(count < 256)
	{
		tree[count].character = (char)count;
		tree[count].charcount = 0;
		count++;
	}

	charfreq(tree, argv[1]);
	
	chartree * root = makell(tree);
	
	/*chartree * surfpointer = root;
	
	while (surfpointer != NULL)
	{
		printf("%c : %d\n", surfpointer->character, surfpointer->charcount);
		surfpointer = surfpointer->next;
	}
	*/
	
	return 0;
}

void charfreq(chartree * tree, char * filename)
{
	FILE * infile = fopen(filename, "r");
			
	char readval = fgetc(infile);

	while(readval != EOF)
	{
		tree[(int)readval].charcount++;
		readval = fgetc(infile);
	}
	
	chartree temp; 
	int checkcheck = 1;
	int lastcheck  = 256;
	int index = 0;	
	
	while((checkcheck == 1) && (lastcheck > 0))
	{		
		checkcheck = 0;
		index = 0;
		while(index < (lastcheck-1))
		{			
			if(tree[index].charcount > tree[index+1].charcount)
			{
				temp = tree[index];
				tree[index] = tree[index+1];
				tree[index+1] = temp;
				checkcheck = 1;
			}
	 		index++;
		}
		lastcheck--;
	}
}

chartree * makell(chartree * tree)
{
	int index = 0;	
	while (tree[index].charcount == 0)
	{
		index++;
	}
	
	chartree * root = malloc(sizeof(chartree));
	
	root = &tree[index];
	
	while(index < 255)
	{
		tree[index].next = &tree[index+1];
		index++;
	}
	
	tree[index].next = NULL;
	return root;
}
	
