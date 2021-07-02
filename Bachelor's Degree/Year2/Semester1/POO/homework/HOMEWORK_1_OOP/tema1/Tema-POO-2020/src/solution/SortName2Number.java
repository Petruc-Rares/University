package solution;

import java.util.Comparator;

public final class SortName2Number implements Comparator<Name2Number> {
    /**
     * compare two Name2Number @param a, @param b based on the first number
     * and in case of equality, compare them based on the second number
     */
    public int compare(final Name2Number a, final Name2Number b) {
        Double diffAvg = b.getNumber1() - a.getNumber1();
        if (diffAvg == 0) {
            // we want to keep it sorted by the second most important number
            // in the class
            return a.getNumber2().compareTo(b.getNumber2());
        } else {
            if (diffAvg > 0) {
                return 1;
            }
            return -1;
        }
    }
}
