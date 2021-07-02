public class Polygon {
    private Point[] pointArray;
    public int nrEdges;

    public Polygon(int n) {
        this.pointArray = new Point[n];
        this.nrEdges = n;
    }

    public Polygon(float array[]) {
        this(array.length/2);
        for (int i = 0; i < nrEdges*2; i+=2) {
            this.pointArray[i/2] = new Point(array[i], array[i+1]);
        }
    }

    public void ShowPoints() {
        for (int i = 0; i < nrEdges; i++) {
            pointArray[i].printCoords();
        }
    }
}
