package fileio;

/**
 * This class contains information about the updates
 * regarding the prices of a specific distributor
 */
public final class NewDistributorPrices {
    /**
     * The id of the distributor
     */
    private int id;
    /**
     * The updated infrastructure cost
     */
    private int infrastructureCost;
    /**
     * The updated production cost
     */
    private int productionCost;

    public NewDistributorPrices() {

    }

    public NewDistributorPrices(final int id, final int infrastructureCost,
                                final int productionCost) {
        this.id = id;
        this.infrastructureCost = infrastructureCost;
        this.productionCost = productionCost;
    }

    public int getId() {
        return id;
    }

    public void setId(final int id) {
        this.id = id;
    }

    public int getInfrastructureCost() {
        return infrastructureCost;
    }

    public void setInfrastructureCost(final int infrastructureCost) {
        this.infrastructureCost = infrastructureCost;
    }

    public int getProductionCost() {
        return productionCost;
    }

    public void setProductionCost(final int productionCost) {
        this.productionCost = productionCost;
    }
}
