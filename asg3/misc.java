// $Id: misc.java,v 1.1 2013-01-31 17:00:19-08 - - $

import static java.lang.System.*;

class misc {
   public static final int EXIT_SUCCESS = 0;
   public static final int EXIT_FAILURE = 1;
   public static final String program_name =
                 basename (getProperty ("java.class.path"));
   public static int exit_status = EXIT_SUCCESS;

   // constructor - prevents instantiation: only static fns allowed.
   private misc() {
      throw new UnsupportedOperationException();
   }

   // basename - strips the dirname and returns only the basename.
   //            See:  man -s 3c basename
   public static String basename (String pathname) {
      if (pathname == null || pathname.length() == 0) return ".";
      String[] paths = pathname.split ("/");
      return paths.length == 0 ? "." : paths[paths.length - 1];
   }

   // warn - print a warning and set exit status to failure.
   public static void warn (Object... args) {
      err.printf ("%s", program_name);
      for (Object arg: args) err.printf (": %s", arg);
      err.printf ("%n");
      exit_status = EXIT_FAILURE;
   }

   // die - print a warning and exit program.
   public static void die (Object... args) {
      warn (args);
      exit (exit_status);
   }

}
