// $Id: linesort.c,v 1.12 2015-03-11 19:34:51-07 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inssort.h"

// String compare function.
int string_cmp(const void *this, const void *that) {
   char **str1 = (char **)this;
   char **str2 = (char **)that;
   return strcmp(*str1,*str2);
}
int main() {
   char *array[0x1000];
   char buffer[0x1000];
   // nelem keeps track of the number of elements and indexes.
   int nelem = 0;
   for (;nelem < 0x1000;++nelem) {
      // We read the string
      char *buf = fgets (buffer, sizeof buffer, stdin);
      if (buf == NULL) break;
      // Change '\n' to '\0'
      size_t length = strlen(buf) - 1;
      if (buf[length] == '\n')
          buf[length] = '\0';
      // Duplicate at index
      array[nelem] = strdup(buf);
   }
   // Sort it.
   inssort(array, nelem, sizeof *array, string_cmp);
   // Print one string per line.
   for (int index = 0; index < nelem; ++index) {
      printf("%s\n",array[index]);
      free(array[index]);
   }
}
