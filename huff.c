#include <stdio.h>
#include <stdlib.h>
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
		
	maketable(treeroot, table, path, pathstep); 	
	index = 0;	
	
	while(index < printtablecount)
	{
		printf("%c : %c\n", table[index].character, table[index].code[0]);
		index++;
	}
			
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
	chartree * temproot = root;	
	while(root->next->next != NULL)
	{
		newnode = malloc(sizeof(chartree));
		newnode->left = root;
		newnode->right = root->next;
		newnode->charcount = root->charcount + root->next->charcount;
		root = root->next->next;
		temproot = insertnode(newnode, root);
		root = temproot;
	}
	
	newnode = malloc(sizeof(chartree));
	newnode->left = root;
	newnode->right = root->next;
	
	return newnode;
}
	 
chartree * insertnode(chartree * newnode, chartree * root)
{
	chartree * prevsurfer = root;
	chartree * surfer = root;
	int firstit = 0;
	while((surfer != NULL) && (newnode->charcount < surfer->charcount))
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
		newnode->next = root;
		root = newnode->next;
	}
	else
	{
		newnode->next = prevsurfer->next;
		prevsurfer->next = newnode;
	}

	return root;	
}

void maketable(chartree * node, hufftable * table, char * currentpath, int pathstep)
{
	int count = 0;
	int index = 0;	
	if((node->left == NULL) && (node->right == NULL))
	{
		while(node->character != table[count].character)
		{
			count++;
		}
				
		while(index < pathstep)
		{
			table[count].code[index] = currentpath[index];
			index++;
		}
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
		maketable(node->left, table, pathtosend, pathstep + 1);
	}	
	if(node->right != NULL)
	{	
		pathtosend[index] = '1';		
		maketable(node->right, table, pathtosend, pathstep + 1);
	}		
}
