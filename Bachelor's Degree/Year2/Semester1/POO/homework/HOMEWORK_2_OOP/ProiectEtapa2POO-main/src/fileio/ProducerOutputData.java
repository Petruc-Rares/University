package fileio;

import java.util.List;

public final class ProducerOutputData {
    private int id;
    private int maxDistributors;
    private double priceKW;
    private String energyType;
    private int energyPerDistributor;
    private List<HistoryProducerOutput> monthlyStats;

    public ProducerOutputData() {

    }

    public ProducerOutputData(int id, int maxDistributors, double priceKW, String energyType,
                              int energyPerDistributor,
                              List<HistoryProducerOutput> monthlyStats) {
        this.id = id;
        this.maxDistributors = maxDistributors;
        this.priceKW = priceKW;
        this.energyType = energyType;
        this.energyPerDistributor = energyPerDistributor;
        this.monthlyStats = monthlyStats;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
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

    public String getEnergyType() {
        return energyType;
    }

    public void setEnergyType(String energyType) {
        this.energyType = energyType;
    }

    public int getEnergyPerDistributor() {
        return energyPerDistributor;
    }

    public void setEnergyPerDistributor(int energyPerDistributor) {
        this.energyPerDistributor = energyPerDistributor;
    }

    public List<HistoryProducerOutput> getMonthlyStats() {
        return monthlyStats;
    }

    public void setMonthlyStats(List<HistoryProducerOutput> monthlyStats) {
        this.monthlyStats = monthlyStats;
    }
}
