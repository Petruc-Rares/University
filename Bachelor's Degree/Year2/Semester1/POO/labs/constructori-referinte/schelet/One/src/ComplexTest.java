public class ComplexTest {
    public static void main(String[] args) {
        Complex no1 = new Complex();
        Complex no2 = new Complex(3, 2);
        Complex no3 = new Complex();

        no3.addWithComplex(no2);
        no3.addWithComplex(no2);
        no3.addWithComplex(no3);

        no1.showNumber();
        no2.showNumber();
        no3.showNumber();
    }

}
