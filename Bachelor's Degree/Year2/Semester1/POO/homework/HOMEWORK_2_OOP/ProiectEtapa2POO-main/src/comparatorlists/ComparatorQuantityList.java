package comparatorlists;

import powergridentities.Producer;

import java.util.Comparator;

/**
 * Sorts the quantity list according to the task
 */
public final class ComparatorQuantityList implements Comparator<Producer> {

    @Override
    public int compare(Producer o1, Producer o2) {
        int o1Energy = o1.getEnergyPerDistributor();
        int o2Energy = o2.getEnergyPerDistributor();

        int diffEnergy = o1Energy - o2Energy;
        if (diffEnergy != 0) {
            return -diffEnergy;
        } else {
            return (o1.getId() - o2.getId());
        }
    }
}
