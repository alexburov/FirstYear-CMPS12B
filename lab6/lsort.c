// $Id: lsort.c,v 1.1 2015-02-18 00:15:18-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>

// Implementation of the node.
typedef struct node node;
struct node {
   char *item;
   node *link;
};

// Head of the list.
node *head = NULL;

// Function to insert strings in the list
void insert_ascending(char *new_item) {
   node *prev = NULL;
   node *curr = head;

   while(curr != NULL) {
      if (strcmp(curr->item,new_item) > 0) break;
      prev = curr;
      curr = curr->link;
   }

   // Creation of the new node in the list.
   node *temp = malloc(sizeof(struct node));
   // Check the malloc result
   assert(temp != NULL);
   // Creation of the new string by strdup function.
   temp->item = strdup(new_item);
   // Check the result of strdup.
   assert(temp->item != NULL);
   temp->link = curr;
   if (prev == NULL) head = temp;
                 else prev->link = temp;
}

// Function to free the list memory
void free_list() {
   while(head != NULL) {
      node *old = head;
      head = head->link;
      // Here we are freeing both item field and the node itself.
      free(old->item);
      free(old);
   }
}

// Function to print the list.
void print_list() {
    for (node *curr = head; curr != NULL; curr = curr->link)
      printf("%s\n", curr->item);
}

// Function to print the list in the debug format.
void print_debug(char *program_name) {
   printf ("%s: head= %p\n", program_name, head);
   while (head != NULL) {
      node *old = head;
      head = head->link;
      printf ("%s: %p-> node {\n"
              "    string= %p->\"%s\",\n"
              "    link= %p}\n",
              program_name, old, old->item, old->item, old->link);
   }
}

int main(int argc, char ** argv) {
   char *program_name = basename (argv[0]);
   char buffer[82];
   int exit_status = EXIT_SUCCESS;



   for (int linenr = 1;;++linenr) {
      // Read the string in the buffer.
      char *gotLine = fgets(buffer, sizeof buffer, stdin);
      if (gotLine == NULL) break;

      char *nlpos = strchr(buffer, '\n');
      if (nlpos != NULL) {
         *nlpos = '\0';
         insert_ascending(buffer);
      } else {
         fprintf(stderr, "%s: %d: unterminated line: %s\n",
                 program_name, linenr, buffer);
         exit_status = EXIT_FAILURE;
      }
   }

   // Option handling
   int opt = getopt(argc, argv, "d");
   if (opt == 'd') {
      print_debug(program_name);
   } else {
       print_list();
   }
   free_list();
   return exit_status;
}

