/*
Name: 	Raul Garcia Jr
    	Cat-Tuong Tu
#RedID: 819138431
		822204923
#Class: CS480-01
#Assignemt 3
*/

//Adding all for now cause don't know which will be used
#include "main.h"
#include "tracereader.h"
#include "output_mode_helpers.h"
#include "pagetable.h"
#include "tlb.h"

int main(int argc, char **argv)
{
    FILE *ifp;	        /* trace file */
    //unsigned long i = 0;  /* instructions processed */
    p2AddrTr trace;	/* traced address */

    int numberOfMemoryAccesses;
    int maxNumberOfPageMapping;
    int numberOfLevels; /*Number of levels for command line arguments*/
    int idx;    /*Index for command line arguments*/

    /*Decleration of OptionType Struct*/
    OutputOptionsType output = {.bitmasks = false, .offset = false, 
                                .summary = false, .v2p_tlb_pt = false, 
                                .virtual2physical = false, .vpn2pfn = false};
    struct PageTable *pg; /*Decleration of PageTable Struct*/

    /*Optional Argument checking*/
    int option;
    while((option = getopt(argc, argv, "n:c:o:")) != -1){
        switch(option){
            case 'n':
                numberOfMemoryAccesses = atoi(optarg);
                break;
            case 'c':
                maxNumberOfPageMapping = atoi(optarg);
                break;
            case 'o': /*Setting the output mode*/
                if(strcmp("bitmask", optarg) == 0){output.bitmasks = true;}
                else if(strcmp("virtual2physical", optarg) == 0){output.virtual2physical = true;}
                else if(strcmp("v2p_tlb_pt", optarg) == 0){output.v2p_tlb_pt = true;}
                else if(strcmp("vpn2pfn", optarg) == 0){output.vpn2pfn = true;}
                else if(strcmp("offset", optarg) == 0){output.offset = true;}
                else {output.summary = true;} /*Default output mode*/
                break;
            default:
                break;
        }
    }

    idx = optind; /* First mandatory argument index, optind is defined by getopt */

    /* attempt to open trace file */
    if((ifp = fopen(argv[idx],"rb")) == NULL) {
        fprintf(stderr,"Unable open <<%s>>\n",argv[idx]); /*If can not open throws error and exits*/
        exit(EXIT_FAILURE);
    }
    idx++;  /*IDX increased after opening tracefile*/

    unsigned int levels = argc - idx; /*Amount of levels from command line stored in PageTable struct*/
    unsigned int levelSizes[levels]; /*Array that holds level bit sizes*/

    int i, j;
    if(idx < argc){ /*Checks for mandatory arguments*/
        j = 0; //Index for the levels array
        /*Assigning bit totals to levels array from command line*/
        for(i = idx; i < argc; i++){
            if(atoi(argv[i]) < 1){/*Checks to see if the page table is at least 1*/
                fprintf(stderr, "Level 0 page table must be at least 1 bit\n");
                exit(EXIT_FAILURE);
            }
            if(atoi(argv[i]) > 28){/*Checks to see if page table isn't over 28*/
                fprintf(stderr, "Too many bits used in page tables\n");
                exit(EXIT_FAILURE);
            }
            levelSizes[j]= atoi(argv[i]); //Assignment of Entrycount to page Table
            j++;
        }
    }

    pg = getPageTable(levels, levelSizes); //PageTable Struct Initilization

    /* Reding of file*/
    while (!feof(ifp)) {
        //get next address and process
        if (NextAddress(ifp, &trace)) {
            AddressDecoder(&trace, stdout);
            i++;
            if ((i % 100000) == 0)fprintf(stderr,"%dK samples processed\r", i/100000);
        }
    }


  /* clean up and return success */
  fclose(ifp);
  return (0);
}