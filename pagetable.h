/*
Name: 	Raul Garcia Jr
		Cat-Tuong Tu
#RedID: 819138431
		822204923
#Class: CS480-01
#Assignemt 3
*/

#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef struct PageTable
{
	struct Level *rootLevel; /*Pointer to root node level*/
	unsigned int *bitMaskArr;
	unsigned int *shiftArr;
	unsigned int *entryCount;
	unsigned int levelCount;
	// unsigned int currentFrame;

} PageTable;

typedef struct Level
{
	struct PageTable *rootPageTable; /*Pointer to root of pagetable*/
	unsigned int currentDepth;		 /*Saves Depth of level*/
	struct Level **nextLevel;		 /*pointer for next level*/
	struct Map **map;				 /*pointer for map entries*/

} Level;

typedef struct Map
{
	bool isValid;		/* Boolean flag denoting whether the page is valid */
	unsigned int frame; /* Physical frame number */
} Map;

struct PageTable *getPageTable(unsigned int levels, unsigned int *levelSizes);
struct Level *getLevel(PageTable *pagetable, unsigned int currentDepth);
struct Map **getMap(PageTable *pg, unsigned int depth);
unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
void pageInsert(PageTable *pagetable, unsigned int virtualAddress, unsigned int frame);
void pageInsertForLevel(Level *levelPtr, unsigned int virtualAddress, unsigned int frame);
Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress);

#endif