package fileio;

import powergridentities.Consumer;
import powergridentities.Distributor;
import powergridentities.Producer;
import reformattedfileio.ReformattedInput;

import java.util.ArrayList;
import java.util.List;

/**
 * This class contains a method
 * that creates the output in the desired format
 * from the input received
 */
public final class OutputBuilder {
    private ReformattedInput input;

    public OutputBuilder(final ReformattedInput input) {
        this.input = input;
    }


    /**
     * given the consumers and the distributors
     * in the input, build the classes which contain
     * the desired info about these
     * no @param
     * @return the new class containing the info
     * in the desired format
     */
    public OutputFormat createOutput() {
        List<Distributor> distributors = input.getReformattedData().getDistributors();
        List<Consumer> consumers = input.getReformattedData().getConsumers();
        List<Producer> producers = input.getReformattedData().getProducers();

        List<ConsumerOutputData> outputConsumers = new ArrayList<>();
        List<DistributorOutputData> outputDistributers = new ArrayList<>();
        List<ProducerOutputData> outputProducers = new ArrayList<>();

        for (Consumer consumer: consumers) {
            outputConsumers.add(new ConsumerOutputData(consumer.getId(),
                    consumer.isBankrupt(), consumer.getCrtBudget()));
        }

        for (Distributor distributor: distributors) {
            outputDistributers.add(new DistributorOutputData(distributor.getId(),
                    distributor.getEnergyNeededKW(), distributor.getContractCost(),
                    distributor.getCrtBudget(), distributor.getProducerStrategy(),
                    distributor.isBankrupt(), distributor.getContracts()));
        }

        for (Producer producer: producers) {
            // seems like we don't need the first month to output
            producer.getMonthlyHistory().remove(0);
            outputProducers.add(new ProducerOutputData(producer.getId(),
                    producer.getMaxDistributors(), producer.getPriceKW(),
                    producer.getEnergyType(), producer.getEnergyPerDistributor(),
                    producer.getMonthlyHistory()
                    ));
        }

        return new OutputFormat(outputConsumers, outputDistributers, outputProducers);
    }
}
