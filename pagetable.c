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
	temp->currentFrame = (unsigned int)0; // Initialize frame number counter

	unsigned int shiftOffset = BITSIZE;
	int i;
	/*Assigning to Shift Array and Entry count*/
	for (i = 0; i < levels; i++)
	{
		temp->shiftArr[i] = shiftOffset - levelSizes[i]; /*Initalizes shift array from offset*/
		temp->entryCount[i] = pow(2, levelSizes[i]);	 /*Initalizes entrycount array for level*/
		shiftOffset = shiftOffset - levelSizes[i];		 /*Removes level bit size from the Offset*/
	}

	/*Bitmask Array Assignment*/
	unsigned int shiftBits = BITSIZE;
	int bits;
	unsigned int bitmask;
	for (i = 0; i < levels; i++)
	{
		bits = levelSizes[i];
		bitmask = (unsigned int)(1 << bits) - 1;
		temp->bitMaskArr[i] = swap_endian(bitmask << shiftBits);
		shiftBits += levelSizes[i];
		// printf("HEX Before: %08x\n", temp->bitMaskArr[i]); /*For testing purposes*/
		// printf("HEX After: %08x\n", swap_endian(temp->bitMaskArr[i])); /*For testing purposes*/
	}
	temp->root = getLevel(temp, 0); // 0 is the depth of the first level
	return temp;
}

/*
struct Level *getLevel(PageTable *pagetable, unsigned int currentDepth)
{
	// Instantiate new level
	Level *level = (Level *)malloc(sizeof(Level));

	// Set pagetable node to root
	level->root = pagetable;

	// Set current depth to depth parameter
	level->currentDepth = currentDepth;

	return level;
}
*/

struct Level *getLevel(PageTable *pagetable, unsigned int currentDepth)
{
	// Instantiate new level
	struct Level *level = (struct Level *)malloc(sizeof(struct Level));

	// Set pagetable node to root
	level->root = pagetable;

	// Set current depth to depth parameter
	level->currentDepth = currentDepth;
	printf("Current depth 2: %d\n", level->currentDepth);

	if (pagetable->levelCount == 1 || currentDepth == pagetable->levelCount - 1)
	{
		struct Map *m[pagetable->entryCount[currentDepth]];
		int i = 0;
		for (i = 0; i < pagetable->entryCount[currentDepth]; i++)
		{
			printf("Made it(leaf)\n");
			m[i] = NULL;
		}
		level->map = m;
		printf("Leaf node has been created\n");
	}
	else
	{
		printf("Creating inner level node...\n");
		struct Level *nextL[pagetable->entryCount[currentDepth]];
		int i;
		printf("Entries: %d\n", pagetable->entryCount[currentDepth]);
		for (i = 0; i < pagetable->entryCount[currentDepth]; i++)
		{
			printf("Made it\n");
			nextL[i] = NULL;
		}
		level->nextLevel = nextL;
		printf("Inner level node has been created\n");
	}

	return level;
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
	// Call page insert for level on the root level
	printf("Pagetable frame from function is %u\n", frame);
	printf("Page insert function called with pagetable, %0x%x, frame: %d\n", virtualAddress, frame);
	pageInsertForLevel(pagetable->root, virtualAddress, frame);
}

void pageInsertForLevel(Level *levelPtr, unsigned int virtualAddress, unsigned int frame)
{
	// Every level has a pointer to the pagetable root node
	printf("Page insert call\n");
	PageTable *pagetable = levelPtr->root;
	unsigned int depth = levelPtr->currentDepth;
	printf("Current depth: %d\n", depth);

	// Find index into the current page level
	unsigned int index = virtualToPageNum(virtualAddress, pagetable->bitMaskArr[depth], pagetable->shiftArr[depth]);
	// If the level is a leaf node
	if (depth == pagetable->levelCount - 1)
	{
		// Set the page index to valid and store frame
		Map pageIndex = levelPtr->map[index];
		pageIndex.isValid = true;
		pageIndex.frame = pagetable->currentFrame;
		// Increment current frame number
		pagetable->currentFrame++;
		printf("Map inserted at index: %d, frame: %d\n", index, frame);
	}
	else
	{
		// Create new Level and set level to current depth + 1
		Level *newLevel = getLevel(pagetable, depth + 1);
		// pageInsertForLevel(pointer to new level, address, frame)
		pageInsertForLevel(newLevel, virtualAddress, frame);
	}
}

Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress)
{
}