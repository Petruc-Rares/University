package reformattedfileio;

import fileio.MonthlyUpdates;

import java.util.List;

public final class ReformattedInput {
    /**
     * The number of rounds the game takes place
     */
    private int numberOfTurns;
    /**
     * The reformatted data after reading the JsonFile
     */
    private ReformattedData reformattedData;
    /**
     * The updates from the input file
     */
    private List<MonthlyUpdates> monthlyUpdates;


    public ReformattedInput(final int numberOfTurns, final ReformattedData reformattedData,
                            final List<MonthlyUpdates> monthlyUpdates) {
        this.numberOfTurns = numberOfTurns;
        this.reformattedData = reformattedData;
        this.monthlyUpdates = monthlyUpdates;
    }

    public int getNumberOfTurns() {
        return numberOfTurns;
    }

    public void setNumberOfTurns(final int numberOfTurns) {
        this.numberOfTurns = numberOfTurns;
    }

    public ReformattedData getReformattedData() {
        return reformattedData;
    }

    public void setReformattedData(final ReformattedData reformattedData) {
        this.reformattedData = reformattedData;
    }

    public List<MonthlyUpdates> getMonthlyUpdates() {
        return monthlyUpdates;
    }

    public void setMonthlyUpdates(final List<MonthlyUpdates> monthlyUpdates) {
        this.monthlyUpdates = monthlyUpdates;
    }
}
