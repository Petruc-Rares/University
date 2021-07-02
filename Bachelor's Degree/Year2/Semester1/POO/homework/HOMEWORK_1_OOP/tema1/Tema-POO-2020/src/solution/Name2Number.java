package solution;

public final class Name2Number extends NameNumber {
    private final Double number2;

    public Name2Number(final String name, final Double number1,
                                final Double number2) {
        super(name, number1);
        this.number2 = number2;
    }

    public Double getNumber1() {
        return super.getNumber();
    }

    public Double getNumber2() {
        return number2;
    }

    @Override
    public String toString() {
        return "SerialRatingPosition{"
                + "nameFilm = " + this.getName() + '\''
                + ", first number as importance = " + this.getNumber1() + '\''
                + ", second number as importance =" + this.getNumber2()
                + '}';
    }
}
