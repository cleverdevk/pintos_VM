#include <stdio.h>
#include <syscall.h>
#include "threads/malloc.h"

int
main (int argc, char **argv)
{
  int i;
  printf("Hello pINTOS\n");
  for (i = 0; i < argc; i++)
    printf ("%s hello", argv[i]);
  printf ("\n");
/*
  for(i =0; i<10000;i++){
	  malloc(400);
	  printf("malloc....");
  }
*/

  return EXIT_SUCCESS;
}
