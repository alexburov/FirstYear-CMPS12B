// $Id: mstat.c,v 1.1 2015-03-02 22:31:51-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#define DAYS180 180 * 24 * 3600

// mstat program.
int main (int argc, char **argv) {
   int exit_status = EXIT_SUCCESS;

   // For each argument:
   for (int index = 1; index < argc; ++index) {
      // Get the filename
      char *filename = argv[index];
      // Allocate new stat structure.
      struct stat *status_buffer = malloc(sizeof(struct stat));
      assert (status_buffer!= NULL);
      // If the lstat failed - print an error and continue.
      if (lstat(filename,status_buffer)){
         exit_status = 1;
         fprintf(stderr,"%s: %s: %s\n",argv[0], filename,
                                        strerror(errno));
         continue;
      } else {
         // Specify format if the file was created 180 days from now.
         char *format = status_buffer->st_mtime < time(NULL) - DAYS180 
         || time(NULL) + DAYS180 < status_buffer->st_mtime 
         ? "%b %e  %Y" : "%b %e %R"; 
         struct tm mt_time;
         // Get the time of the last modification of the file.
         localtime_r (&status_buffer->st_mtime, &mt_time);
         char buffer[64];
         // Put the time in specified format in the buffer.
         strftime (buffer, sizeof buffer,format, &mt_time);
         // Print the access, size, last modification time and filename
         printf("%06o %9ld %s %s",status_buffer->st_mode,
         status_buffer->st_size, buffer, filename);
         char linkname[PATH_MAX + 1];
         ssize_t retval = readlink (filename, linkname,
                                       sizeof linkname);
         // Print linkname if any.
         if (retval >= 0) {
            linkname[retval < PATH_MAX + 1 ? retval : PATH_MAX] = '\0';
            printf (" -> %s", linkname);
         } 
         printf("\n");
      }
      // Free up the stat struct.
      free(status_buffer);
   }
   return exit_status;
}
