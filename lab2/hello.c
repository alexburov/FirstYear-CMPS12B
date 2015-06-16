// $Id: hello.c,v 1.5 2015-01-17 13:29:29-08 - - $

//
// NAME
//    hello - print the "Hello, World!" message.
//
// SYNOPSIS
//    hello
//
// DESCRIPTION
//    Prints the "Hello, World!" message if no operands are
//    present.  Errors out with a Usage message otherwise.
//

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
   int exit_status = EXIT_SUCCESS;
   if (argc == 1) {
      printf ("Hello, World!\n");
   } else {
      fprintf(stderr,"Usage: %s\n",basename (argv[0]));
      exit_status = EXIT_FAILURE;
   }
   return exit_status;
}
