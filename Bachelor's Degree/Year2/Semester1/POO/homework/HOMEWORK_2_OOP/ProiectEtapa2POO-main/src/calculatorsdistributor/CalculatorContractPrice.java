package calculatorsdistributor;

import powergridentities.Distributor;

public final class CalculatorContractPrice implements CalculateDistributor {

    private static CalculatorContractPrice instance = null;

    private CalculatorContractPrice() {

    }

    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorContractPrice wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorContractPrice getInstance() {
        if (instance == null) {
            instance = new CalculatorContractPrice();
        }
        return instance;
    }

    /**
     * calculates the new contract price
     * according to the changes made (number of customers,
     * cost of the infrastructure and production) for
     * @param distributor
     */
    public void calculate(final Distributor distributor) {
        int noClients = distributor.getContracts().size();
        int infrastructureCost = distributor.getInfrastructureCost();
        int productionCost = distributor.getProductionCost();
        int profit = distributor.getProfit();

        if (noClients == 0) {
            distributor.setContractCost(infrastructureCost + productionCost + profit);
        } else {
            distributor.setContractCost((int) Math.round(Math.floor(infrastructureCost / noClients)
                    + productionCost + profit));
        }
    }
}
