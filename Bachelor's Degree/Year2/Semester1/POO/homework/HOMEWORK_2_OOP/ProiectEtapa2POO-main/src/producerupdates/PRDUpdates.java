package producerupdates;

import powergridentities.Producer;
import producerslists.ProducersLists;

import java.util.List;

/**
 * This class updates a producer's amount of energy
 * and appeals other class that will find the best producer
 * for the the distributors that had energy from the current producer
 */
public final class PRDUpdates {
    /**
     * A list of producers that will notify distributors
     * to look for another producer once the update took place
     */
    private List<ProducersDistributors> producersDistributors;
    /**
     * Contains the lists of producers sorted according to the task
     */
    private ProducersLists producersLists;

    public PRDUpdates(List<ProducersDistributors> producersDistributors,
                      ProducersLists producersLists) {
        this.producersDistributors = producersDistributors;
        this.producersLists = producersLists;
    }

    /**
     * given the id of the producer, this method selects
     * its distributors and notifies them to change their
     * flag for searching producers again to true
     * @param id represents the id of the current producer
     *           that changes have been made for
     */
    public void reassignDistributors(int id) {

        Producer producer = null;
        ProducersDistributors relationship = null;

        // now, notify all distributors that updates have been made and only for those
        // who had energy from the updated producer, reapply the strategy
        for (ProducersDistributors producerDistributor: producersDistributors) {
            if (producerDistributor.getProducer().getId() == id) {
                relationship = producerDistributor;
                break;
            }
        }

        // when that quantity changes, lists should be reordered
        producersLists.sortLists();
        relationship.notifyDistributors();
    }
}
