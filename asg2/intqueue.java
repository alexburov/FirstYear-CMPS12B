// $Id: intqueue.java,v 1.4 2013-10-16 17:10:32-07 - - $
// Author: Olexiy Burov, oburov@ucsc.edu

import java.util.Iterator;
import java.util.NoSuchElementException;

class intqueue implements Iterable<Integer> {

   private class node {
      int linenr;
      node link;

      private node(int number) {
         this.linenr = number;
      }

      private void add_node_after(int number) {
         link = new node(number);
      }
   }
   private int count = 0;
   private node front = null;
   private node rear = null;


   // Insert method for the linked queue.
   public void insert (int number) {
      // Case 1: Empty
      if (empty()) {
         // Make new node and set head and tail point at that node.
         front = new node(number);
         rear = front;
      } else {  // Case 2: Non-empty queue
         // Add node after the tail and make it a new tail.
         rear.add_node_after(number);
         rear = rear.link;
      }

      // Increment the count.
      ++count;
   }

   // Tells whether the queue is empty.
   public boolean empty() {
      return count == 0;
   }

   // Gets the number of times word appeared in the file.
   public int getcount() {
      return count;
   }


   // Iterator implementation.
   public Iterator<Integer> iterator() {
      return new iterator();
   }

   private class iterator implements Iterator<Integer> {
      node curr = front;

      public boolean hasNext() {
         return curr != null;
      }

      public Integer next() {
         if (curr == null) throw new NoSuchElementException();
         Integer next = curr.linenr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }
   }

}
