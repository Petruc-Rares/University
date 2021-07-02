package calculatorsdistributor;

import powergridentities.Distributor;

public final class CalculatorProfit implements CalculateDistributor {

    private static CalculatorProfit instance = null;

    /**
     * this factor will be multiplied
     * by the production cost to get the profit
     * per consumer of the distributer
     */
    private static final double MULTIPLICATIONFACTOR = 0.2;

    private CalculatorProfit() {

    }

    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorProfit wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorProfit getInstance() {
        if (instance == null) {
            instance = new CalculatorProfit();
        }
        return instance;
    }

    /**
     * calculates the profit per customer
     * according to the production cost
     * for the specific
     * @param distributor
     */
    @Override
    public void calculate(final Distributor distributor) {
        distributor.setProfit((int) Math.round(Math.floor(MULTIPLICATIONFACTOR
                * distributor.getProductionCost())));
    }
}
