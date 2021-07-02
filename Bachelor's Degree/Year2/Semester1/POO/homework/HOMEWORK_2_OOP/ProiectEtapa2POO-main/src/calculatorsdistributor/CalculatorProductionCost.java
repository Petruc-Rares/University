package calculatorsdistributor;

import powergridentities.Distributor;
import powergridentities.Producer;

public final class CalculatorProductionCost implements CalculateDistributor {

    private static CalculatorProductionCost instance = null;

    static final int DIVISIONFACTOR = 10;

    private CalculatorProductionCost() {

    }

    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorProductionCost wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorProductionCost getInstance() {
        if (instance == null) {
            instance = new CalculatorProductionCost();
        }
        return instance;
    }

    @Override
    public void calculate(Distributor distributor) {
        int cost = 0;
        for (Producer producer: distributor.getProducers()) {
            cost += producer.getPriceKW() * producer.getEnergyPerDistributor();
        }
        int productionCost = (int) Math.round(Math.floor(cost / DIVISIONFACTOR));

        distributor.setProductionCost(productionCost);
    }
}
