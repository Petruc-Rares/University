
import java.util.Collection;

public class Generic {
    public static <T extends Sumabil> T sumAll(Collection<T> collection) {

        Constants constantClass = Constants.stringToConstant(collection.iterator().next().getClass().toString());

            int matrix[][] = new int[4][4];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    matrix[i][j] = 0;
                }
            }

        T sum =  (T) switch (constantClass) {
            case MY_VECTOR3 -> new MyVector3(0, 0, 0);

            case MY_MATRIX -> new MyMatrix(matrix);
        };

        for (T elem: collection) {
            sum.addValue(elem);
        }

        return sum;

    }
}