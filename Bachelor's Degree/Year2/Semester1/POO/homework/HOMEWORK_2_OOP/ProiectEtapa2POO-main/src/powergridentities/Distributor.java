package powergridentities;

import formalities.Contract;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

/**
 * This class contains information about the distributor
 */
public final class Distributor implements Observer {
    /**
     * The id of the distributor
     */
    private int id;
    /**
     * The current budget of the distributor
     */
    private int crtBudget;
    /**
     * The length of the contract adopted by the distributor
     */
    private int contractLength;
    /**
     * The cost for the infrastructure (monthly)
     */
    private int infrastructureCost;
    /**
     * The cost of the production / customer (monthly)
     */
    private int productionCost;
    /**
     * The profit / customer (monthly)
     */
    private int profit;
    /**
     * The cost of the contract
     */
    private int contractCost;
    /**
     * The expenses for this month of the distributor
     */
    private int monthlyExpenses;
    /**
     * The current contracts the distributor has
     */
    private List<Contract> contracts;
    /**
     * Holds the current status of the distributor
     */
    private boolean isBankrupt;
    /**
     * The amount of energy needed by the distributor
     * (in KW)
     */
    private int energyNeededKW;
    /**
     * The amount of energy the distributor HAS
     * (in KW)
     */
    private int energyHasKW;
    /**
     * The strategy the distributor adopts when selecting
     * the producer
     */
    private String producerStrategy;
    /**
     * The current producers the distributor has energy from
     */
    private List<Producer> producers = new ArrayList<>();
    /**
     * holds the truth value of asking whether current distributor
     * should look for new producers or not
     */
    private boolean needReassignment;

    public Distributor(int crtBudget) {
        this.crtBudget = crtBudget;
    }

    public Distributor(int id, int crtBudget, int contractLength, int infrastructureCost,
                       int energyNeededKW, String producerStrategy) {
        this.id = id;
        this.crtBudget = crtBudget;
        this.contractLength = contractLength;
        this.infrastructureCost = infrastructureCost;
        this.contracts = new ArrayList<>();
        this.isBankrupt = false;
        this.energyNeededKW = energyNeededKW;
        this.producerStrategy = producerStrategy;
        this.contractCost = Integer.MAX_VALUE;
    }

    public Distributor(final int id, final int crtBudget, final int contractLength,
                       final int infrastructureCost, final int productionCost) {
        this.id = id;
        this.crtBudget = crtBudget;
        this.contractLength = contractLength;
        this.infrastructureCost = infrastructureCost;
        this.productionCost = productionCost;
        this.profit = 0;
        this.contractCost = Integer.MAX_VALUE;
        this.monthlyExpenses = 0;
        this.contracts = new ArrayList<>();
        this.isBankrupt = false;
    }

    public int getId() {
        return id;
    }

    public void setId(final int id) {
        this.id = id;
    }

    public int getCrtBudget() {
        return crtBudget;
    }

    public void setCrtBudget(final int crtBudget) {
        this.crtBudget = crtBudget;
    }

    public int getContractLength() {
        return contractLength;
    }

    public void setContractLength(final int contractLength) {
        this.contractLength = contractLength;
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

    public int getProfit() {
        return profit;
    }

    public void setProfit(final int profit) {
        this.profit = profit;
    }

    public int getContractCost() {
        return contractCost;
    }

    public void setContractCost(final int contractCost) {
        this.contractCost = contractCost;
    }

    public int getMonthlyExpenses() {
        return monthlyExpenses;
    }

    public void setMonthlyExpenses(final int monthlyExpenses) {
        this.monthlyExpenses = monthlyExpenses;
    }

    public List<Contract> getContracts() {
        return contracts;
    }

    public void setContracts(final List<Contract> contracts) {
        this.contracts = contracts;
    }

    public boolean isBankrupt() {
        return isBankrupt;
    }

    public void setBankrupt(final boolean bankrupt) {
        isBankrupt = bankrupt;
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

    public List<Producer> getProducers() {
        return producers;
    }

    public void setProducers(List<Producer> producers) {
        this.producers = producers;
    }

    public int getEnergyHasKW() {
        return energyHasKW;
    }

    public void setEnergyHasKW(int energyHasKW) {
        this.energyHasKW = energyHasKW;
    }

    /**
     * reformulated getter
     * @return value of truth for the below expressed statement
     */
    public boolean needsReassignment() {
        return needReassignment;
    }

    public void setNeedReassignment(boolean needReassignment) {
        this.needReassignment = needReassignment;
    }

    @Override
    public void update(Observable o, Object arg) {
        int indexProducer = producers.indexOf(arg);
        // if it doesn't work like this (the second condition), try with id
        if ((!this.isBankrupt()) && (indexProducer != -1)) {
            this.energyHasKW = 0;
            this.needReassignment = true;
        }
        // else nothing to change as changes for the producers do not affect
        // the current distributor
    }
}
