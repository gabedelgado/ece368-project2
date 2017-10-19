#define HUFF_H

typedef struct chartree
{
	char character;
	int charcount;
	struct chartree * left;
	struct chartree * right;
} chartree;


void charfreq(chartree*, char*);


