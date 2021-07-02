package fileio;

/**
 * This class contains information about the distributor
 * according to the json file (name related)
 */
public final class DistributorInputData {
    /**
     * The id of the distributor
     */
    private int id;
    /**
     * The length of the contract adopted by the distributor
     */
    private int contractLength;
    /**
     * The initial budget of the distributor
     */
    private int initialBudget;
    /**
     * The cost of the infrastructure initially
     */
    private int initialInfrastructureCost;
    /**
     * The amount of energy needed by the distributor
     * (in KW)
     */
    private int energyNeededKW;
    /**
     * The strategy the distributor adopts when selecting
     * the producer
     */
    private String producerStrategy;

    public DistributorInputData() {
    }

    public DistributorInputData(int id, int contractLength, int initialBudget,
                                int initialInfrastructureCost, int energyNeededKW,
                                String producerStrategy) {
        this.id = id;
        this.contractLength = contractLength;
        this.initialBudget = initialBudget;
        this.initialInfrastructureCost = initialInfrastructureCost;
        this.energyNeededKW = energyNeededKW;
        this.producerStrategy = producerStrategy;
    }

    public int getId() {
        return id;
    }

    public void setId(final int id) {
        this.id = id;
    }

    public int getContractLength() {
        return contractLength;
    }

    public void setContractLength(final int contractLength) {
        this.contractLength = contractLength;
    }

    public int getInitialBudget() {
        return initialBudget;
    }

    public void setInitialBudget(final int initialBudget) {
        this.initialBudget = initialBudget;
    }

    public int getInitialInfrastructureCost() {
        return initialInfrastructureCost;
    }

    public void setInitialInfrastructureCost(final int initialInfrastructureCost) {
        this.initialInfrastructureCost = initialInfrastructureCost;
    }

    public int getEnergyNeededKW() {
        return energyNeededKW;
    }

    public void setEnergyNeededKW(int energyNeededKW) {
        this.energyNeededKW = energyNeededKW;
    }

    public String getProducerStrategy() {
        return producerStrategy;
    }

    public void setProducerStrategy(String producerStrategy) {
        this.producerStrategy = producerStrategy;
    }
}
