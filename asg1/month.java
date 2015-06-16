// Author: Olexiy Burov, oburov@ucsc.edu
// 
// $Id: month.java,v 1.4 2015-01-18 19:10:13-08 - - $
//
// Describes a calendar month.

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Locale;

public class month {
   // Change date for British Empire.
   static final GregorianCalendar CHANGE_DATE
      = new GregorianCalendar(1752, Calendar.SEPTEMBER, 14);

   static final int WEEKS_IN_MONTH = 6;
   static final int DAYS_IN_WEEK = 7;

   // Array which stores all the days in.
   private int[][] month = null;

   // Calendar which iterates through dates.
   private GregorianCalendar calendar = new GregorianCalendar();

   // Locale which affects the language used to print dates.
   private static Locale locale = Locale.getDefault();

   // Helper method for getting first 2 letter of the day of the week.
   private static String prefix (String string, int length) {
      return length > string.length ()
         ? string
         : string.substring (0, length);
   }

   // Sets locale.
   public static void set_locale(Locale new_locale) {
      locale = new_locale;
   }

   // Constructor for month, creates specified month.
   public month(int calmonth, int calyear) {
      // Sets change.
      calendar.setGregorianChange(CHANGE_DATE.getTime());

      // Initializes array of days of the month.
      month = new int[WEEKS_IN_MONTH][DAYS_IN_WEEK];

      // Starting from the first day, we fill up the month.
      calendar.set(calyear, calmonth, 1);
      while (calmonth == calendar.get(GregorianCalendar.MONTH)) {
         int calday = calendar.get(GregorianCalendar.DAY_OF_MONTH);
         int weekday = calendar.get(GregorianCalendar.DAY_OF_WEEK) - 1;
         int week = calendar.get(GregorianCalendar.WEEK_OF_MONTH) - 1;
         month[week][weekday] = calday;
         calendar.add(GregorianCalendar.DAY_OF_MONTH, 1);
      }
      calendar.set(calyear, calmonth, 1);
   }

   // Returns a string containing day names of the week.
   public String daynames() {
      StringBuffer string_buffer = new StringBuffer();
      calendar.set(Calendar.DAY_OF_WEEK,1);
      for (int i = 0;i < DAYS_IN_WEEK - 1;i++)
      {
         string_buffer.append(
            prefix(String.format(locale,"%tA",calendar),2)+" ");

         calendar.add(Calendar.DAY_OF_MONTH,1);
      }
      string_buffer.append(
         prefix(String.format(locale,"%tA",calendar),2));

      return string_buffer.toString();
   }

   // Returns centrally aligned string containing name of the month.
   public String monthname() {
      return misclib.central_align(String.format(locale, " %tB", calendar), 20);
   }

   // Returns month name followed by a year.
   public String month_year() {
      return String.format(locale, "%tB %d", calendar,
         calendar.get(Calendar.YEAR));
   }

   // Returns string which represent one of six weeks in the month.
   public String week_by_index(int index) {
      StringBuffer string_buffer = new StringBuffer();

      for (int i = 0; i < DAYS_IN_WEEK; i++) {
         int dayOfWeek = month[index][i];

         // First week empty days count as two whitespaces.
         if (dayOfWeek == 0 && index < 1)
            string_buffer.append("   ");
         // Any other  week empty days do not count as symbols at all.
         else if (dayOfWeek == 0)
            string_buffer.append("");
         else
            string_buffer.append(String.format("%2d ", dayOfWeek));
      }

      // Trims the weeks, so that the are no extra whitespaces left in
      // the end of the line.
      try {
         string_buffer.deleteCharAt(string_buffer.lastIndexOf(" "));
      }

      catch (Exception ex) {
      }

      return string_buffer.toString();
   }

   // toString implementation for the month class.
   // returns string which represents month, year and all the days of
   // that month in columns.
   @Override
   public String toString() {
      StringBuffer string_buffer = new StringBuffer();
      string_buffer.append(String.format("%s%n%s%n",
         misclib.central_align(month_year(), 20),
         daynames()));

      for (int i = 0; i < 6; i++) {
         string_buffer.append(String.format("%s%n",
            this.week_by_index(i)));
      }

      return string_buffer.toString();
   }
}
