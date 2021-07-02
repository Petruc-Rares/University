package calculatorsdistributor;

import utils.Utils;

public final class CalculatorDistributorFactory {
    private static CalculatorDistributorFactory instance = null;

    private CalculatorDistributorFactory() {

    }

    /**
     * "constructor" for singleton, in case an
     * object of class CalculatorDistributorFactory wasn't instantiated
     * otherwise return the object
     * @return the only instance of this class
     */
    public static CalculatorDistributorFactory getInstance() {
        if (instance == null) {
            instance = new CalculatorDistributorFactory();
        }
        return instance;
    }


    /**
     * get the calculator for the specific
     * @param calculationType
     * @return an instance on the desired calculator
     */
    public CalculateDistributor getCalculator(final String calculationType) {
        if (calculationType == null) {
            return null;
        }

        CalculatorTypes calculatorType = Utils.stringToCalculator(calculationType.toUpperCase());

        return switch (calculatorType) {
            case CONTRACT_PRICE -> CalculatorContractPrice.getInstance();
            case CURRENT_BUDGET -> CalculatorCurrentBudget.getInstance();
            case MONTHLY_EXPENSES ->  CalculatorMonthlyExpenses.getInstance();
            case PROFIT -> CalculatorProfit.getInstance();
            case PRODUCTION_COST -> CalculatorProductionCost.getInstance();
            default -> null;
        };
    }
}
