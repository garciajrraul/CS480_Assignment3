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

typedef struct PageTable{
	struct Levels *root; /*MAYBE, Pointer to root node level*/

	unsigned int levelCount; 
	uint32_t *bitMaskArr;
	unsigned int *shiftArr;
	unsigned int *entryCount;
}PageTable;

typedef struct Level{
	struct Pagetable *root; /*MAYBE, Pointer to root of pagetable*/

	unsigned int currentDepth;
	struct Levels *nextLevel; /*pointer for next level*/
	struct Map *map;/*pointer for map entries*/
}Level;

typedef struct Map{

}Map;


struct pageTable *getPageTable(unsigned int levels, unsigned int *levelSizes);
unsigned int virtualAddressToPageNum (unsigned int virtualAddress, unsigned int mask, unsigned int shift);
void pageInsert(PageTable *pagetable, unsigned int virtualAddress, unsigned int frame);
Map * pageLookup(PageTable *pageTable, unsigned int virtualAddress);

#endif