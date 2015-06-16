// Author: Olexiy Burov, oburov@ucsc.edu
//
// $Id: jcal.java,v 1.11 2015-01-19 21:46:50-08 - - $
//
// NAME
//    jcal - prints calendar
//
// SYNOPSIS
//    jcal [-locale] [[month] year]
//
// DESCRIPTION
//    Program which displays the calendar for any particular
//    month or year with international support.


import java.util.Locale;
import static java.lang.System.*;

public class jcal {

   // keeps track of the year.
   private static int calyear = 0;

   //keeps track of the month.
   private static int calmonth = 0;

   // transforms current time into year in form of integer.
   static int intyear(long now) {
      return Integer.parseInt(String.format("%tY", now));
   }

   // transforms current time into month in form if integer.
   static int intmonth(long now) {
      return Integer.parseInt(String.format("%tm", now));
   }

   // prints error about illegal year argument and exits with status 1.
   private static void print_year_error() {
      misclib.die("illegal year value: use 1-9999");
   }

   // prints error about illegal month argument and exits with status 1.
   private static void print_month_error() {
      misclib.die("illegal month value: use 1-12");
   }

   // reads year argument.
   private static void read_year(String year) {

      try {
         calyear = Integer.parseInt(year);
      }

      catch (NumberFormatException ex) {
         print_year_error();
      }

      if (calyear > 9999 || calyear < 1) {
         print_year_error();
      }
   }

   // reads month argument.
   private static void read_month(String month) {
      try {
         calmonth = Integer.parseInt(month);
      }

      catch (NumberFormatException ex) {
         print_month_error();
      }

      if (calmonth > 12 || calmonth < 1) {
         print_month_error();
      }

      // decrease calmonth, because our calendar works with values 0-11.
      calmonth--;
   }

   // displays current month.
   private static void display_this_month() {
      // get current time.
      long now = System.currentTimeMillis();

      // creates a month, based on current time.
      month thisMonth = new month(intmonth(now) - 1, intyear(now));

      // prints the month.
      out.printf("%s", thisMonth);
   }

   // displays specific year.
   private static void display_year(String year) {
      read_year(year);

      year some_year = new year(calyear);

      out.printf("%s", some_year);
   }

   // displays specific month of a specific year.
   private static void display_month(String month, String year) {
      read_month(month);
      read_year(year);

      month some_month = new month(calmonth, calyear);

      out.printf("%s", some_month);
   }

   public static void main(String[] args) {
      // check if there are any locale options in the parameter list.
      if (args.length > 0 && args[0].charAt(0) == '-'){
         // if yes, assign locale for the month class.
         String locale = args[0].substring(1);
         month.set_locale(new Locale(locale));
         switch (args.length) {
            // only locale - prints current month.
            case 1:
               display_this_month();
               break;
            // locale + year(1-9999) - prints specified year.
            case 2:
               display_year(args[1]);
               break;
            // locale+month(1-12)+year(1-9999) - prints specified month
            case 3:
               display_month(args[1],args[2]);
               break;
         }
      } else {

         // if no locale options enabled:
         switch (args.length) {
            // prints current month(no arguments)
            case 0:
               display_this_month();
               break;
            // one argument, prints specified year(1-9999)
            case 1:
               display_year(args[0]);
               break;
            // two arguments, prints specified month.
            case 2:
               display_month(args[0],args[1]);
               break;
         }
      }
   }
}
