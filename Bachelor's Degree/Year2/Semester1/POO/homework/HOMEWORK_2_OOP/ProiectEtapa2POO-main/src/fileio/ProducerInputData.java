package fileio;

public final class ProducerInputData {
    /**
     * The id of the producer
     */
    private int id;
    /**
     * The type of energy that the producer delivers
     */
    private String energyType;
    /**
     * The maximum number of distributors the producer
     * can provide electricity
     */
    private int maxDistributors;
    /**
     * The price for a kw
     */
    private double priceKW;
    /**
     * The energy the producer provides for a distributor
     */
    private int energyPerDistributor;

    public ProducerInputData() {
    }

    public ProducerInputData(int id, String energyType, int maxDistributors, double priceKW,
                             int energyPerDistributor) {
        this.id = id;
        this.energyType = energyType;
        this.maxDistributors = maxDistributors;
        this.priceKW = priceKW;
        this.energyPerDistributor = energyPerDistributor;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getEnergyType() {
        return energyType;
    }

    public void setEnergyType(String energyType) {
        this.energyType = energyType;
    }

    public int getMaxDistributors() {
        return maxDistributors;
    }

    public void setMaxDistributors(int maxDistributors) {
        this.maxDistributors = maxDistributors;
    }

    public double getPriceKW() {
        return priceKW;
    }

    public void setPriceKW(double priceKW) {
        this.priceKW = priceKW;
    }

    public int getEnergyPerDistributor() {
        return energyPerDistributor;
    }

    public void setEnergyPerDistributor(int energyPerDistributor) {
        this.energyPerDistributor = energyPerDistributor;
    }
}
