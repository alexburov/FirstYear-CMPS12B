// $Id: bigint.c,v 1.2 2015-02-26 22:40:20-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

// Bigint structure.
struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

// Removes high order zeores from the bigint
void trim_zeros (bigint *this) {
   while (this->size > 1) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

// Returns the largest out of two bigint objects
bigint *get_largest(bigint *this, bigint *that) {
   bigint *largest = this; 

   // If they have the same sizes then compare digit by digit
   // starting from the most significant digits at size - 1.
   if (this->size == that->size) {
      for (int index = this->size-1; index >= 0; index--) {
         if (this->digits[index] < that->digits[index]) {
            largest = that;
            break;
         } else if (that->digits[index] < this->digits[index]){
            largest = this;
            break;
         }
      }
   } else { 
      // If the sizes are different just return the one that is bigger.
      largest = (this->size) > (that->size) ? this :
                                              that ;
   }
   return largest;
}

// Bigint constructor function.
bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}

// Constructs bigint from the string.
bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   return this;
}

// Addition function.
bigint *do_add (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   // The size of the result can be as big as the size
   // of the biggest + 1. We assume that this is always the
   // largest of two numbers and that is the smallest.
   size_t maxsize = this->size+1;

   bigint *result = new_bigint(maxsize);
   int carry = 0;

   // We do the addition in this loop.
   for (size_t index = 0; index < maxsize; index++) {
      size_t digit1 = index < this-> size ? this->digits[index] : 0; 
      size_t digit2 = index < that->size ? that->digits[index] : 0;
      size_t digit_result = digit1 + digit2 + carry;
      result->digits[index] = digit_result % 10;
      carry = digit_result / 10;
   }
   
   // Set the size of the bigint object.
   result->size = maxsize;
   trim_zeros(result);
   return result;
}

// Substraction function.
bigint *do_sub (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result = new_bigint(this->size);
   int borrow = 0;
   // Substraction loop.
   for (size_t index = 0; index < this->size; index++) {
      size_t digit1 = this->digits[index];
      size_t digit2 = index < that->size ? that->digits[index] : 0;
      size_t digit_result = digit1 - digit2 - borrow + 10;
      result->digits[index] = digit_result % 10;
      borrow = 1 - (digit_result / 10);
   }
   // Set the result's size to that of the biggest bigint object.
   // We again assume that this is always the bigger the method
   // add_bigints gurantees that precondition.
   result->size = this->size;
   trim_zeros(result);
   return result;
}

// Destructor function.
void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

// Function to print the bigint.
void print_bigint (bigint *this) {
   DEBUGS ('b', show_bigint (this));
   // Special case when the bigint has one digit and that is 0
   // then just print a zero, ignoring sign.
   if (this->size == 1 && this->digits[0] == 0) {
      printf("0");
   } else {
    // Print negative sign before the negative number.
    if (this->negative) printf("-");
      int charnbr = 0; 
      for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte, charnbr++) {
         // Put line breaks every 69th character, just like dc.
         if (charnbr % 69 == 0 && charnbr != 0) printf("\\\n");
      printf ("%d", *byte);
      }
   }
   printf("\n");
}



// Function, which decides on how to perform the addition of two bigints
bigint *add_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));

   // Here we figure out which one is bigger and which one is smaller.
   bigint *largest = get_largest(this,that);
   bigint *smallest = largest == this ? that : this;

   // That's the decision free, if the signs are the same, do addition
   bigint *result = NULL; 
   if (this->negative == that->negative) {
      result = do_add(largest,smallest);
      result->negative = this->negative;
   } else { // else do substraction
      result = do_sub(largest,smallest);
      result->negative = largest->negative;
   }

   return result;
}

// Function, which decides on how to perform 
// the substruction of two bingints.
bigint *sub_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));

   // Here we figure out which one is bigger and which one is smaller.
   bigint *largest = get_largest(this,that);
   bigint *smallest = largest == this ? that : this;

   bigint *result = NULL;
   if (this->negative == that->negative) {
      // We substract if the signs are the same.
      result = do_sub(largest,smallest);
      // It's a sign determination 6-way logic condition.
      result->negative = (this == smallest) ?
       (this->negative ? false : true) :
       (this->negative ? true : false);
   } else {
      // However, we add when the signs are different.
      result = do_add(largest,smallest);
      result->negative = this->negative;
   }
   return result;
}

// Function, which mediates the multiplication process.
bigint *mul_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));

   // The result would contain as much numbers as 
   // both the multiplicants have together.
   bigint *result = new_bigint(this->size + that->size);
   // Multiplication loop.
   for (size_t index_first = 0; index_first < this->size;
               index_first++) {
      int carry = 0;
      for (size_t index_second = 0; index_second < that->size;
                  index_second++) {
         int digit = result->digits[index_first+index_second] +
          this->digits[index_first] * that->digits[index_second] +
                                        carry;
          result->digits[index_first+index_second] = digit % 10;
          carry = digit / 10;
      }
      // We add a carry on top of the number 
      // we already have, before we go to the next one.
      result->digits[index_first+that->size] = carry;
   }
   // Then we setup the size and signs.
   result->size = this->size + that->size;
   // Signs rule is easy here:
   // SAME = POSITIVE
   // DIFFERENT = NEGATIVE
   if (this->negative != that->negative) result->negative = true;
   trim_zeros(result);
   return result;
}

// Function, which does debuging.
void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

