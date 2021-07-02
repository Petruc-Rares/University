package solution;

import java.util.Comparator;

public class SortDescNo implements Comparator<NameNumber> {
    /**
     * compare descending two NameNumbers @param a, @param b based on their number
     * and in case of equality compare by name
     */
    public int compare(final NameNumber a, final NameNumber b) {
        double diffAvg = b.getNumber() - a.getNumber();
        if (diffAvg == 0) {
            return b.getName().compareTo(a.getName());
        } else {
            if (diffAvg > 0) {
                return 1;
            }
            return -1;
        }
    }
}
