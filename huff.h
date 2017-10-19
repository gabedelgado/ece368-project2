#define HUFF_H

typedef struct chartree
{
	char character;
	int charcount;
	struct chartree * left;
	struct chartree * right;
	struct chartree * next;
} chartree;


void charfreq(chartree*, char*);
chartree * makell(chartree*);
//chartree * addnode( 

