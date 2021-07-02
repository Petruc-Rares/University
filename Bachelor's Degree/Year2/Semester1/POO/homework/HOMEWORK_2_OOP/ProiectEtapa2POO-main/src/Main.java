import fileio.Input;
import fileio.InputLoader;
import powergridentities.Distributor;
import powergridentities.Producer;
import producerslists.ProducersLists;
import producerupdates.PRDUpdates;
import producerupdates.ProducersDistributors;
import reformattedfileio.ReformatInput;
import reformattedfileio.ReformattedInput;
import solver.Solver;
import strategies.DistributorStrategy;
import utils.Utils;

import java.util.ArrayList;
import java.util.List;

/**
 * Entry point to the simulation
 */
public final class Main {

    private Main() { }

    /**
     * Main function which reads the input file and starts simulation
     *
     * @param args input and output files
     * @throws Exception might error when reading/writing/opening files, parsing JSON
     */
    public static void main(final String[] args) throws Exception {
        InputLoader inputLoader = new InputLoader(args[0]);

        Input input = inputLoader.readData();
        ReformattedInput reformattedInput = ReformatInput.reformatInput(input);

        List<Distributor> distributors = reformattedInput.getReformattedData().getDistributors();
        List<Producer> producers = reformattedInput.getReformattedData().getProducers();

        // utils will require an input for its'
        // methods to work as desired
        Utils.setInput(reformattedInput);

        // create the lists of the producers, sorted accordingly to the task
        ProducersLists producersLists = ProducersLists.initialiseLists(producers);

        // select the best option for the distributors to choose from
        DistributorStrategy distributorStrategy = new DistributorStrategy(producersLists);
        distributorStrategy.initialiseDistributors(distributors);

        List<ProducersDistributors> producersDistributors = new ArrayList<>();
        for (Producer producer: producers) {
            ProducersDistributors.buildListRelationships(producer, producersDistributors,
                    distributors);
        }

        // entity responsible for monthly updates that may come for producers
        PRDUpdates prdUpdates = new PRDUpdates(producersDistributors, producersLists);

        Solver solver = new Solver(reformattedInput, prdUpdates, args[0], args[1]);
        solver.solveTest();
    }
}
