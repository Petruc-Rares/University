public class PolygonTest {
    public static void main(String[] args) {

        float[] arrayCoordinates = {3.1f, 2.5f, 6.1f, 1.61f, 4.32f, 5.41f};
        Polygon triunghi = new Polygon(arrayCoordinates);

        triunghi.ShowPoints();
    }
}
