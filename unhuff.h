#define UNHUFF_H

typedef struct chartree
{
	char character;
	int charcount;
	struct chartree * left;
	struct chartree * right;
	struct chartree * next;
} chartree;

chartree * makell(chartree *, int);
chartree * makehufftree(chartree *);
void insertnode(chartree *, chartree **);
void decompress(FILE *, chartree *, char *);
//void makebinary(char *, int);



