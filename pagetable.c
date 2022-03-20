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
		temp->shiftArr[i] = BITSIZE - levelSizes[i]; /*Initalizes shift array from offset*/
		temp->entryCount[i] = pow(2, levelSizes[i]); /*Initalizes entrycount array for level*/
		shiftOffset = shiftOffset - levelSizes[i];	 /*Removes level bit size from the Offset*/
	}

	/*Bitmask Array Assignment*/
	unsigned int shiftBits = BITSIZE;
	int bits;
	unsigned int bitmask;
	for (i = 0; i < levels; i++)
	{
		bits = levelSizes[i];
		bitmask = (unsigned int)(1 << bits) - 1;
		temp->bitMaskArr[i] = bitmask << shiftBits;
		shiftBits += levelSizes[i];
		printf("HEX: %08x\n", temp->bitMaskArr[i]); /*For testing purposes*/
	}

	return temp;
}

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
	pageInsertForLevel(pagetable->root, virtualAddress, frame);
}

void pageInsertForLevel(Level *levelPtr, unsigned int virtualAddress, unsigned int frame)
{
	// Every level has a pointer to the pagetable root node
	PageTable *pagetable = levelPtr->root;
	unsigned int depth = levelPtr->currentDepth;

	// Find index into the current page level
	unsigned int index = virtualToPageNum(virtualAddress, pagetable->bitMaskArr[depth], pagetable->shiftArr[depth]);

	// If the level is a leaf node
	if (levelPtr->currentDepth == pagetable->levelCount - 1)
	{
		// Set the page index to valid and store frame
		Map pageIndex = levelPtr->map[index];
		pageIndex.isValid = true;
		pageIndex.frame = pagetable->currentFrame;
	}
	else
	{
		// Create new Level and set level to current depth + 1
		Level *newLevel = getLevel(pagetable, depth + 1);

		// Amount of entries for new level
		unsigned int entries = pagetable->entryCount[depth + 1];

		// Create an array of Level* or Map* entries based on the number of entries in the new level
		// Initialize each array entry to null if inner node/invalid if leaf node

		// Next level is an inner node
		if (depth + 1 < pagetable->levelCount - 1)
		{
			Level *nextLevelArr = (Level *)malloc(sizeof(Level) * entries);

			// Initialize the next level array to null
			memset(nextLevelArr, '\0', sizeof(nextLevelArr));

			// Set the new level's next level array
			newLevel->nextLevel = nextLevelArr;
		}
		else // Next level is a leaf node
		{
			Map *mapArr = (Map *)malloc(sizeof(Map) * entries);
			int i;
			for (int i = 0; i < entries; i++)
			{
				mapArr[i].isValid = false;
			}
			// Set the new Level's map array
			newLevel->map = mapArr;
		}
		// pageInsertForLevel(pointer to new level, address, frame)
		pageInsertForLevel(newLevel, virtualAddress, frame);
	}
}

Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress)
{
}