package strategies;

import calculatorsdistributor.CalculateDistributor;
import calculatorsdistributor.CalculatorDistributorFactory;
import fileio.HistoryProducerOutput;
import powergridentities.Distributor;
import powergridentities.Producer;

import java.util.Collections;
import java.util.List;

public final class ChooseProducer {
    private static final CalculatorDistributorFactory CALCULATION_FACTORY =
            CalculatorDistributorFactory.getInstance();

    private static final CalculateDistributor PROFIT_CALCULATOR = CALCULATION_FACTORY.
            getCalculator("PROFIT");

    private static final CalculateDistributor PRODUCTION_COST_CALCULATOR = CALCULATION_FACTORY.
            getCalculator("PRODUCTION_COST");

    private ChooseProducer() {

    }

    // find the best possible producer from the list given

    /**
     * method that find the best fit producer from the list given for the
     * @param distributor
     * @param producers given this list
     */
    public static void chooseProducer(Distributor distributor, List<Producer> producers) {
        // distributor should not be bankrupt by this point
        int iterator = 0;
        List<Producer> distributorsProducers = distributor.getProducers();

        while (distributor.getEnergyHasKW() < distributor.getEnergyNeededKW()) {
            Producer crtProducer = producers.get(iterator);

            // if current producer has capacity to provide electricity to another distributor
            // and is not already providing to the distributor we should add it
            if ((crtProducer.getCrtNoDistributors() < crtProducer.getMaxDistributors())
                    && (!distributorsProducers.contains(crtProducer))) {
                distributorsProducers.add(crtProducer);

                distributor.setEnergyHasKW(distributor.getEnergyHasKW()
                        + crtProducer.getEnergyPerDistributor());

                // add the distributor id to the ids and select current month
                List<HistoryProducerOutput> history = crtProducer.getMonthlyHistory();
                HistoryProducerOutput histThisMonth  = history.get(history.size() - 1);

                List<Integer> distributorsIds = histThisMonth.getDistributorsIds();
                distributorsIds.add(distributor.getId());

                /**
                 * try without this sort to see the score
                 */
                Collections.sort(distributorsIds);

                histThisMonth.setDistributorsIds(distributorsIds);

                crtProducer.setCrtNoDistributors(crtProducer.getCrtNoDistributors() + 1);
            }
            iterator++;
        }

        // this might be replaced by a calculator (factory)

        PRODUCTION_COST_CALCULATOR.calculate(distributor);
        PROFIT_CALCULATOR.calculate(distributor);
    }
}
