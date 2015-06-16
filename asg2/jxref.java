// $Id: jxref.java,v 1.7 2015-01-31 13:30:45-08 - - $
// Author: Olexiy Burov, oburov@ucsc.edu

import java.io.*;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import static java.lang.System.*;

class jxref {
   private static final String STDIN_FILENAME = "-";

   // Regular expression for words
   private static final String REGEX = "\\w+([-'.:/]\\w+)*";
   private static final Pattern PATTERN = Pattern.compile(REGEX);

   // Map of words.
   private static listmap map;

   // Adds all the words into the map.
   private static void xref_file (Scanner file, String filename) {
      map = new listmap();
      for (int linenr = 1; file.hasNextLine(); ++linenr) {
         String line = file.nextLine();
         Matcher match = PATTERN.matcher (line);
         while (match.find()) {
            String word = match.group();
            map.insert(word,linenr);
         }
      }

      out.println("::::::::::::::::::::::::::::::::");
      out.println(filename);
      out.println("::::::::::::::::::::::::::::::::");

      // Prints the map in the end.
      print_map();
   }
   

   // Prints the map.
   private static void print_map() {
      // For each entry in the map print word(key)
      // and the number of times word appears in the file.
      for (Entry<String, intqueue> entry : map) {
         out.printf("%s [%d]", entry.getKey(),
                               entry.getValue().getcount());
         for (int line_number : entry.getValue()) {
            // For each number in the queue print the number.
            out.printf(" %d", line_number);
         }
         // After we are done with one entry in the map print newline.
         out.printf("%n");
      }
   }

   // For each filename, open the file, cross reference it,
   // and print.
   private static void xref_filename (String filename) {
      if (filename.equals (STDIN_FILENAME)) {
         xref_file (new Scanner (System.in), filename);
      }else {
         try {
            Scanner file = new Scanner (new File (filename));
            xref_file (file, filename);
            file.close();
         }catch (IOException error) {
            misc.die (error.getMessage());
         }
      }
   }

   // Main function scans arguments to cross reference files.
   public static void main (String[] args) {
      if (args.length == 0) {
         xref_filename (STDIN_FILENAME);
      } else {
         for (String filename: args) {
            xref_filename (filename);
         }
      }
      exit (misc.exit_status);
   }

}
