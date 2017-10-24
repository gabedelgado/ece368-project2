#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "huff.h"

int main(int argc, char ** argv){

		
	chartree tree[256]; 
	int count = 0;
	printf("starting...\n");	
	while(count < 256)
	{
		tree[count].character = (char)count;
		tree[count].charcount = 0;
		count++;
	}
	
	charfreq(tree, argv[1]);
	
	
	
	int index = 0;
	count = 0;	
	while(index < 256)
	{
		if(tree[index].charcount != 0)
		{
			count++;
		}
		index++;	
	}
	
	chartree * root = makell(tree);
	
	/*chartree * surfpointer = root;
	
	while (surfpointer != NULL)
	{
		printf("%c : %d\n", surfpointer->character, surfpointer->charcount);
		surfpointer = surfpointer->next;
	}
	*/
	
	
	hufftable table[count + 1];
	index = 255;	
	int printtablecount = count + 1;
	count = 0;	
		
	while((tree[index].charcount != 0) && (index >= 0))
	{
		table[count].character = tree[index].character;
		count++;
		index--;
	}	
		
		
	chartree * psuedo = malloc(sizeof(chartree));
	
	psuedo->charcount = 0;
	psuedo->left = NULL;
	psuedo->right = NULL;
	psuedo->next = root;
	root = psuedo;
		
		
		
	chartree * treeroot = makehufftree(root);
	char * path = malloc(sizeof(char)*256);
	index = 0;
		
	while(index < 256)
	{
		path[index] = '5';
		index++;
	}
	int pathstep = 0;
		
	maketable(treeroot, table, path, pathstep, printtablecount - 1); 	
	index = 0;	
	
	/*while(index < printtablecount)
	{
		printf("%c : %s\n", table[index].character, table[index].code);
		index++;
	}
	*/
	compress(table, argv[1], tree, printtablecount - 1);

			
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
	fclose(infile);
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
		tree[index].left = NULL;
		tree[index].right = NULL;		
		index++;
	}
	
	tree[index].right = NULL;
	tree[index].left = NULL;
	tree[index].next = NULL;
	return root;
}
	
chartree * makehufftree(chartree * root)
{
	chartree * newnode;		
	while(root->next->next != NULL)
	{
		newnode = malloc(sizeof(chartree));
		newnode->left = root;
		newnode->right = root->next;
		newnode->charcount = root->charcount + root->next->charcount;
		root = root->next->next;
		insertnode(newnode, &root);
	}
	
	newnode = malloc(sizeof(chartree));
	newnode->left = root;
	newnode->right = root->next;
	newnode->charcount = root->charcount + root->next->charcount;
	
	return newnode;
}
	 
void insertnode(chartree * newnode, chartree ** root)
{
	chartree * prevsurfer = *root;
	chartree * surfer = *root;
	int firstit = 0;
	while((surfer != NULL) && (newnode->charcount > surfer->charcount))
	{
		surfer = surfer->next;
		if(firstit == 1)
		{
			prevsurfer = prevsurfer->next;
		}
		firstit = 1;
	}

	if(surfer == NULL)
	{
		prevsurfer->next = newnode;
		newnode->next = NULL;		
	}
	else if (surfer == prevsurfer)
	{
		newnode->next = *root;
		*root = newnode;
	}
	else
	{
		newnode->next = prevsurfer->next;
		prevsurfer->next = newnode;
	}

}

void maketable(chartree * node, hufftable * table, char * currentpath, int pathstep, int lastintable)
{
	int count = 0;
	int index = 0;	
	if(((node->left == NULL) && (node->right == NULL)) || (node->charcount == 0))
	{
		if(node->charcount != 0)
		{
			while(node->character != table[count].character)
			{
				count++;
			}
		}		
		
		if(node->charcount == 0)
		{
			count = lastintable;
		}
		while(index < pathstep)
		{
			table[count].code[index] = currentpath[index];
			index++;
		}
		table[count].code[index] = '5';
		return;
	}
	
	char pathtosend[256];
	
	while(index < pathstep)
	{
		pathtosend[index] = currentpath[index];
		index++;
	}
		
	if(node->left != NULL)
	{
		pathtosend[index] = '0';
		maketable(node->left, table, pathtosend, pathstep + 1, lastintable);
	}	
	
	if(node->right != NULL)
	{	
		pathtosend[index] = '1';		
		maketable(node->right, table, pathtosend, pathstep + 1, lastintable);
	}		
}

