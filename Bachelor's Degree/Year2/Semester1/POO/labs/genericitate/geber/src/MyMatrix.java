//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

public class MyMatrix implements Sumabil {
    private int[][] matrix;

    public MyMatrix(int[][] matrix) {
        this.matrix = matrix;
    }

    public void addValue(Sumabil value) {
        if (Constants.stringToConstant(value.getClass().toString()) != Constants.MY_MATRIX) {
            System.out.println("can't add value");
        } else {
            MyMatrix copyValue = (MyMatrix)value;
            int[][] toAddMatrix = copyValue.getMatrix();

            for(int i = 0; i < 4; ++i) {
                for(int j = 0; j < 4; ++j) {
                    int[] var10000 = this.matrix[i];
                    var10000[j] += toAddMatrix[i][j];
                }
            }

        }
    }

    public int[][] getMatrix() {
        return this.matrix;
    }

    public void printMatrix() {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                System.out.print(this.matrix[i][j] + " ");
            }

            System.out.print("\n");
        }

    }
}
