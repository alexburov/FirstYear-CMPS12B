// $Id$
// Author Olexiy Burov, oburov@ucsc.edu

#ifndef _INSORT_H_
#define _INSORT_H_

#include <stdlib.h>
#include <stdio.h>

void inssort(void* base,size_t nelem,size_t size,
             int (*compar) (const void*, const void*));

#endif