void compress(hufftable * table, char * filename, chartree * tree, int lastintable)
{
	char buffer[256];
	int index = 0;
	char outfilename[100];
	FILE * infile = fopen(filename, "r");
	while (filename[index] != '\0')
	{
		outfilename[index] = filename[index];
		index++;
	}
	outfilename[index] = '.';
	index++;	
	outfilename[index] = 'h';
	index++;
	outfilename[index] = 'u';
	index++;
	outfilename[index] = 'f';
	index++;
	outfilename[index] = 'f';
	
	FILE * outfile = fopen(outfilename, "w");
	
	addtreeheader(infile, outfile, tree);
	char readval = fgetc(infile);
	int bufferindex = 0;	
	int codeindex = 0;
	int powerof2 = 7;
	int transferbuffer = 0;
	int chartoint = 0;	
	int chartowrite = 0;
	int clearbuffer = 0;	
	int clearbuffercount = 0;
	while(readval != EOF)
	{
		index = 0;		
		while(table[index].character != readval)
		{
			index++;	
		}
		codeindex = 0;		
		while(table[index].code[codeindex] != '5')
		{
			buffer[bufferindex] = table[index].code[codeindex];
			codeindex++;
			bufferindex++;
		}
		powerof2 = 7;		
		transferbuffer = 0;
		chartowrite = 0;		
		while(bufferindex >= 8)
		{
			powerof2 = 7;			
			transferbuffer = 0;
			chartowrite = 0;
			while(transferbuffer < 8)
			{
				if(buffer[transferbuffer] == '0')
				{	
					chartoint = 0;
				}	
				else
				{
					chartoint = 1;
				}
				chartowrite += chartoint * ((int)(pow(2, powerof2)));
				transferbuffer++;
				powerof2--;
			}				
			fprintf(outfile, "%c", chartowrite);
			clearbuffer = bufferindex - 8;
			clearbuffercount = 0;			
			while(clearbuffercount < clearbuffer)
			{
				buffer[clearbuffercount] = buffer[clearbuffercount + 8];
				clearbuffercount++;
			}
			while(clearbuffercount < bufferindex)
			{
				buffer[clearbuffercount] = '\0';
				clearbuffercount++;
			}
			bufferindex = bufferindex - 8;
		}
		
		readval = fgetc(infile);
			
	}
	index = 0;
	while(table[lastintable].code[index] != '5')
	{
		buffer[bufferindex] = table[lastintable].code[index];
		bufferindex++;
		index++;
	}
	while((bufferindex % 8) != 0)
	{
		buffer[bufferindex] = '0';
		bufferindex++;
	}

	while(bufferindex >= 8)
	{
		powerof2 = 7;			
		transferbuffer = 0;
		chartowrite = 0;
		while(transferbuffer < 8)
		{
			if(buffer[transferbuffer] == '0')
			{	
				chartoint = 0;
			}	
			else
			{
				chartoint = 1;
			}
			chartowrite += chartoint * ((int)(pow(2, powerof2)));
			transferbuffer++;
			powerof2--;
		}				
		fprintf(outfile, "%c", chartowrite);
		clearbuffer = bufferindex - 8;
		clearbuffercount = 0;			
		while(clearbuffercount < clearbuffer)
		{
			buffer[clearbuffercount] = buffer[clearbuffercount + 8];
			clearbuffercount++;
		}
		while(clearbuffercount < bufferindex)
		{
			buffer[clearbuffercount] = '\0';
			clearbuffercount++;
		}
		bufferindex = bufferindex - 8;
	}
	
}	

void addtreeheader(FILE * infile, FILE * outfile, chartree * tree)
{
	int index = 0;
	
	while (tree[index].charcount == 0)
	{
		index++;
	}

	int numchars = 256 - index;
	
	fprintf(outfile, "%d", numchars);
	
	while(index < 256)
	{
		fprintf(outfile, " %c %d", tree[index].character, tree[index].charcount);
		index++;
	}
}	

