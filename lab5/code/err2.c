// $Id: err2.c,v 1.5 2014-04-23 21:19:13-07 - - $

#include <stdio.h>
#include <string.h>
int main (void) {
   char *p = 20;
   int c = strcmp (p, "foo");
   printf ("%d\n", c);
   return 0;
}
