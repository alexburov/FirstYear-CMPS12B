// $Id: hashset.c,v 1.10 2015-03-07 14:21:20-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

// Structure, which describes the node in the hashset chain.
typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

// Structure, which describes the hashset itself.
struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

// Hashset constructor.
hashset *new_hashset (void) {
   // We allocate memory for the hashset structure.
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   // We allocate memory for chains array.
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %zd, chains=%p}\n",
                this, this->size, this->chains);
   // Then we return resulting hashset.
   return this;
}

// Hashset destructor.
void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   // Frees hashset's memory in postorder manner.
   // Here we free each chain
   for (size_t index = 0; index < this->size; ++index) {
      hashnode *chain = this->chains[index];
      while (chain != NULL) {
         hashnode *old = chain;
         chain = chain->link;
         free(old->word);
         free(old);
      }
   }
   // Then free array of chains
   free(this->chains);
   // Then free the hashset itself.
   free(this);
}

// Prints the -x debug format.
void print_debug(hashset *this) {
   printf("%zu words in the hash set\n", this->load);
   printf("%zu size of the hash array\n", this->size);
   // Array to count lenghts of chains.
   int lengths[10] = {0};
   
   // For loop counts the length of the chain and increments
   // the corresponding index in the array.
   for (size_t index = 0; index < this->size; ++index) {
      int count = 0;
      for (hashnode *curr = this->chains[index]; curr != NULL;
                     curr = curr->link) {
         count++;
      }
      lengths[count]++;
   }
   // Then we print all the chain-lengths we recorded.
   for (int i = 1; i < 10; ++i) {
      if (lengths[i] != 0) {
         printf("%d chains of size %d\n", lengths[i], i);
      }
   }
}

// Prints out the whole hashset.
void dump_hashset(hashset *this) {
   // Traverses each chain and prints its elements.
   for (size_t index = 0; index < this->size; ++index) {
      if (this->chains[index]) {
         printf("array[%10zu] = %20lu \"%s\"\n",index,
         strhash(this->chains[index]->word),
         this->chains[index]->word); 
         for (hashnode *curr = this->chains[index]->link; 
          curr != NULL; curr = curr->link) {
            char *word = curr->word;
            printf("                  = %20lu \"%s\"\n",
            strhash(word),word);
         }
      }
   }
}

// Doubles the capacity of the hashset.
void double_hashset(hashset *this) {
   // We calculate the new size.
   size_t new_size = (this->size * 2 + 1);
   // Allocate the array of that size.
   hashnode **new_array = malloc(new_size * sizeof (hashnode *));
   assert (new_array != NULL);
   // Set all of its elements to 0.
   memset(new_array, 0, new_size * sizeof (hashnode *));
   // Then we pop each node from the old chain and push into the new one
   // with the new index, because the new array has different size.
   for (size_t index = 0; index < this->size; ++index) {
      hashnode *old_chain = this->chains[index];
      while(old_chain != NULL) {
         hashnode *t = old_chain;
         old_chain = old_chain->link;
         size_t new_hash_index = strhash(t->word) % new_size;
         hashnode *new_chain = new_array[new_hash_index];
         t->link = new_chain;
         new_array[new_hash_index] = t;
      }
   }
   // We then free the old array and assign new array to be out array.
   free(this->chains);
   this->chains = new_array;
   this->size = new_size;
}

// Puts new item in the hashset.
void put_hashset (hashset *this, const char *item) {
   // It starts similarly with has_hashset by first performing
   // the search on the hashset looking whether we already have this
   // item in it.
   if (this->load * 2 > this->size) double_hashset(this);
   size_t hash_index = strhash(item) % this->size;
   hashnode *chain = this->chains[hash_index];
   for (hashnode *curr = chain; curr != NULL; curr = curr->link) {
      // If the item is there we just return.
      if (strcmp(curr->word, item) == 0) return;
   }
   // If not we create a new node, fill it with the values
   // from the parameters and append it to the old chain.
   hashnode *new_node = malloc(sizeof (struct hashnode));
   assert (new_node != NULL);
   new_node->link = chain;
   new_node->word = strdup(item);
   assert (new_node->word != NULL);
   this->chains[hash_index] = new_node; 
   this->load++;
}

// Checks whether the item is in the hashset.
bool has_hashset (hashset *this, const char *item) {
   // Calculates the hashindex and then traverses the chain
   // at that index looking for our item.
   size_t hash_index = strhash(item) % this->size;
   hashnode *chainnode = this->chains[hash_index];
   for (hashnode *curr = chainnode; curr != NULL; curr = curr->link) {
      if (strcmp(item, curr->word) == 0) return true;
   }
   return false;
}

