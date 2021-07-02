package reformattedfileio;

import fileio.ConsumerInputData;
import fileio.DistributorInputData;
import fileio.Input;
import fileio.ProducerInputData;
import powergridentities.Consumer;
import powergridentities.Distributor;
import powergridentities.Producer;

import java.util.ArrayList;
import java.util.List;

/**
 * this class reformats the input for the output
 * to look like the ref
 */
public final class ReformatInput {

    private ReformatInput() {

    }

    /**
     * this method changes the format of the input
     * @param input
     * @return an instance of the reformatted input
     */
    public static ReformattedInput reformatInput(final Input input) {
        List<Consumer> consumers = new ArrayList<>();
        List<Distributor> distributors = new ArrayList<>();
        List<Producer> producers = new ArrayList<>();

        List<ConsumerInputData> inputConsumers = input.getInitialData().getConsumers();
        List<DistributorInputData> inputDistributors = input.getInitialData().getDistributors();
        List<ProducerInputData> inputProducers = input.getInitialData().getProducers();

        // create the consumers in the format that will
        // help us develop the application using the input provided
        for (ConsumerInputData consumer : inputConsumers) {
            Consumer reformattedConsumer = new Consumer(consumer.getId(),
                    consumer.getInitialBudget(), consumer.getMonthlyIncome());

            consumers.add(reformattedConsumer);
        }

        // create the distributors in the format that will
        // help us develop the application using the input provided
        for (DistributorInputData distributor : inputDistributors) {
            Distributor reformattedDistributor = new Distributor(distributor.getId(),
                    distributor.getInitialBudget(), distributor.getContractLength(),
                    distributor.getInitialInfrastructureCost(),
                    distributor.getEnergyNeededKW(), distributor.getProducerStrategy());

            distributors.add(reformattedDistributor);
        }

        // create the distributors in the format that will
        // help us develop the application using the input provided
        for (ProducerInputData producer: inputProducers) {
            Producer reformattedProducer = new Producer(producer.getId(), producer.getEnergyType(),
                    producer.getMaxDistributors(), producer.getPriceKW(),
                    producer.getEnergyPerDistributor(), input.getNumberOfTurns());

            producers.add(reformattedProducer);
        }

        ReformattedData reformattedData = new ReformattedData(consumers, distributors, producers);
        ReformattedInput reformattedInput = new ReformattedInput(input.getNumberOfTurns(),
                reformattedData, input.getMonthlyUpdates());

        return reformattedInput;
    }
}
