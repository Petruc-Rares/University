package reformattedfileio;

import powergridentities.Consumer;
import powergridentities.Distributor;
import powergridentities.Producer;

import java.util.List;

/**
 * This class holds the the information
 * about the reformatted data
 */
public final class ReformattedData {
    /**
     * The consumers in the reformatted data
     */
    private List<Consumer> consumers;
    /**
     * The distributors in the reformatted data
     */
    private List<Distributor> distributors;
    /**
     * The producers in the reformatted data
     */
    private List<Producer> producers;

    public ReformattedData(final List<Consumer> consumers,
                           final List<Distributor> distributors,
                           final List<Producer> producers) {
        this.consumers = consumers;
        this.distributors = distributors;
        this.producers = producers;
    }

    public List<Consumer> getConsumers() {
        return consumers;
    }

    public void setConsumers(final List<Consumer> consumers) {
        this.consumers = consumers;
    }

    public List<Distributor> getDistributors() {
        return distributors;
    }

    public void setDistributors(final List<Distributor> distributors) {
        this.distributors = distributors;
    }

    public List<Producer> getProducers() {
        return producers;
    }

    public void setProducers(List<Producer> producers) {
        this.producers = producers;
    }
}
