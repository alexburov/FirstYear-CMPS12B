// $Id: treemap.java,v 1.3 2015-02-15 17:19:49-08 - - $
// Author Olexiy Burov, oburov@ucsc.edu

class treemap {

   class tree {
      String key;
      String value;
      tree left;
      tree right;

      // Constructor to conviniently initialize tree nodes.
      public tree(String key, String value) {
         this.key = key;
         this.value = value;
      }
   }

   tree root = null;


   // public get, uses private recursive helper
   // function to do the search on the tree.
   public String get(String key) {
      tree result = get(root,key);
      if (result != null) {
         return result.value;
      } else return null;
   }


   // Recursive get helper traverse the tree
   // until the key was not found or hit null
   private tree get(tree tree_root, String key) {
      if (tree_root == null) return null;
      tree cursor = tree_root;

      int compare = key.compareTo(cursor.key);

      if (compare == 0) return cursor;
      else if (compare < 0)
         return get(cursor.left, key);
      else
         return get(cursor.right, key);
   }

   // Puts new element into the tree.
   public String put(String key, String value) {
      // If the tree is empty create new node for the root
      // and put data inside. After that return the value.
      if (root == null) {
         root = new tree(key, value);
         return value;
      }

      tree cursor = root;

      // Traverses the tree until it creates new node or replaces
      // the existing one with new data.
      while (true) {
         int cmp = key.compareTo(cursor.key);

         if (cmp == 0) {
            String previous_value = cursor.value;
            cursor.value = value;
            // Replaces the old value in the node with a new one,
            // returning the old one.
            return previous_value;
         } else if (cmp < 0) { // If key is smaller, go left.
            if (cursor.left == null) { // If there left is null
               // Create new node for left and return the new value.
               cursor.left = new tree(key, value);
               return value;
            } else cursor = cursor.left; // Go left if it's not null.
         } else { // If key is not smaller, go right.
            if (cursor.right == null) { // If right is null
               // Create new node for right and return the new value.
               cursor.right = new tree(key, value);
               return value;
            } else cursor = cursor.right; // Go right if it's not null.
         }
      }
   }


   // Prints tree in debug format("-d" option)
   public void debug_tree() {
      debug_tree_recur(root, 0);
   }

   // Recursively prints tree in debug format
   // Helper function.
   private void debug_tree_recur(tree root, int depth) {
      if (root == null) return;
      debug_tree_recur(root.left,depth + 1);
      System.out.printf("%3d \"%s\" \"%s\" %s %s%n",
         depth,root.key,root.value,root.left,root.right);
      debug_tree_recur(root.right, depth + 1);
   }
}
