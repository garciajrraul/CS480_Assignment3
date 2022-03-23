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
#include "tracereader.h"

struct PageTable *getPageTable(unsigned int levels, unsigned int *levelSizes)
{
	struct PageTable *PT = (struct PageTable *)malloc(sizeof(struct PageTable));
	PT->levelCount = levels;
	PT->bitMaskArr = malloc(sizeof(unsigned int)*levels);
	PT->entryCount = malloc(sizeof(unsigned int)*levels);
	PT->shiftArr = malloc(sizeof(unsigned int)*levels);
	
	unsigned int shiftOffset = BITSIZE;
	int i;
	/*Assigning to Shift Array and Entry count*/
	for (i = 0; i < levels; i++)
	{
		PT->shiftArr[i] = shiftOffset - levelSizes[i]; /*Initalizes shift array from offset*/
		PT->entryCount[i] = pow(2, levelSizes[i]);	 /*Initalizes entrycount array for level*/
		shiftOffset = shiftOffset - levelSizes[i];		 /*Removes level bit size from the Offset*/
		//printf("Shift getPageTable: %d\n", PT->shiftArr[i]);
		//printf("Entry getPageTable: %d\n", PT->entryCount[i]);
	}

	/*Bitmask Array Assignment*/
	unsigned int shiftBits = BITSIZE;
	int bits;
	unsigned int bitmask;
	for (i = 0; i < levels; i++)
	{
		bits = levelSizes[i];
		bitmask = (unsigned int)(1 << bits) - 1;
		PT->bitMaskArr[i] = swap_endian(bitmask << shiftBits); //Maybe Swap_endian?*/
		shiftBits += levelSizes[i];
		//printf("BIT OG: %08x\n", PT->bitMaskArr[i]); /*For testing purposes*/
	}

	PT->rootLevel = getLevel(PT, 0);
	return PT;
}

struct Level *getLevel(PageTable *pagetable, unsigned int depth)
{
	// Instantiate new level 0
	struct Level *levelZero = (struct Level *)malloc(sizeof(struct Level));

	// Set Level 0 to point back to pagetable
	levelZero->rootPageTable = pagetable;

	// Set current depth to depth parameter should be zero
	levelZero->currentDepth = depth;

	if (pagetable->levelCount == 1)
	{
		levelZero->map = getMap(pagetable, depth);
	}
	else
	{
		//printf("Creating inner level node...\n");
		levelZero->nextLevel = (struct Level *)malloc(sizeof(struct Level)*pagetable->entryCount[depth]);
		//printf("Array of struct created\n");
		int i = 0;
		for (i = 0; i < pagetable->entryCount[depth]; i++)
		{
			levelZero->nextLevel = NULL; //Maybe initalize to NULL
		}
	}
	return levelZero;
}

struct Map * getMap(PageTable *pg, unsigned int depth){
	struct Map * m = (struct Map*)malloc(sizeof(struct Map) * pg->entryCount[depth]);
	int i;
	for(i = 0; i < pg->entryCount[depth]; i++){
		m[i].isValid = false;
	}
	return m;
}

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
	printf("Virtual Address: AT GET PAGE: %08x\n", virtualAddress);
	printf("Mask AT GET PAGE: %08x\n", mask);
	printf("Shift AT GET PAGE: %d\n", shift);
	return (virtualAddress & mask) >> shift;
}

void pageInsert(struct PageTable *pg, unsigned int virtualAddress, unsigned int frame)
{
	pageInsertForLevel(pg->rootLevel, virtualAddress, frame);
}

void pageInsertForLevel(Level *levelPtr, unsigned int virtualAddress, unsigned int frame) /*ISSUE WITH PAGETABLE of nextLevel pointing to original pagetable*/
{
	unsigned int depth = levelPtr->currentDepth;
	// Find index into the current page level WORKING PROPERLY!
	unsigned int page = virtualAddressToPageNum(virtualAddress, levelPtr->rootPageTable->bitMaskArr[depth], levelPtr->rootPageTable->shiftArr[depth]);

	printf("PAGE: %d\n", page);
	// If the level is a leaf node
	if (depth  == levelPtr->rootPageTable->levelCount - 1)
	{
		printf("Entry: %d\n", levelPtr->rootPageTable->entryCount[levelPtr->currentDepth]);
		levelPtr->map = getMap(levelPtr->rootPageTable, depth);
		levelPtr->map[page].isValid = true;
		levelPtr->map[page].frame = frame;
		printf("Map inserted at index: %u, frame: %d\n", page, frame);
	}
	else
	{
		printf("Entry: %d\n", levelPtr->rootPageTable->entryCount[levelPtr->currentDepth]);
		// Create new Level and set level to current depth + 1
		struct Level *newLevel = getLevel(levelPtr->rootPageTable, depth + 1);
		pageInsertForLevel(newLevel, virtualAddress, frame);
	}
}

/*Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress)
{
	struct Map *mp = (struct Map*)malloc(sizeof(struct Map));
	int i;
	for(i = 0; i < pageTable->levelCount; i++){
		if(pageTable->rootLevel->nextLevel = NULL){
			mp = NULL;
		}
		unsigned int pg = virtualAddressToPageNum(virtualAddress, pageTable->bitMaskArr[i], pageTable->shiftArr[i]);
		if(pageTable->rootLevel->map){
			
		}
	}

	return mp;
}*/