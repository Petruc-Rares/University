package producerupdates;

import powergridentities.Distributor;
import powergridentities.Producer;

import java.util.List;
import java.util.Observable;

/**
 * This class will contain a producer along with it's distributors
 */
public final class ProducersDistributors extends Observable {
    /**
     * The current producer
     */
    private Producer producer;

    public ProducersDistributors(Producer producer) {
        this.producer = producer;
    }

    /**
     * method that is used to notify the distributors
     * of a certain producer in ascending order after their id
     */
    public void notifyDistributors() {
        setChanged();
        notifyObservers(producer);
        clearChanged();
    }

    /**
     * method that establishes the relationships between producer
     * and the distributors that are put in reverse order because
     * we want them to be notified of changes in ascending order of their id
     * @param crtProducer represents the current producer
     * @param allRelationships represents the list of relationships between
     *                         all producers and the distributors
     * @param distributors represents the list of distributors built using the input
     */
    public static void buildListRelationships(Producer crtProducer,
                                       List<ProducersDistributors> allRelationships,
                                       List<Distributor> distributors) {
        ProducersDistributors producerDistributors = new ProducersDistributors(crtProducer);

        // add the observers in reverse order, for distributors
        // to be notified in ascending order of their ids
        // when changes that affect them are made
        for (int i = distributors.size() - 1; i >= 0; i--) {
            producerDistributors.addObserver(distributors.get(i));
        }

        allRelationships.add(producerDistributors);
    }

    public Producer getProducer() {
        return producer;
    }

    public void setProducer(Producer producer) {
        this.producer = producer;
    }
}
