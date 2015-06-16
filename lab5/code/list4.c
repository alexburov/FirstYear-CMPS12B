// $Id: list4.c,v 1.23 2015-02-10 21:31:56-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
   char *word;
   node *link;
};

// Recursive memory free mechanism.
void free_list(node *head) {
   if (head == NULL) return;
   free_list(head->link);
   free(head);
}

int main (int argc, char **argv) {
   node *head = NULL;
   for (int argi = 0; argi < 5; ++argi) {
      node *node = malloc (sizeof (struct node));
      assert (node != NULL);
      node->word = argv[argi];
      node->link = head;
      head = node;
   }
   for (node *curr = head; curr->link != NULL; curr = curr->link) {
      printf ("%p->node {word=%p->[%s], link=%p}\n",
              curr, curr->word, curr->word, curr->link);
   }

   free_list(head);
   return 0;
}
