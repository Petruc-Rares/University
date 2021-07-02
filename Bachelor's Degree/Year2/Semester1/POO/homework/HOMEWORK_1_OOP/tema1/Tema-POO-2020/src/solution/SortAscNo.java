package solution;

import java.util.Comparator;

public class SortAscNo implements Comparator<NameNumber> {
    /**
     * compare ascending two NameNumbers @param a, @param b based on their number
     * and in case of equality compare by name
     */
    public int compare(final NameNumber a, final NameNumber b) {
        double diffAvg = a.getNumber() - b.getNumber();
        if (diffAvg == 0) {
            return a.getName().compareTo(b.getName());
        } else {
            if (diffAvg > 0) {
                return 1;
            }
            return -1;
        }
    }
}
