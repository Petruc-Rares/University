package fileio;

public final class ConsumerInputData {
    /**
     * The id of the consumer
     */
    private int id;
    /**
     * The initial budget of the consumer
     */
    private int initialBudget;
    /**
     * The monthly income of the consumer
     */
    private int monthlyIncome;

    public ConsumerInputData() {

    }

    public ConsumerInputData(final int id, final int initialBudget, final int monthlyIncome) {
        this.id = id;
        this.initialBudget = initialBudget;
        this.monthlyIncome = monthlyIncome;
    }


    public int getId() {
        return id;
    }

    public void setId(final int id) {
        this.id = id;
    }

    public int getInitialBudget() {
        return initialBudget;
    }

    public void setInitialBudget(final int initialBudget) {
        this.initialBudget = initialBudget;
    }

    public int getMonthlyIncome() {
        return monthlyIncome;
    }

    public void setMonthlyIncome(final int monthlyIncome) {
        this.monthlyIncome = monthlyIncome;
    }
}
