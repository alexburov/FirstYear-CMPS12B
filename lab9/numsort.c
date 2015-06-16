// $Id: numsort.c,v 1.12 2015-03-11 19:34:50-07 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include <stdlib.h>
#include <stdio.h>
#include "inssort.h"

// Double compare function
int double_cmp(const void* this,const void* that) {
   double a = *(double*)this;
   double b = *(double*)that;
   return (a > b) - (a < b);
}

int main() {
   double array[0x1000];
   // nelem keeps track of the number of elements and indexes.
   int nelem = 0;
   while (scanf("%lf",&array[nelem]) && nelem < 0x1000) {
      nelem++;
   }
   // Sort it.
   inssort(array, nelem, sizeof *array, double_cmp);
   // Print it in the specified format.
   for (int index = 0; index < nelem; ++index) {
      printf("%20.15g\n", array[index]);
   }
}
