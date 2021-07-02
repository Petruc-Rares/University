package comparatorlists;

import entities.EnergyType;
import powergridentities.Producer;

import java.util.Comparator;


/**
 * Sorts the green list according to the task
 */
public final class ComparatorGreenList implements Comparator<Producer> {

    @Override
    public int compare(Producer o1, Producer o2) {
        boolean o1Renewable = EnergyType.valueOf(o1.getEnergyType()).isRenewable();
        boolean o2Renewable = EnergyType.valueOf(o2.getEnergyType()).isRenewable();

        // first check by renewable
        if ((o1Renewable) && (!o2Renewable)) {
            // o1 should be at the beginning
            return -1;
        } else if ((!o1Renewable) && (o2Renewable)) {
            // o1 should be at the end
            return 1;
        } else {
            // o1Renewable == o2Renewable
            return ComparePriceQuantity.compare(o1, o2);
        }
    }
}
