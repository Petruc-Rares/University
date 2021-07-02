package powergridentities;

import com.fasterxml.jackson.annotation.JsonProperty;
import formalities.Contract;

/**
 * This class contains information about the consumer
 */
public final class Consumer {
    /**
     * The id of the consumer
     */
    private int id;
    /**
     * The total amount of money the consumer has at the moment
     * of interogation
     */
    private int crtBudget;
    /**
     * The monthly income of the consumer
     */
    private int monthlyIncome;
    /**
     * inDebt set to false when consumer paid at time all the instalments
     * and set to true when he missed one month or two ( when he also will get
     * excluded out of the game)
     */
    private boolean inDebt;
    /**
     * The sum the consumer wasn't able to pay last month
     */
    private int overdueSum;
    /**
     * isBankrupt set to true only when the consumer is in debt for two
     * months in a row at the same company
     */
    private boolean isBankrupt;
    /**
     * The current contract the consumer is engaged in
     */
    private Contract contract;
    /**
     * the id of the distributor the consumer has debt to
     */
    private int debtDistributorId;

    public Consumer() {
    }

    public Consumer(final int id, final int crtBudget, final int monthlyIncome) {
        this.id = id;
        this.crtBudget = crtBudget;
        this.monthlyIncome = monthlyIncome;
    }

    public int getOverdueSum() {
        return overdueSum;
    }

    public void setOverdueSum(final int overdueSum) {
        this.overdueSum = overdueSum;
    }

    public int getId() {
        return id;
    }

    @JsonProperty("id")
    public void setId(final int id) {
        this.id = id;
    }

    public int getCrtBudget() {
        return crtBudget;
    }

    @JsonProperty("initialBudget")
    public void setCrtBudget(final int crtBudget) {
        this.crtBudget = crtBudget;
    }

    public int getMonthlyIncome() {
        return monthlyIncome;
    }

    @JsonProperty("monthlyIncome")
    public void setMonthlyIncome(final int monthlyIncome) {
        this.monthlyIncome = monthlyIncome;
    }

    public boolean isInDebt() {
        return inDebt;
    }

    public void setInDebt(final boolean inDebt) {
        this.inDebt = inDebt;
    }

    public boolean isBankrupt() {
        return isBankrupt;
    }

    public void setBankrupt(final boolean bankrupt) {
        isBankrupt = bankrupt;
    }

    public Contract getContract() {
        return contract;
    }

    public void setContract(final Contract contract) {
        this.contract = contract;
    }

    public int getDebtDistributorId() {
        return debtDistributorId;
    }

    public void setDebtDistributorId(int debtDistributorId) {
        this.debtDistributorId = debtDistributorId;
    }
}
