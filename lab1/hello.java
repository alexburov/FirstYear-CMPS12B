// $Id: hello.java,v 1.5 2015-01-12 15:47:28-08 - - $
// Author: Olexiy Burov, oburov@ucsc.edu
public class hello {
   public static void main(String[] args) {
      if (args.length > 0) {
         System.err.println("Usage: hello");
         System.exit(1);
        } else {
         System.out.println("Hello, World!");
         System.exit(0);
        }
   }
}
