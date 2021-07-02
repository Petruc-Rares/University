public class PointTest {
    public static void main(String[] args) {
        Point point1 = new Point(5.2f, 10.3f);

        // inainte de schimbarea coordonatelor
        point1.printCoords();

        point1.changeCoords(3.8f, 9.421f);

        // dupa schimbarea coordonatelor
        point1.printCoords();
    }
}
