package calculatorsdistributor;

import formalities.Contract;
import powergridentities.Distributor;

import java.util.List;

public final class CalculatorMonthlyExpenses implements CalculateDistributor {

    private static CalculatorMonthlyExpenses instance = null;

    private CalculatorMonthlyExpenses() {

    }

    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorMonthlyExpenses wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorMonthlyExpenses getInstance() {
        if (instance == null) {
            instance = new CalculatorMonthlyExpenses();
        }
        return instance;
    }

    /**
     * calculates the monthly expenses of the distributor
     * @param distributor
     */
    public void calculate(final Distributor distributor) {
        List<Contract> contracts = distributor.getContracts();
        int noClients = contracts.size();
        int infrastructureCost = distributor.getInfrastructureCost();
        int productionCost = distributor.getProductionCost();

        distributor.setMonthlyExpenses(infrastructureCost + productionCost * noClients);
    }
}
