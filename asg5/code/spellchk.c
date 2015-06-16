// $Id: spellchk.c,v 1.10 2015-03-07 14:21:25-08 - - $
// Author Olexiy VBurov, oburov@ucsc.edu

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "debug.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

// Prints the error.
void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", program_name, object, message);
   fflush (NULL);
   exit_status = EXIT_FAILURE;
}

// Function, which returns a pointer to a file specified by filename.
FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) {
       print_error (filename, strerror (errno));
       exit_status = 2;
   }
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

// Performs actual spellcheck of the file word by word.
void spellcheck (const char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   // Reads all the words from the file
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      // We check whether the word is not present
      // in our hashset and if not
      if (!has_hashset(hashset,yytext)) { 
         // We change first letter of it to lowercase and check again.
         yytext[0] = tolower(yytext[0]);
         if (!has_hashset(hashset,yytext)) {
            // If we did not find the word then we notify the user.
            printf("%s: %d: %s\n",filename,yylineno,yytext);
            exit_status = 1;
         }
      }
   }
}

// Loads dictionary into the hashset
void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   FILE *dictionary = open_infile(dictionary_name);
   char buffer[50];
   for (;;) {
      // We read each line into the buffer, change
      // newline character with the null plug and put
      // it into the hashset.
      char *buf = fgets (buffer,sizeof buffer, dictionary);
      if (buf == NULL) break;
      size_t length = strlen(buf) - 1;
      if (buf[length] == '\n')
          buf[length] = '\0';
      put_hashset(hashset,buf);
   }
   // We close the filestream in the end.
   fclose(dictionary);
}

// Option scanning function.
void scan_options (int argc, char** argv,
                   char **default_dictionary,
                   char **user_dictionary,
                   bool *debug,
                   bool *dump) {
   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': *user_dictionary = optarg;
                   break;
         case 'n': *default_dictionary = NULL;
                   break;
         case 'x': 
         // In case double -xx option is encountered we
         // not only change the debug flag but also the dump flag.
                   if (*debug != true) {
                      *debug = true;
                   } else {
                      *dump = true;
                   }
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debug_flags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }
}

int main (int argc, char **argv) {
   // We specify dictionaries and scan options here.
   program_name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   bool dump = false;
   bool debug = false;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   scan_options (argc, argv, &default_dictionary, 
   &user_dictionary, &debug, &dump);

   // Then we check whether we have at least one dictionary to load.
   if (default_dictionary == NULL && user_dictionary == NULL) {
      fprintf(stderr,"%s: dictionary is empty\n",program_name);
      exit_status = 2;
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   // Checks debug flags that were set by options.
   if (debug == true) {
      print_debug(hashset);
      if (dump == true) dump_hashset(hashset);
      exit_status = EXIT_SUCCESS;
      return exit_status;
   }

   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
  
   // Free the hashset in the end. 
   free_hashset(hashset);
   yylex_destroy ();
   return exit_status;
}

