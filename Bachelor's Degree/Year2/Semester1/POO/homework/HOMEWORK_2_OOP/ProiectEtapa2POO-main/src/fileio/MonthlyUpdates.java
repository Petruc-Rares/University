package fileio;

import java.util.List;

/**
 * This class contains information about the new rounds of the game
 */
public final class MonthlyUpdates {
    /**
    * List containing info about the new customers
    */
    private List<ConsumerInputData> newConsumers;
    /**
     * List containing info about the distributors' new prices
     */
    private List<NewDistributorPrices> distributorChanges;
    /**
     * List containing info about the producers' changes
     * related to the energy per distributor
     */
    private List<UpdateProducer> producerChanges;

    public MonthlyUpdates() {
    }

    public MonthlyUpdates(List<ConsumerInputData> newConsumers,
                          List<NewDistributorPrices> distributorChanges,
                          List<UpdateProducer> producerChanges) {
        this.newConsumers = newConsumers;
        this.distributorChanges = distributorChanges;
        this.producerChanges = producerChanges;
    }

    public List<ConsumerInputData> getNewConsumers() {
        return newConsumers;
    }


    public void setNewConsumers(final List<ConsumerInputData> newConsumers) {
        this.newConsumers = newConsumers;
    }

    public List<NewDistributorPrices> getDistributorChanges() {
        return distributorChanges;
    }

    public void setDistributorChanges(final List<NewDistributorPrices> costsChanges) {
        this.distributorChanges = costsChanges;
    }

    public List<UpdateProducer> getProducerChanges() {
        return producerChanges;
    }

    public void setProducerChanges(List<UpdateProducer> producerChanges) {
        this.producerChanges = producerChanges;
    }
}
