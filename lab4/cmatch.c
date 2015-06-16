// Author Olexiy Burov, oburov@ucsc.edu
//
// $Id: cmatch.c,v 1.6 2015-02-07 22:55:52-08 - - $
// 
// NAME
//      cmatch -- print matching lines from files
//
//      SYNOPSIS
//           cmatch [-iln] string [filename...]
#define _GNU_SOURCE

#include <libgen.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

const char *program_name = NULL;
int exit_status = EXIT_SUCCESS;
#define STDIN_NAME "-"

// Options implementation from code examples.
typedef struct options {
    bool ignore_case;
    bool filenames_only;
    bool number_lines;
} options ;

// scan_options method merged from the example code.
void scan_options (int argc, char * const *argv, options *opts) {
    opts->ignore_case = false;
    opts->filenames_only = false;
    opts->number_lines = false;
    opterr = false;
    for (;;) {
        int opt = getopt(argc, argv, "iln");
        if (opt == EOF) break;
        switch (opt) {
            case 'i':
                opts->ignore_case = true;
                break;
            case 'l':
                opts->filenames_only = true;
                break;
            case 'n':
                opts->number_lines = true;
                break;
            default:
                exit_status = EXIT_FAILURE;
                fflush(NULL);
                fprintf(stderr, "%s: -%c: invalid option\n",
                        program_name, optopt);
                break;
        }
    }
}

// matches pattern string from the file input
// depending on specified options opts
void match_by_line (FILE *input, const char *filename, options *opts,
                             const char *pattern, int argc) {
    //buffer to read lines in.
    char buffer[1024];
    //boolean flag to keep track whether we found at lead one occurence
    bool found = false;
    for (int linenr = 1;; ++linenr) {
        char *buf = fgets (buffer, sizeof buffer, input);
        if (buf == NULL) break;
        
        // compares strings using strstr or
        // strcasestr depending on -i flag
        bool matched = opts->ignore_case ? strcasestr(buf,pattern):
                                          strstr(buf,pattern);
        if (matched) {
            found = matched;
            // If we don't have -l flag turned on:
            if (!opts->filenames_only) {
               // If multiple files read, then print their names.
               if (argc > 1) printf("%s:",filename);
               // If -n flag specified, print line numbers
               if (opts->number_lines) printf("%d:",linenr);
               // Print the actual line with the pattern in it.
               printf("%s",buf);
            }
        }
    }
    // If there happened to be an occurence of patern
    // and -l flag is specified then just print the filename,
    // supressing the other output.
    if (found && opts->filenames_only) printf("%s\n",filename);
}


int main(int argc, char * const argv[]) {
   program_name = argv[0];
   // in case no arguments were passed - quit with error message:
   if (argc == 1) {
      fprintf(stderr,"Usage: %s [-iln] string [filename...]\n",
              program_name);
      return 1;
   }
    // setting up options
    options opts;
    program_name = basename (argv[0]);
    scan_options(argc, argv, &opts);
    
    // If we are reading the pattern from the stdin:
    if (argc == 2) {
        const char *pattern = argv[1];
        match_by_line(stdin,STDIN_NAME, &opts, pattern,1);
    } else {
        const char *pattern = argv[optind];
        // Open all the specified files
        for (int argi = optind + 1; argi < argc; ++argi) {
            const char *filename = argv[argi];
                FILE *input = fopen (filename, "r");
                if (input != NULL) {
                    // match by line each file.
                    // count variable just tells how many files we read
                    int count = argc - (optind + 1);
                    match_by_line(input,filename,&opts,pattern,count);
                    fclose(input);
                } else {
                    exit_status = EXIT_FAILURE;
                    fflush (NULL);
                    fprintf(stderr, "%s: %s: %s\n", program_name,
                            filename, strerror(errno));
                    fflush(NULL);
                }
        }
    }
    return exit_status;
}

