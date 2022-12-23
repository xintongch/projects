// Name: Ting Fung Lam
// USC NetID: tingfunl
// CS 455 PA3
// Fall 2019

import java.awt.*;
import java.util.*;

/** 
   MineField
      class with locations of mines for a game.
      This class is mutable, because we sometimes need to change it once it's created.
      mutators: populateMineField, resetEmpty
      includes convenience method to tell the number of mines adjacent to a location.
 */
public class MineField {
   
   // <put instance variables here>
   Point[] mines;
   int numMines;
   private int numRows;
   private int numCols;
   Set<Point> hashset;
   private static final int ADJACENT = 1;
   private static final int ONE_MINE = 1; // for counting adjacent mines
   
   
   /**
      Create a minefield with same dimensions as the given array, and populate it with the mines in the array
      such that if mineData[row][col] is true, then hasMine(row,col) will be true and vice versa.  numMines() for
      this minefield will corresponds to the number of 'true' values in mineData.
    * @param mineData  the data for the mines; must have at least one row and one col.
    */
   public MineField(boolean[][] mineData) {
      numRows = mineData.length;
      numCols = mineData[0].length;
      // get number of mines
      for (int i = 0; i < numRows; i++) {
         for (int j = 0; j < numCols; j++) {
            if (mineData[i][j]) {
               numMines++;
            }
         }
      }
      mines = new Point[numMines];
      // Assign the mines
      int k = 0;
      for (int i = 0; i < numRows; i++) {
         for (int j = 0; j < numCols; j++) {
            if (mineData[i][j]) {
               mines[k++] = new Point(i, j);
            }            
         }
      }
      hashset = hash(mines);
   }
   
   
   /**
      Create an empty minefield (i.e. no mines anywhere), that may later have numMines mines (once 
      populateMineField is called on this object).  Until populateMineField is called on such a MineField, 
      numMines() will not correspond to the number of mines currently in the MineField.
      @param numRows  number of rows this minefield will have, must be positive
      @param numCols  number of columns this minefield will have, must be positive
      @param numMines   number of mines this minefield will have,  once we populate it.
      PRE: numRows > 0 and numCols > 0 and 0 <= numMines < (1/3 of total number of field locations). 
    */
   public MineField(int numRows, int numCols, int numMines) {
      this.numMines = numMines;
      this.numRows = numRows;
      this.numCols = numCols;
      mines = new Point[numMines];
   }
   

   /**
      Removes any current mines on the minefield, and puts numMines() mines in random locations on the minefield,
      ensuring that no mine is placed at (row, col).
      @param row the row of the location to avoid placing a mine
      @param col the column of the location to avoid placing a mine
      PRE: inRange(row, col)
    */
   public void populateMineField(int row, int col) {
      Random random = new Random();
      // Assign new mines
      for (int i = 0; i < numMines; i++) {
         int x;
         int y;
         Point temp = new Point();
         boolean valid = false;
         // Loop until new mine location is valid
         while (!valid) {
            x = random.nextInt(numCols);
            y = random.nextInt(numRows);
            temp.setLocation(x, y);
            valid = temp.x != row || temp.y != col;
            for (int j = 0; j < i; j++) {
               if (mines[j].equals(temp)) {
                  valid = false;
               }
            }
         }
         mines[i] = new Point(temp);
      }
      hashset = hash(mines);
   }
   
   
   /**
      Reset the minefield to all empty squares.  This does not affect numMines(), numRows() or numCols()
      Thus, after this call, the actual number of mines in the minefield does not match numMines().  
      Note: This is the state the minefield is in at the beginning of a game.
    */
   public void resetEmpty() {
      mines = new Point[numMines]; // Create new point array to mines, auto garbage collection
      hashset.clear();
   }

   
  /**
     Returns the number of mines adjacent to the specified mine location (not counting a possible 
     mine at (row, col) itself).
     Diagonals are also considered adjacent, so the return value will be in the range [0,8]
     @param row  row of the location to check
     @param col  column of the location to check
     @return  the number of mines adjacent to the square at (row, col)
     PRE: inRange(row, col)
   */
   public int numAdjacentMines(int row, int col) {
      int adjacentMines = 0;
      adjacentMines += numAdjacentMinesHelper(row + ADJACENT, col);
      adjacentMines += numAdjacentMinesHelper(row - ADJACENT, col);
      adjacentMines += numAdjacentMinesHelper(row, col + ADJACENT);
      adjacentMines += numAdjacentMinesHelper(row, col - ADJACENT);
      adjacentMines += numAdjacentMinesHelper(row + ADJACENT, col + ADJACENT);
      adjacentMines += numAdjacentMinesHelper(row + ADJACENT, col - ADJACENT);
      adjacentMines += numAdjacentMinesHelper(row - ADJACENT, col - ADJACENT);
      adjacentMines += numAdjacentMinesHelper(row - ADJACENT, col + ADJACENT);
      return adjacentMines;
   }
   
   /**
      Returns true iff (row,col) is a valid field location.  Row numbers and column numbers
      start from 0.
      @param row  row of the location to consider
      @param col  column of the location to consider
      @return whether (row, col) is a valid field location
   */
   public boolean inRange(int row, int col) {
      return row >= 0 && row < numRows && col >= 0 && col < numCols;
   }
   
   
   /**
      Returns the number of rows in the field.
      @return number of rows in the field
   */  
   public int numRows() {
      return numRows;
   }
   
   
   /**
      Returns the number of columns in the field.
      @return number of columns in the field
   */    
   public int numCols() {
      return numCols;
   }
   
   
   /**
      Returns whether there is a mine in this square
      @param row  row of the location to check
      @param col  column of the location to check
      @return whether there is a mine in this square
      PRE: inRange(row, col)   
   */    
   public boolean hasMine(int row, int col) {
      return hashset.contains(new Point(row, col));
   }
   
   
   /**
      Returns the number of mines you can have in this minefield.  For mines created with the 3-arg constructor,
      some of the time this value does not match the actual number of mines currently on the field.  See doc for that
      constructor, resetEmpty, and populateMineField for more details.
    * @return
    */
   public int numMines() {
      return numMines;
   }

   
   // <put private methods here>
   /**
    * Returns the hashset of mines used for hasMine method
    * @param mines  the array of point objects
    * @return hashset  the hash set
    */
   private Set<Point> hash (Point[] mines) {
      Set<Point> hashset = new HashSet<>();
      for (int i = 0; i < numMines; i++) {
         hashset.add(new Point(mines[i].x, mines[i].y));
      }
      return hashset;
   }
   
   /**
    * Helper function for numAdjacentMines
    * @param row  row of the location to check
    * @param col  column of the location to check
    * @return  1 if has mine, 0 otherwise
    */
   private int numAdjacentMinesHelper(int row, int col) {
      if (hasMine(row, col) && inRange(row, col)) {
         return ONE_MINE;
      }
      return 0;
   }

}

