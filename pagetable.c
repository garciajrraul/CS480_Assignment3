/*
Name: 	Raul Garcia Jr
		Cat-Tuong Tu
#RedID: 819138431
		822204923
#Class: CS480-01
#Assignemt 3
*/

#include "main.h"
#include "pagetable.h"

struct pageTable *getPageTable(unsigned int levels, unsigned int *levelSizes)
{
	struct PageTable *temp = (struct PageTable *)malloc(sizeof(struct PageTable));
	unsigned int entry[levels], shift[levels];
	uint32_t bit[levels];

	temp->levelCount = levels;
	temp->bitMaskArr = bit;
	temp->entryCount = entry;
	temp->shiftArr = shift;

	unsigned int offset = BITSIZE;
	int i;
	for (i = 0; i < levels; i++)
	{
		temp->shiftArr[i] = BITSIZE - levelSizes[i]; /*Initalizes shift array from offset*/
		temp->entryCount[i] = pow(2, levelSizes[i]); /*Initalizes entrycount array for level*/
		offset = offset - levelSizes[i];			 /*Removes level bit size from the Offset*/
	}

	return temp;
}

unsigned int virtualToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
	unsigned int page;
	// Bitwise AND the virtual address and mask
	page = virtualAddress & mask;
	// Right shift by offset
	page = page >> shift;
	return page;
}

void pageInsert(PageTable *pagetable, unsigned int virtualAddress, unsigned int frame)
{
}

Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress)
{
}