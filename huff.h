#define HUFF_H

typedef struct chartree
{
	char character;
	int count;
	chartree * left;
	chartree * right;
} chartree;



void countfreq(chartree
