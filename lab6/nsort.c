// $Id: nsort.c,v 1.1 2015-02-18 00:15:12-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>

// Implementation of the list element.
typedef struct node node;
struct node {
   double item;
   node *link;
};

// Head of the list.
node *head = NULL;

// Insertion function.
void insert_ascending(double new_item) {
   node *prev = NULL;
   node *curr = head;

   while(curr != NULL) {
      if (curr->item > new_item) break;
      prev = curr;
      curr = curr->link;
   }

   node *temp = malloc(sizeof(struct node));
   assert(temp != NULL);
   temp->item = new_item;
   temp->link = curr;
   if (prev == NULL) head = temp;
                 else prev->link = temp;
}

// Function to free memory allocated for the list.
void free_list() {
   while(head != NULL) {
      node *old = head;
      head = head->link;
      free(old);
   }
}

// Function to print the list.
void print_list() {
    for (node *curr = head; curr != NULL; curr = curr->link)
      printf("%24.15g\n", curr->item);
}

// Function to print the list in the debug format.
void print_debug(char *program_name) {
   printf ("%s: head= %p\n", program_name, head);
   while (head != NULL) {
      node *old = head;
      head = head->link;
      printf ("%s: %p-> node {\n"
              "    item= %24.15g\n"
              "    link= %p}\n",
              program_name, old, old->item, old->link);
   }
}

int main(int argc, char ** argv) {
   char *program_name = basename (argv[0]);
   int exit_status = EXIT_SUCCESS;
   // Buffer to store wrongly entered data.
   char error_buffer[102];



   for (;;) {
      // Reads the number.
      double number;
      int scan_status = scanf("%lg", &number);


      if (scan_status == EOF) {
         break;
      } else if (scan_status == 1) {
         insert_ascending(number);
      } else {
         scan_status = scanf("%s", &error_buffer);
         assert(scan_status == 1);
         fprintf(stderr,"%s: bad number \"%s\"\n",
                       program_name,error_buffer);
         exit_status = EXIT_FAILURE;
      }
   }

   // Options handling.
   int opt = getopt(argc, argv, "d");
   if (opt == 'd') {
      print_debug(program_name);
   } else {
       print_list();
   }
   free_list();
   return exit_status;
}
