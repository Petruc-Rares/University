package comparatorlists;

import powergridentities.Producer;

/**
 * This class has a method that gives the result
 * of comparision between two producers considering their
 * prices and quantities for the electricity
 */
public final class ComparePriceQuantity {

    private ComparePriceQuantity() {

    }

    static int compare(Producer o1, Producer o2) {
        double o1Price = o1.getPriceKW();
        double o2Price = o2.getPriceKW();

        double diffPrice = o1Price - o2Price;
        if (diffPrice != 0) {
            return (diffPrice > 0) ? 1 : -1;
        } else {
            // o1Price == o2Price, so compare by quantity
            // stil don't know if it is total quantity or per distributor
            int o1Quantity = o1.getEnergyPerDistributor();
            int o2Quantity = o2.getEnergyPerDistributor();

            int diffQuantity = o1Quantity - o2Quantity;
            if (diffQuantity != 0) {
                return -diffQuantity;
            } else {
                return (o1.getId() - o2.getId());
            }
        }
    }
}
