/*
Name: 	Raul Garcia Jr
    	Cat-Tuong Tu
#RedID: 819138431
		822204923
#Class: CS480-01
#Assignemt 3
*/

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "tracereader.h"
#include "output_mode_helpers.h"

#define MODEBUFFER 10
#define INDEXOFTRACEFILE 1

int main(int argc, char **argv)
{
    FILE *ifp;	        /* trace file */
    unsigned long i = 0;  /* instructions processed */
    p2AddrTr trace;	/* traced address */

    int numberOfMemoryAccesses;
    int maxNumberOfPageMapping;
    int numberOfLevels; /*Number of levels for command line arguments*/
    int idx;    /*Index for command line arguments*/

    OutputOptionsType output = {.bitmasks = false, .offset = false, 
                                .summary = false, .v2p_tlb_pt = false, 
                                .virtual2physical = false, .vpn2pfn = false};

    //Argument checking
    int option;
    while((option = getopt(argc, argv, "n:c:o:")) != -1){
        switch(option){
            case 'n':
                numberOfMemoryAccesses = atoi(optarg);
                break;
            case 'c':
                maxNumberOfPageMapping = atoi(optarg);
                break;
            case 'o':
                char mode[MODEBUFFER] = atoi(optarg);
                if(mode == 'bitmaks'){output.bitmasks = true;}
                else if(mode == 'virtual2physical'){output.virtual2physical = true;}
                else if(mode == 'v2p_tlb_pt'){output.v2p_tlb_pt = true;}
                else if(mode == 'vpn2pfn'){output.vpn2pfn = true;}
                else if(mode == 'offset'){output.offset = true;}
                else if(mode == 'summary'){output.summary = true;}
                else{}
                break;
            case 'default':
                /*NOT SURE YET*/
                break;
        }
    }

    /* first mandatory argument, optind is defined by getopt */
    idx = optind;

    int levels = argc - INDEXOFTRACEFILE; /*levels holds the amount of levels from command line*/
    int levelSizes[levels]; /*Array that holds level bit sizes*/
    int i;
    if(idx < argc){ /*Checks for mandatory arguments*/
        /* attempt to open trace file */
        if((ifp = fopen(argv[idx],"rb")) == NULL) {
            fprintf(stderr,"Unable open <<%s>>\n",argv[INDEXOFTRACEFILE]); /*If can not open throws error and exits*/
            exit(EXIT_FAILURE);
        }
        /*Assigning bit totals to levels array from command line*/
        for(i = idx; idx < argc; idx++){
            if(argv[i] < 1){/*Checks to see if the page table is at least 1*/
                fprintf(stderr, "Level 0 page table must be at least 1 bit\n");
                exit(EXIT_FAILURE);
            }
            if(argv[i] > 28){/*Checks to see if page table isn't over*/
                fprintf(stderr, "Too many bits used in page tables\n");
                exit(EXIT_FAILURE);
            }
            //levelSizes[];
        }
    }
    else{/*No Mandatory arguments*/
        fprintf(stderr, "Not enough arguments\n"); /*Prints error and exits*/
        exit(EXIT_FAILURE);
    }


    /*while (!feof(ifp)) {
        //get next address and process
        if (NextAddress(ifp, &trace)) {
            AddressDecoder(&trace, stdout);
            i++;
            if ((i % 100000) == 0)fprintf(stderr,"%dK samples processed\r", i/100000);
        }
    }*/

  /* clean up and return success */
  fclose(ifp);
  return (0);
}