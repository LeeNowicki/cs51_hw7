#include <stdio.h>
#include <stdlib.h>

unsigned long fun(unsigned long);

int main(int argc, char *argv[]) {

  unsigned long in, out;

  if (argc < 2) {
    printf("requires an unsigned decimal argument\n");
    exit(-1);
  }

  in = strtoul(argv[1], NULL, 10);

  printf("fun(%lu) ...\n", in);
  out = fun(in);

  printf("...returns %lu\n", out);

  return 0;



}

