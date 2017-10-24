#include <stdio.h>
#include <stdlib.h>	
#include "unhuff.h"

int main(int argc, char ** argv)
{
	FILE * infile = fopen(argv[1], "r");
	int numchars;	
	int index = 1;

	fscanf(infile, "%d", &numchars);	
	
	chartree table[numchars + 1];
	
	while(index <= numchars)
	{
		fgetc(infile);
		table[index].character = fgetc(infile);		
		fgetc(infile);
		fscanf(infile, "%d", &table[index].charcount);
		index++;
	}
	
	table[0].charcount = 0;
	index = 1;
	//printf("EOF : %d\n", table[0].charcount);
	
	//while (index <= numchars)
	//{
//		printf("%c : %d\n", table[index].character, table[index].charcount);
//		index++;
//	}
	
	chartree * head = makell(table, numchars);
	
	chartree * treeroot = makehufftree(head);
	
	decompress(infile, treeroot, argv[1]);
	
	return 0;
}

chartree * makell(chartree * tree, int listsize)
{
	int index = 0;	

	chartree * root = malloc(sizeof(chartree));
	
	root = &tree[index];
	
	while(index < listsize)
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

void decompress(FILE * infile, chartree * treeroot, char * infilename)
{
	int on = 1;
	char readchar;	
	char path[8];
	int pathindex = 0;	
	unsigned int charinint = 0;
	chartree * surfer = treeroot;
	int index = 7;
	char outfilename[100];
	int stringindex = 0;
	while(infilename[stringindex] != '\0')
	{
		outfilename[stringindex] = infilename[stringindex];
		stringindex++;
	}
	
	outfilename[stringindex] = '.';
	stringindex++;
	outfilename[stringindex] = 'u';
	stringindex++;	
	outfilename[stringindex] = 'n';
	stringindex++;
	outfilename[stringindex] = 'h';
	stringindex++;
	outfilename[stringindex] = 'u';
	stringindex++;
	outfilename[stringindex] = 'f';
	stringindex++;
	outfilename[stringindex] = 'f';
	
	FILE * outfile = fopen(outfilename, "w");	
		
	while (on == 1)
	{
		readchar = fgetc(infile);
		//fread(&readchar, 1, 1, infile);
		//makebinary(path, (int)readchar);
		index = 7;
		charinint = (unsigned int)readchar;
		printf("%d :  ", charinint);		
		while(index >= 0)
		{
			if((charinint % 2) == 1)
			{
				path[index] = '1';
			}
			else 
			{
				path[index] = '0';
			}
			charinint = charinint / 2;
			index--;
		}
		index = 0;
		pathindex = 0;
		printf("%s\n", path);	
		while(pathindex < 8)
		{
			if ((surfer->right == NULL) && (surfer->left == NULL))
			{
				if(surfer->charcount == 0)
				{
					on = 0;
					break;
				}
				else
				{
					fprintf(outfile, "%c", surfer->character);
					surfer = treeroot;
				}
			}
			if(path[pathindex] == '1')
			{
				surfer = surfer->right;
			}
			else
			{
				surfer = surfer->left;
			}
			pathindex++;
		}
	}
	fclose(outfile);
}

/*void makebinary(char * binary, int num)
{
	
	int index = 7;
	while(index >= 0)
	{
		if((num % 2) == 1)
		{
			binary[index] = '1'
		}
		else 
		{
			binary[index] = '0'
		}
		binary = binary / 2
		index--;
	}

}*/

