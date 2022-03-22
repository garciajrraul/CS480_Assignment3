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
	struct PageTable *temp = (struct PageTable *)malloc(sizeof(struct PageTable));
	unsigned int entry[levels], shift[levels];
	uint32_t bit[levels];

	temp->levelCount = levels;
	temp->bitMaskArr = bit;
	temp->entryCount = entry;
	temp->shiftArr = shift;

	unsigned int shiftOffset = BITSIZE;
	int i;
	/*Assigning to Shift Array and Entry count*/
	for (i = 0; i < levels; i++)
	{
		temp->shiftArr[i] = shiftOffset - levelSizes[i]; /*Initalizes shift array from offset*/
		temp->entryCount[i] = pow(2, levelSizes[i]);	 /*Initalizes entrycount array for level*/
		shiftOffset = shiftOffset - levelSizes[i];		 /*Removes level bit size from the Offset*/
		printf("Entry Count getPageTable: %d\n", temp->entryCount[i]);
	}

	/*Bitmask Array Assignment*/
	unsigned int shiftBits = BITSIZE;
	int bits;
	unsigned int bitmask;
	for (i = 0; i < levels; i++)
	{
		bits = levelSizes[i];
		bitmask = (unsigned int)(1 << bits) - 1;
		temp->bitMaskArr[i] = swap_endian(bitmask << shiftBits); //Maybe Swap_endian?
		shiftBits += levelSizes[i];
	}

	temp->rootLevel = getLevel(temp, 0);
	return temp;
}

struct Level *getLevel(PageTable *pagetable, unsigned int depth)
{
	// Instantiate new level 0
	struct Level *levelZero = (struct Level *)malloc(sizeof(struct Level));
	// Set Level 0 to point back to pagetable
	levelZero->rootPageTable = pagetable;

	// Set current depth to depth parameter should be zero
	levelZero->currentDepth = depth;

	if (depth == pagetable->levelCount - 1)
	{
		printf("Entry Count getLevel: %d\n", pagetable->entryCount[levelZero->currentDepth]);
		levelZero->map = (struct Map *)malloc(sizeof(struct Map)*pagetable->entryCount[levelZero->currentDepth]);
		int i = 0;
		for (i = 0; i < pagetable->entryCount[depth]; i++)
		{
			levelZero->map[i].isValid = false;
		}
		printf("Leaf node has been created\n");
	}
	else
	{
		printf("Creating inner level node...\n");
		levelZero->nextLevel = (struct Level *)malloc(sizeof(struct Level)*pagetable->entryCount[depth]);
		printf("Array of struct created\n");
		int i = 0;
		for (i = 0; i < pagetable->entryCount[depth]; i++)
		{
			printf("Initalizing\n");
			levelZero->nextLevel = NULL;
		}
		printf("Inner level node has been created\n");
	}

	return levelZero;
}

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
	unsigned int page;
	// Bitwise AND the virtual address and mask
	page = virtualAddress & mask;
	// Right shift by offset
	page = page >> shift;
	return page;
}

void pageInsert(struct PageTable *pg, unsigned int virtualAddress, unsigned int frame)
{
	if(pg->rootLevel->currentDepth == pg->levelCount-1){
		printf("Entry Count pageInsert: %d\n", pg->entryCount[pg->rootLevel->currentDepth]);
		printf("Current Depth of Root Level: %d\n", pg->rootLevel->currentDepth);
		unsigned int index = virtualAddressToPageNum(virtualAddress, pg->bitMaskArr[pg->rootLevel->currentDepth], pg->shiftArr[pg->rootLevel->currentDepth]);
		printf("INDEX %d\n", index);
		pg->rootLevel->map[index].frame = frame; //Segmentation fault index huge number
		printf("MADE\n");
		pg->rootLevel->map[index].isValid = true;
		printf("ENDS\n");
	}
	else{
		pageInsertForLevel(pg->rootLevel, virtualAddress, frame);
	}
}

void pageInsertForLevel(Level *levelPtr, unsigned int virtualAddress, unsigned int frame)
{

	// Every level has a pointer to the pagetable root node
	printf("Page insert call\n");
	PageTable *pagetable = levelPtr->nextLevel; //Creates a new pageTable instead of pointing to the pagetable at the beginning
	unsigned int depth = levelPtr->currentDepth; //assigns depth to current depth
	printf("Current depth: %d\n", depth);

	// Find index into the current page level
	unsigned int index = virtualAddressToPageNum(virtualAddress, pagetable->bitMaskArr[depth], pagetable->shiftArr[depth]);
	// If the level is a leaf node
	if (depth == pagetable->levelCount - 1)
	{
		printf("FLAG3\n");
		levelPtr->map = (struct Map*)malloc(sizeof(struct Map) * pagetable->entryCount[depth]);
		// Set the page index to valid and store frame
		int i;
		for(i = 0; i < pagetable->entryCount[depth]; i++){
			levelPtr->map[i].isValid = false;
		}
		levelPtr->map[index].isValid = true;
		levelPtr->map[index].frame = frame;
		// Increment current frame number
		//pagetable->currentFrame++;
		printf("Map inserted at index: %d, frame: %d\n", index, frame);
	}
	else
	{
		printf("FLAG1\n");
		// Create new Level and set level to current depth + 1
		struct Level *newLevel = (struct Level *)malloc(sizeof(struct Level));
		newLevel->currentDepth = depth + 1;
		newLevel ->rootPageTable = pagetable;
		levelPtr->nextLevel = newLevel;
		printf("FLAG2\n");
		pageInsertForLevel(newLevel, virtualAddress, frame);
	}
}

Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress)
{
}