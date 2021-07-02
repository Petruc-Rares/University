package comparatorlists;

import powergridentities.Producer;

import java.util.Comparator;

/**
 * Sorts the price list according to the task
 */
public final class ComparatorPriceList implements Comparator<Producer> {
    @Override
    public int compare(Producer o1, Producer o2) {
        return ComparePriceQuantity.compare(o1, o2);
    }
}
