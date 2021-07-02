package fileio;

/**
 * This class contains information about the updates to
 * a producer's information
 */
public final class UpdateProducer {
    /**
     * The id of the producer, that have been made changes for
     */
    private int id;
    /**
     * The new amount of energy that the producer provides
     * to each distributor
     */
    private int energyPerDistributor;

    public UpdateProducer() {

    }

    public UpdateProducer(int id, int energyPerDistributor) {
        this.id = id;
        this.energyPerDistributor = energyPerDistributor;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getEnergyPerDistributor() {
        return energyPerDistributor;
    }

    public void setEnergyPerDistributor(int energyPerDistributor) {
        this.energyPerDistributor = energyPerDistributor;
    }
}
