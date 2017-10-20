#define HUFF_H

typedef struct chartree
{
	char character;
	int charcount;
	struct chartree * left;
	struct chartree * right;
	struct chartree * next;
} chartree;

typedef struct hufftable
{
	char character;
	char code[256];
} hufftable;

void charfreq(chartree*, char*);
chartree * makell(chartree*);
//chartree * addnode(
chartree * makehufftree(chartree *);
chartree * insertnode(chartree *, chartree *);
void maketable(chartree *, hufftable *, char *, int);

 

