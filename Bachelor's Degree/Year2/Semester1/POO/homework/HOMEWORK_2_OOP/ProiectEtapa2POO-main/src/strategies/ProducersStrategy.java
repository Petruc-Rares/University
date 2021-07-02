package strategies;

import powergridentities.Distributor;
import producerslists.ProducersLists;

/**
 * interface that has a method for applying a strategy
 * for the lists that are present in producersLists
 */
public interface ProducersStrategy {
    /**
     * applies the strategy for the lists
     * present in the
     * @param producersLists
     * for a given
     * @param distributor
     */
    void applyStrategy(Distributor distributor, ProducersLists producersLists);
}
