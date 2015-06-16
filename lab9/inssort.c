// $Id: inssort.c,v 1.60 2015-03-20 17:10:37-07 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include "inssort.h"
#include <string.h>

void swap(void *this, void *that, size_t size) {
   void *temp = malloc(size);
   memcpy(temp, this, size);
   memcpy(this, that, size);
   memcpy(that, temp, size);
   free(temp);
}

void inssort (void* base, size_t nelem, size_t size,
              int (*compar) (const void*, const void*)) {
   char *array = (char*)base;
   for (size_t sorted = 1; sorted < nelem; ++sorted) {
      int slot = sorted;
      void *copy = malloc(size);
      for (; slot > 0; --slot) {
         int cmp = (*compar)(array + slot * size, 
                             array + (slot -1) * size);
         if (cmp > 0) break;
         swap(array + slot * size, array + (slot-1) * size, size);
      }
   }
}
