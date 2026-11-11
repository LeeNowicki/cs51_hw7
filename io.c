/*
 * io.c
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

int main(int argc, char *argv[])
{
    char *filename = "-";
    FILE *fh;
    unsigned int n;  // this will contain the number read
    char c;          // this will contain the char

    if(argc > 1)
        filename = argv[1];

    if(strcmp(filename, "-") == 0) {
        fh = stdin;
    } else {
        fh = fopen(filename, "r");
        if(!fh) {
            perror("fopen");
            exit(1);
        }
    }

    while(fscanf(fh, "%c %x\n", &c, &n) != EOF) {
      
      // ok, now we have the line from the trace file.  
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

    fclose(fh);
}

