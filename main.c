/*
Name: 	Raul Garcia Jr
        Cat-Tuong Tu
#RedID: 819138431
        822204923
#Class: CS480-01
#Assignemt 3
*/

// Includes all header files to be used in the main
#include "main.h"
#include "tracereader.h"
#include "output_mode_helpers.h"
#include "pagetable.h"
#include "tlb.h"

int main(int argc, char **argv)
{
    FILE *ifp; /* trace file */
    // unsigned long i = 0;  /* instructions processed */
    p2AddrTr trace; /* traced address */

    int numberOfMemoryAccesses; //Number of memory addrresses to be accessed
    int maxNumberOfPageMapping; //Number of mapings for TLB

    unsigned int numberOfLevels; /*Number of levels for command line arguments*/
    int idx;                     /*Index for command line arguments*/

    /*Decleration of OptionType Struct and sets all to false until one is set to true by -o*/
    OutputOptionsType output = {.bitmasks = false, .offset = false, .summary = false, .v2p_tlb_pt = false, .virtual2physical = false, .vpn2pfn = false};
    struct PageTable *pg = (struct PageTable *)malloc(sizeof(struct PageTable)); /*Decleration of PageTable Struct*/

    /*Optional Argument checking*/
    int option;

    while ((option = getopt(argc, argv, "n:c:o:")) != -1)
    {
        switch (option)
        {
        case 'n':
            numberOfMemoryAccesses = atoi(optarg);
            break;
        case 'c':
            maxNumberOfPageMapping = atoi(optarg);
            if(maxNumberOfPageMapping <= 0){
                fprintf(stderr, "Cache capacity must be a number, greater than or equal to 0\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'o': /*Setting the output mode*/
            if (strcmp("bitmasks", optarg) == 0)
            {
                output.bitmasks = true;
            }
            else if (strcmp("virtual2physical", optarg) == 0)
            {
                output.virtual2physical = true;
            }
            else if (strcmp("v2p_tlb_pt", optarg) == 0)
            {
                output.v2p_tlb_pt = true;
            }
            else if (strcmp("vpn2pfn", optarg) == 0)
            {
                output.vpn2pfn = true;
            }
            else if (strcmp("offset", optarg) == 0)
            {
                output.offset = true;
            }
            else
            {
                output.summary = true;
            } /*Default output mode*/
            break;
        default:
            break;
        }
    }

    idx = optind; /* First mandatory argument index, optind is defined by getopt */

    /* Attempt to open trace file */
    if ((ifp = fopen(argv[idx], "rb")) == NULL)
    {
        fprintf(stderr, "Unable open <<%s>>\n", argv[idx]); /*If can not open throws error and exits*/
        exit(EXIT_FAILURE);
    }
    idx++; /*IDX increased after opening tracefile*/

    unsigned int levels = argc - idx; /*Amount of levels from command line stored in PageTable struct*/
    unsigned int levelSizes[levels];  /*Array that holds level bit sizes*/

    int i, j;
    int total = 0; //Holds the total for the bits in the levels
    if (idx < argc) /*Checks for mandatory arguments*/
    {
        j = 0; // Index for the levels array
        /*Assigning bit totals to levels array from command line*/
        for (i = idx; i < argc; i++)
        {
            if (atoi(argv[i]) < 1)
            { /*Checks to see if the page table is at least 1*/
                fprintf(stderr, "Level %d page table must be at least 1 bit\n", idx-1);
                exit(EXIT_FAILURE);
            }
            total += atoi(argv[i]);
            levelSizes[j] = atoi(argv[i]); // Assignment of Entrycount to page Table
            j++;
        }
        if (total > 28) /*Checks to see if page table isn't over 28*/
        {
            fprintf(stderr, "Too many bits used in page tables\n");
            exit(EXIT_FAILURE);
        }
    }

    unsigned int currentFrame = 0; //Variable to keep count of the frames
    pg = getPageTable(levels, levelSizes); // PageTable Struct Initilization
    int limit = 0; //Variable to keep track of how many addresses are read

    //Prints BitMask
    if (output.bitmasks)
    {
        report_bitmasks(levels,pg->bitMaskArr);
    }
    else{
        /* Reading of file*/
        while (!feof(ifp))
        {
            // get next address and process
            // BYUADDRESSTRACE is stored in 'trace' variable
            if (NextAddress(ifp, &trace))
            {
                //printf("Address: %0x%x\n", trace.addr);
                // Gets address, don't have to swap endian it is already swapped to little endian
                long unsigned int addr = trace.addr;

                Map *mappingFound = pageLookup(pg, addr);
                if(mappingFound == NULL){
                    pageInsert(pg, addr, currentFrame);
                    ++currentFrame;
                }
                //Prints Offset
                if(output.offset){
                    hexnum(virtualToOffset(pg, addr));
                }

                //Prints Virtual Address
                if(output.vpn2pfn){
                    //report_pagemap(pg->levelCount,  ,currentFrame);
                }

                // TODO: Search the TLB Cache
                //
                //
                // TODO: If VPN->PFN mapping is found in TLB, use frame number for translation
                //       and TLB Cache Hits + 1
                //
                //
                // TODO: If TLB is a miss, use the pagetable to find the VPN->PFN mapping
                //
                //
                //
                // TODO: If VPN->PFN mapping is in pagetable, insert it in the TLB Cache
                //       and PageTable Hits + 1
                //       If the cache is full, apply cache replacement using approx LRU policy
                //
                //
                // TODO: If VPN->PFN mapping is NOT in pagetable, insert page to page table,
                //       insert to TLB cache, and apply cache replacement if TLB is full
                //
                //
            }
            //Checks to see if number of entries are equal to numberOfMemoryAccesses
            if (limit == numberOfMemoryAccesses && numberOfMemoryAccesses != NULL)
            {
                break;
            }
            limit++; //Increases the address read
        }

        if(output.summary){
        }
    }
    
    /* clean up and return success */
    fclose(ifp);
    return (0);
}