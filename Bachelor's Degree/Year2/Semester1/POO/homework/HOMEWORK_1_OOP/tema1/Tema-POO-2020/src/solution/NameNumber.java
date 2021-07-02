package solution;

public class NameNumber {
    private final String name;
    private final Double number;

    public NameNumber(final String name, final Double number) {
        this.name = name;
        this.number = number;
    }


    /**
     * In this class we are interested of the name
     * @return the name
     */
    public String toString() {
        return name;
    }

    public final Double getNumber() {
        return number;
    }

    public final String getName() {
        return name;
    }
}
