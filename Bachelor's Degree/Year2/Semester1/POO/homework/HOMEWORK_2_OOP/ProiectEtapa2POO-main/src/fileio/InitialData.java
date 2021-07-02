package fileio;

import java.util.List;

/**
 * This class holds the the information
 * about the initial data with accordingly
 * name correspondence
 */
public final class InitialData {
    /**
     * The consumers in the initialData
     */
    private List<ConsumerInputData> consumers;
    /**
     * The distributors in the initialData
     */
    private List<DistributorInputData> distributors;
    /**
     * The producers in the initialData
     */
    private List<ProducerInputData> producers;

    public InitialData() {
    }

    public InitialData(List<ConsumerInputData> consumers,
                       List<DistributorInputData> distributors,
                       List<ProducerInputData> producers) {
        this.consumers = consumers;
        this.distributors = distributors;
        this.producers = producers;
    }

    public void setConsumers(final List<ConsumerInputData> consumers) {
        this.consumers = consumers;
    }

    public List<ConsumerInputData> getConsumers() {
        return consumers;
    }

    public void setDistributors(final List<DistributorInputData> distributors) {
        this.distributors = distributors;
    }

    public List<DistributorInputData> getDistributors() {
        return distributors;
    }

    public List<ProducerInputData> getProducers() {
        return producers;
    }

    public void setProducers(List<ProducerInputData> producers) {
        this.producers = producers;
    }
}
