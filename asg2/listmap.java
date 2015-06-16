// $Id: listmap.java,v 1.5 2013-10-16 17:10:32-07 - - $
// Author: Olexiy Burov, oburov@ucsc.edu
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.NoSuchElementException;

class listmap implements Iterable<Entry<String,intqueue>> {

   // Node implementation.
   private class node implements Entry<String,intqueue> {
      String key;
      intqueue queue = new intqueue();
      node link;

      public String getKey() {
         return key;
      }
      public intqueue getValue() {
         return queue;
      }
      public intqueue setValue (intqueue queue) {
         throw new UnsupportedOperationException();
      }
   }
   private node head;

   public void insert (String key, int linenr) {
      node prev = null;
      node curr = head;
      int compare = 1;
      // Phase 1 - find insertion point. -- O(n)
      while (curr != null) {
         // Compare two strings.
         compare = curr.getKey().compareTo(key);
         // If they are equal add their line number to the queue.
         if (compare == 0) {
            curr.queue.insert(linenr);
            break;
         } else if (compare > 0) break;
         prev = curr;
         curr = curr.link;
      }
      // Phase 2 - insert if not already there. -- O(1)
      if (compare != 0) {
         node tmp = new node();
         tmp.key = key;
         tmp.link = curr;
         if (prev == null) head = tmp;
                      else prev.link = tmp;
         tmp.queue.insert(linenr);
      }
   }

   // Iterator implementation.

   public Iterator<Entry<String,intqueue>> iterator() {
      return new iterator();
   }

   private class iterator
      implements Iterator<Entry<String,intqueue>> {
      node curr = head;

      public boolean hasNext() {
         return curr != null;
      }

      public Entry<String,intqueue> next() {
         if (curr == null) throw new NoSuchElementException();
         node next = curr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }

   }

}
