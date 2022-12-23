
public class MineFieldTester {

   public static void main(String[] args) {
      // TODO Auto-generated method stub      
      boolean[][] mineData = new boolean[9][9];
      mineData[4][2] = true;
      mineData[3][8] = true;
      mineData[0][0] = true;
      mineData[2][2] = true;
      mineData[8][8] = true;
      MineField mineField = new MineField(mineData);
      System.out.println("=====Testing minedata minefield init=====");
      System.out.println(mineField.mines[0].getLocation().toString());
      System.out.println(mineField.mines[1].getLocation().toString());  
      System.out.println(mineField.mines[2].getLocation().toString());  
      System.out.println(mineField.mines[3].getLocation().toString());  
      
      System.out.println("Testing hashset");
      System.out.println(mineField.hashset.toString());
      
      System.out.println("Testing inRange");
      System.out.println("[exp: false] " + mineField.inRange(11, 11)); // false
      System.out.println("[exp: true] " + mineField.inRange(3, 1)); // true
      
      System.out.println("Testing hasmine");
      System.out.println("[exp: true] " + mineField.hasMine(4, 2));
      System.out.println("[exp: false] "+ mineField.hasMine(2, 4));
      
      System.out.println("Testing Adjacent");
      System.out.println("[exp:1]" + mineField.numAdjacentMines(5, 2));
      System.out.println("[exp:2]" + mineField.numAdjacentMines(1, 1));
      System.out.println("[exp:1]" + mineField.numAdjacentMines(1, 0));
      System.out.println("[exp:1]" + mineField.numAdjacentMines(7, 8));
      System.out.println("[exp:0]" + mineField.numAdjacentMines(5, 5));
      
      System.out.println("Testing reset");
      mineField.resetEmpty();
      System.out.println("[exp: true] " + (mineField.mines[0] == null)); 
      System.out.println(mineField.hashset.toString());
      
      
      System.out.println();
      mineField = new MineField(9, 9, 10);
      mineField.populateMineField(3, 4);
      System.out.println("=====Testing random minefield init=====");
      for (int i = 0; i < mineField.numMines; i++) {
         System.out.println(mineField.mines[i].getLocation().toString()); 
      }
      System.out.println("Testing hashset");
      System.out.println(mineField.hashset.toString());
      
      System.out.println("Testing inRange");
      System.out.println("[exp: false] " + mineField.inRange(11, 11)); // false
      System.out.println("[exp: true] " + mineField.inRange(3, 1)); // true
      
      System.out.println("Testing hasmine");
      System.out.println(mineField.hasMine(4, 2));
      System.out.println(mineField.hasMine(2, 4));
      
      System.out.println("Testing reset");
      mineField.resetEmpty();
      System.out.println("[exp: true] " + (mineField.mines[0] == null)); 
      System.out.println(mineField.hashset.toString());
      // Testing
   }

}
