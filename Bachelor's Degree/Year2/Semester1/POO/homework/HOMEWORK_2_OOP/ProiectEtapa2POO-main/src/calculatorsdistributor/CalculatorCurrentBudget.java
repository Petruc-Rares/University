package calculatorsdistributor;

import formalities.Contract;
import powergridentities.Consumer;
import powergridentities.Distributor;
import utils.Utils;

import java.util.List;

public final class CalculatorCurrentBudget implements CalculateDistributor {

    private static CalculatorCurrentBudget instance = null;

    private CalculatorCurrentBudget() {

    }


    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorCurrentBudget wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorCurrentBudget getInstance() {
        if (instance == null) {
            instance = new CalculatorCurrentBudget();
        }
        return instance;
    }

    /**
     * calculates the current budget of the distributor
     * @param distributor
     */
    public void calculate(final Distributor distributor) {
        int profit = 0;

        int crtBudget = distributor.getCrtBudget();
        int monthlyExpenses = distributor.getMonthlyExpenses();
        int contractPrice = distributor.getContractCost();

        List<Contract> contracts = distributor.getContracts();

        // no matter what, we substract the monthly expenses
        crtBudget -= monthlyExpenses;

        // check for profit
        for (Contract crtContract: contracts) {
            Consumer consumer = Utils.getConsumer(crtContract.getConsumerId());

            if (consumer != null) {
                // if consumer is not in debt, he can make
                // the payment, so number of clients who pay
                // the installment increments
                if (!consumer.isInDebt()) {
                    profit += consumer.getContract().getPrice();
                }
            }
        }

        crtBudget += profit;

        distributor.setCrtBudget(crtBudget);
    }
}
