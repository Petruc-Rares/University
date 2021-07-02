package fileio;

import java.util.List;

public final class HistoryProducerOutput {
    /**
     * The number of the month
     */
    private int month;
    /**
     * The ids of the distributors the producer
     * provides energy for
     */
    private List<Integer> distributorsIds;

    public HistoryProducerOutput(int month, List<Integer> distributorsIds) {
        this.month = month;
        this.distributorsIds = distributorsIds;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public List<Integer> getDistributorsIds() {
        return distributorsIds;
    }

    public void setDistributorsIds(List<Integer> distributorIds) {
        this.distributorsIds = distributorIds;
    }
}
