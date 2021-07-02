package powergridentities;

import fileio.HistoryProducerOutput;

import java.util.ArrayList;
import java.util.List;

public final class Producer {
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
     * The current number of distributors the producer
     * provides electricity to
     */
    private int crtNoDistributors;
    /**
     * The price for a kw
     */
    private double priceKW;
    /**
     * The energy the producer provides for a distributor
     */
    private int energyPerDistributor;
    /**
     * The monthly distributors this producer has
     */
    private List<HistoryProducerOutput> monthlyHistory = new ArrayList<>();

    public Producer(int id, String energyType, int maxDistributors, double priceKW,
                             int energyPerDistributor, int numberOfTurns) {
        this.id = id;
        this.energyType = energyType;
        this.maxDistributors = maxDistributors;
        this.priceKW = priceKW;
        this.energyPerDistributor = energyPerDistributor;

        monthlyHistory.add(new HistoryProducerOutput(0, new ArrayList<>()));
    }

    public Producer(int id, String energyType, int maxDistributors, int crtNoDistributors,
                    int priceKW,
                    int energyPerDistributor,
                    List<HistoryProducerOutput> monthlyHistory) {
        this.id = id;
        this.energyType = energyType;
        this.maxDistributors = maxDistributors;
        this.crtNoDistributors = crtNoDistributors;
        this.priceKW = priceKW;
        this.energyPerDistributor = energyPerDistributor;
        this.monthlyHistory = monthlyHistory;
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

    public int getCrtNoDistributors() {
        return crtNoDistributors;
    }

    public void setCrtNoDistributors(int crtNoDistributors) {
        this.crtNoDistributors = crtNoDistributors;
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

    public List<HistoryProducerOutput> getMonthlyHistory() {
        return monthlyHistory;
    }

    public void setMonthlyHistory(List<HistoryProducerOutput> monthlyHistory) {
        this.monthlyHistory = monthlyHistory;
    }
}
