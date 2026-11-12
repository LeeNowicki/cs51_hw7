/*
 * cache.c
 *
 * Aleksander Nowicki
 * aleksander.l.nowicki.27@dartmouth.edu
 *
 * Modified from io.c, with credits below.
 *
 * File has been modified to simultate the operation of a cache, 
 * with the different methods possible through 
 *
 * Peter C. Johnson
 * pete@cs.dartmouth.edu
 *
 * updated by sws
 *
 * A simple program to demonstrate file I/O in C.  Reads in a file of lines
 * consisting of a single character, a space, and a 32-bit address in hex.
 * File is specified as only
 * command-line argument; if no argument is provided or argument is "-", reads
 * from stdin.
 *

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>



typedef struct Line {
    bool valid;
    uint32_t tag;
    int lastUse;
    } line;

    
int cacheRead( unsigned int addr, struct Line simCache, int s);
    
int main(int argc, char *argv[])
{
    char *filename = "-";
    FILE *fh;
    unsigned int n;  // this will contain the number read
    char c;          // this will contain the char

    char* type = "1";
    char* verbose_arg = "0";

    bool verbose = false;

    if(argc > 2){
        filename = argv[1];
        type = argv[2];
    }
    else{
        printf("Usage: ./cache filename, caching type, verbose\n \
               filename: string filename\n \
                caching type: 1-4, 1 direct, 2 2-way, 3 4-way, 4 fully associative\n \
                verbose: 1 if verbose, else defaults to not");
        exit(1);
    }

    if (argc >3){
        verbose_arg = argv[2];
    }

    if (strcmp(verbose_arg, "1") == 0){
        verbose = true;
    }

    //set type of caching
    //Hardcode the mask for each of the four types
    int setSize = 1;
    uint32_t tagMask = 0xfffffc00;
    uint32_t setMask = 0x000003f0;
    if(strcmp(type, "1") == 0){
        setSize = 1;
        tagMask = 0xfffffc00;
        setMask = 0x000003f0;
    }
    if(strcmp(type, "2") == 0){
        setSize = 2;
        tagMask = 0xfffffe00;
        setMask = 0x000001f0;
    }
    if(strcmp(type, "3") == 0){
        setSize = 4;
        tagMask = 0xffffff00;
        setMask = 0x000000f0;
    }
    if(strcmp(type, "4") == 0){
        setSize = 64;
        tagMask = 0xfffffff0;
        setMask = 0x00000000;
    }

    if(strcmp(filename, "-") == 0) {
        fh = stdin;
    } else {
        fh = fopen(filename, "r");
        if(!fh) {
            perror("fopen");
            exit(1);
        }
    }

    //Make the cache
    struct Line cache[64] = {0};

    //define some variables for the loop
    int last_touch = 1;
    int least_touch = 1;
    int oldest_index = 0;
    uint32_t setNum = 0;
    uint32_t tag = 0;
    line currLine;

    int hitNum = 0;
    int missNum = 0;
    bool found = false;

    while(fscanf(fh, "%c %x\n", &c, &n) != EOF) {
      
      // ok, now we have the line from the trace file.  
        if(verbose){
      printf("address %08x, ", n);
      switch (c) {
      case 'I':
	printf("instruction fetch\n");
	break;
      case 'D':
	printf("data\n");
	break;
      default:
	printf("??\n");
	break;
      }
        }


        //Now do the cache reading

        //Determine the set number
        setNum = n & setMask;
        setNum = setNum >> 4;

        if(verbose)
            printf("set number: %x\n", setNum);

        //Determine tag
        tag = n & tagMask;
        //leave unshifted for simplicity - else need to define offset for
        //each kind of caching?

        if (verbose) printf("tag: %x\n", tag);

        least_touch = last_touch;
        oldest_index = setNum*setSize;

        found = false;

        for (int i = setNum*setSize; i<setNum*setSize + setSize; i++){
            currLine = cache[i];

            if (verbose) printf("State of line %x in set %x: valid: %x, tag: %x, lastUse: %x\n", 
                   i-setNum, setNum, currLine.valid, currLine.tag, currLine.lastUse);

            if(currLine.valid && tag == currLine.tag){
                // Have a hit - increment the hit counter, update the last use, and break
                hitNum++;
                cache[i].lastUse = last_touch;
                found = true;
                if (verbose) printf("Hit!\n");
                break;
            }

            //Not in this line, keep looking through set
            
            if (least_touch > currLine.lastUse){
                least_touch = currLine.lastUse;
                oldest_index = i;
            }
        }

        if(!found){
            //Not found in the set, update the miss counter and overwrite the least used in the set
            if (verbose) printf("Miss!\n");
            missNum++;

            cache[oldest_index].tag = tag;
            cache[oldest_index].valid = true;
            cache[oldest_index].lastUse = last_touch;

            currLine = cache[oldest_index];

            if (verbose) printf("New state of line %x, contained in set %x: valid: %x, tag: %x, lastUse: %x\n", 
                   oldest_index, setNum, currLine.valid, currLine.tag, currLine.lastUse);
        }

        last_touch++;
    }

    //Now the report

    printf("Run had %d hits and %d misses. Hit percentage is %f\n", hitNum, missNum, (float)hitNum/(hitNum+missNum));

    fclose(fh);
}
