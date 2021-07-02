public class Complex {
    private int real;
    private int imaginary;

    // prima bulinuta
    public Complex(int real, int imaginary) {
        this.real = real;
        this.imaginary = imaginary;
    }

    public Complex() {
        this(0, 0);
    }

    public Complex(Complex no) {
        this.real = no.real;
        this.imaginary = no.imaginary;
    }

    // a doua bulinuta
    public int getReal() {
        return real;
    }

    public int getImaginary() {
        return imaginary;
    }

    public void setReal(int real) {
        this.real = real;
    }

    public void setImaginary(int imaginary) {
        this.imaginary = imaginary;
    }

    // a treia bulinuta
    public void addWithComplex(Complex no) {
        this.real += no.real;
        this.imaginary += no.imaginary;
    }

    // a patra bulinuta
    public void showNumber() {
        System.out.println(this.real + " " + "+" + " " +this.imaginary + "i");
    }
}
