
public class VisibleFieldTester {

   public static void main(String[] args) {
      // TODO Auto-generated method stub
      boolean[][] mineData = new boolean[8][9];
      mineData[4][2] = true;
      mineData[3][8] = true;
      mineData[0][0] = true;
      mineData[2][2] = true;
      //mineData[8][8] = true;
      MineField mineField = new MineField(mineData);
      
      VisibleField visibleField = new VisibleField(mineField);
   }

}
