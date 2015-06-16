// Author: Olexiy Burov, oburov@ucsc.edu
//
// $Id: year.java,v 1.2 2015-01-17 17:30:05-08 - - $
//
// Describes a calendar year

public class year {
   // A year is an array of months.
   private month[] year = null;

   // Keeps track of the year.
   private String calyear;

   // StringBuffer for printing the year.
   private StringBuffer string_buffer = new StringBuffer();

   private final int MONTHS_IN_YEAR = 12;

   // Constructor.
   public year(int calyear) {

      this.calyear = String.valueOf(calyear);

      year = new month[MONTHS_IN_YEAR];

      for (int i = 0; i < MONTHS_IN_YEAR; i++) {
         year[i] = new month(i, calyear);
      }
   }

   // adds the year header to the string_buffer.
   private void print_year_header() {
      string_buffer.append(String.format("%s\n\n",
         misclib.central_align(calyear, 66)));
   }

   // adds names of the months to the string_buffer.
   private void print_monthname_thrice(int index) {
      String[] monthnames = new String[3];
      for (int i = 0; i < 3; i++) {
         monthnames[i] = year[index].monthname();
         index++;
      }
      string_buffer.append(String.format("%s   %s   %s%n",
         monthnames[0],monthnames[1],monthnames[2]));
   }

   // adds names of the days of the week to the string_buffer.
   private void print_daynames_thrice(int index) {
      String[] daynames = new String[3];
      for (int i = 0; i < 3; i++) {
         daynames[i] = year[index].daynames();
         index++;
      }
      string_buffer.append(String.format("%s   %s   %s%n",
         daynames[0],daynames[1],daynames[2]));
   }

   // adds day numbers to the string_buffer.
   private void print_weeks_thrice(int index) {
      for (int i = 0; i < 6; i++) {
         String[] weeks = new String[3];
         for (int j = 0; j < 3; j++) {
            weeks[j] = year[index].week_by_index(i);
            index++;
         }

         // switch controls the output, depending on the length of the
         // last week among three weeks.
         switch (weeks[2].length()) {
            case 0:
               switch (weeks[1].length()) {
                  case 0:
                     switch (weeks[0].length()) {
                        case 0:
                           string_buffer.append(String.format("%n"));
                           break;
                        default:
                           string_buffer.append(String.format("%s%n",
                              weeks[0]));
                           break;
                     }
                     break;
                  default:
                     string_buffer.append(String.format("%-20s   %s%n",
                        weeks[0], weeks[1]));
                     break;
               }
               break;
            default:
               string_buffer.append(String.format(
                  "%-20s   %-20s   %s%n",weeks[0],weeks[1],weeks[2]));
               break;
         }

         index -= 3;
      }
   }

   // toString implementation which puts it all together.
   @Override
   public String toString() {
      print_year_header();

      for (int i = 0; i < 12; i += 3) {
         print_monthname_thrice(i);
         print_daynames_thrice(i);
         print_weeks_thrice(i);
      }

      string_buffer.append("\n");

      return string_buffer.toString();
   }
}
