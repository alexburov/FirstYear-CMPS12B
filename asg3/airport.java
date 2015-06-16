// $Id: airport.java,v 1.33 2015-02-17 19:23:47-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class airport {
   // Loads database from the file.
   private static treemap load_database(String database_name) {
      // Create new binary search tree
      treemap tree = new treemap();
      try {
         // Try opening the file into Scanner
         Scanner database = new Scanner(new File(database_name));
         // Read it line by line, tracking the line numbers
         for (int linenr = 1; database.hasNextLine(); linenr++) {
            String line = database.nextLine();
            // Make sure it's in the correct format
            if (line.matches ("^\\s*(#.*)?$")) continue;
            String[] keyvalue = line.split (":");
            if (keyvalue.length != 2) {
               misc.warn (database_name , linenr, "invalid line");
               continue;
            }
            // Put the value to the binary search tree.
            tree.put(keyvalue[0],keyvalue[1]);
         }
         // Stop reading from the file.
         database.close();
      } catch (IOException error) {
         misc.die(error.getMessage());
      }
      return tree;
   }

   public static void main (String[] args) {
      // First, check for options in the arguments.
      if (args.length == 2 && args[0].startsWith("-")) {
         // If "-d" option is found, load
         // the databse and print it in debug format.
         if (args[0].equals("-d")) {
            treemap tree = load_database(args[1]);
            tree.debug_tree();
         } else {
            misc.die(args[0], "Invalid option");
         }
      } else if (args.length > 2 || args.length < 1) {
         misc.die(String.format("Usage: %s [-d] database",
                                      misc.program_name));
      } else {
         // If there is one argument, try to load it to the tree,
         // assuming that's the databse.
         treemap tree = load_database(args[0]);
         Scanner stdin = new Scanner(in);
         // Then start scanning System.in for
         // user entered airport codes
         while (stdin.hasNextLine()) {
            String airport = stdin.nextLine().toUpperCase().trim();

            // Disregard coments in test input files
            if (airport.matches ("^\\s*(#.*)?$")) continue;
            
            // Look for the airport in the binary tree. 
            String airport_name = tree.get(airport);
            // If not found print an error.
            if (airport_name == null) {
               misc.warn(airport,"no such airport");
            } else { // Print the found airport name.
               out.printf("%s = %s%n", airport, airport_name);
            }
         }
      }
      exit(misc.exit_status);
   }
}
