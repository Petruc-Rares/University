package utils;

import calculatorsdistributor.CalculatorTypes;
import powergridentities.Consumer;
import powergridentities.Distributor;
import powergridentities.Producer;
import reformattedfileio.ReformattedInput;

import java.util.List;

/**
 * The class contains static methods that help with common tasks
 * related to the input
 */
public final class Utils {


    /**
     * Transforms a string into an enum
     *
     * @param calculatorType representing the
     *                       type of the calculator
     * @return an CalculatorType enum
     */
    public static CalculatorTypes stringToCalculator(final String calculatorType) {
        return switch (calculatorType) {
            case "CONTRACT_PRICE" -> CalculatorTypes.CONTRACT_PRICE;
            case "CURRENT_BUDGET" -> CalculatorTypes.CURRENT_BUDGET;
            case "MONTHLY_EXPENSES" -> CalculatorTypes.MONTHLY_EXPENSES;
            case "PROFIT" -> CalculatorTypes.PROFIT;
            case "PRODUCTION_COST" -> CalculatorTypes.PRODUCTION_COST;
            default -> null;
        };
    }

    /**
     * will need the input in order to solve
     * utils tasks
     */
    private static ReformattedInput input;

    private Utils() {
    }

    public static void setInput(final ReformattedInput input) {
        Utils.input = input;
    }

    /**
     * checks the list of consumers in the input
     * and returns the one corresponding to the id
     * @param consumerId representing the id of the consumer
     * @return the corresponding consumer instance
     */
    public static Consumer getConsumer(final int consumerId) {
        List<Consumer> consumers = input.getReformattedData().getConsumers();

        for (Consumer consumer: consumers) {
            if (consumer.getId() == consumerId) {
                return consumer;
            }
        }
        // no consumer with the specified id was found
        return null;
    }

    /**
     * checks the list of distributors in the input
     * and returns the one corresponding to the id
     * @param distributorId representing the id of the distributor
     * @return the corresponding distributor instance
     */
    public static Distributor getDistributor(final int distributorId) {
        List<Distributor> distributors = input.getReformattedData().getDistributors();

        for (Distributor distributor: distributors) {
            if (distributor.getId() == distributorId) {
                return distributor;
            }
        }
        // no distributor with the specified id was found
        return null;
    }

    /**
     * checks the list of producers in the input
     * and returns the one corresponding to the id
     * @param producerId representing the id of the producer
     * @return the corresponding producer instance
     */
    public static Producer getProducer(final int producerId) {
        List<Producer> producers = input.getReformattedData().getProducers();

        for (Producer producer: producers) {
            if (producer.getId() == producerId) {
                return producer;
            }
        }
        // no producer with the specified id was found
        return null;
    }

    /**
     * checks the list of distributors in the input
     * and returns the one who has the best contract price
     * no @param
     * @return the corresponding distributor instance
     */
    public static Distributor getCheapestDistributor() {
        List<Distributor> distributors = input.getReformattedData().getDistributors();

        int minPriceDistributor = Integer.MAX_VALUE;
        Distributor cheapestDistributor = null;

        for (Distributor distributor: distributors) {
            if ((distributor.getContractCost() < minPriceDistributor)
                    && (!distributor.isBankrupt())) {
                minPriceDistributor = distributor.getContractCost();
                cheapestDistributor = distributor;
            }
        }

        return cheapestDistributor;
    }

}
