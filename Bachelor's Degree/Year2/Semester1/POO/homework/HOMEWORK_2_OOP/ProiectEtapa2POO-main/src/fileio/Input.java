package fileio;

import java.util.List;

/**
 * This class contains information about the input
 */
public final class Input {
    /**
     * The number of rounds the game takes place
     */
    private int numberOfTurns;
    /**
     * The initial data in the json file
     */
    private InitialData initialData;
    /**
     * The updates in the input file
     */
    private List<MonthlyUpdates> monthlyUpdates;

    public Input() {
    }

    public Input(final int noRounds, final InitialData initialData,
                 final List<MonthlyUpdates> monthlyUpdates) {
        this.numberOfTurns = noRounds;
        this.initialData = initialData;
        this.monthlyUpdates = monthlyUpdates;
    }

    public List<MonthlyUpdates> getMonthlyUpdates() {
        return monthlyUpdates;
    }

    public void setMonthlyUpdates(final List<MonthlyUpdates> monthlyUpdates) {
        this.monthlyUpdates = monthlyUpdates;
    }

    public int getNumberOfTurns() {
        return numberOfTurns;
    }

    public void setNumberOfTurns(final int numberOfTurns) {
        this.numberOfTurns = numberOfTurns;
    }

    public void setInitialData(final InitialData initialData) {
        this.initialData = initialData;
    }

    public InitialData getInitialData() {
        return initialData;
    }
}
