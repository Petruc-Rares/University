package solver;

import calculatorsconsumer.ConsumerFinancialStatus;
import calculatorsdistributor.CalculateDistributor;
import calculatorsdistributor.CalculatorDistributorFactory;
import fileio.ConsumerInputData;
import fileio.HistoryProducerOutput;
import fileio.MonthlyUpdates;
import fileio.NewDistributorPrices;
import fileio.OutputBuilder;
import fileio.OutputFormat;
import fileio.UpdateProducer;
import formalities.Contract;
import powergridentities.Consumer;
import powergridentities.Distributor;
import powergridentities.Producer;
import producerupdates.PRDUpdates;
import reformattedfileio.ReformattedInput;
import strategies.DistributorStrategy;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * The class contains methods that will solve
 * the tests for this project
 */
public final class Solver {
    /**
     * will need the input in order to know
     * information about the the number of rounds,
     * consumers, distributors and updates
     */
    private ReformattedInput input;
    /**
     * string representing the name of the input file
     */
    private String inputFile;
    /**
     * string representing the name of the output file
     */
    private String outputFile;
    /**
     * factory managing the basic calculations that
     * have to be made for a distributor
     */
    private CalculatorDistributorFactory calculationFactory;
    /**
     * calculates the profit that a distributor has
     * per person
     */
    private CalculateDistributor profitCalculator;
    /**
     * calculates the budget of the distributor
     * in the current month
     */
    private CalculateDistributor budgetCalculator;
    /**
     * calculates the contract price that the consumers
     * will see in the current month
     */
    private CalculateDistributor contractPriceCalculator;
    /**
     * calculates the monthly expenses that the distributor
     * should pay
     */
    private CalculateDistributor monthlyExpensesCalculator;
    /**
     * need such a variable to check if there are
     * any distributors available in any month
     * (if all distributors are bankrupt the game ends)
     */
    private int noBankruptDistributors;
    /**
     * holds account of the producers' lists
     * and notifies the distributors when updates
     * for producers are done
     **/
    private PRDUpdates prdUpdates;

    public Solver(final ReformattedInput input, final PRDUpdates prdUpdates,
                  final String inputFile, final String outputFile) {
        this.input = input;
        this.prdUpdates = prdUpdates;
        this.inputFile = inputFile;
        this.outputFile = outputFile;

        calculationFactory = CalculatorDistributorFactory.getInstance();

        profitCalculator = calculationFactory.getCalculator("PROFIT");
        budgetCalculator = calculationFactory.getCalculator("CURRENT_BUDGET");
        contractPriceCalculator = calculationFactory.getCalculator("CONTRACT_PRICE");
        monthlyExpensesCalculator =
                calculationFactory.getCalculator("MONTHLY_EXPENSES");
    }

    /**
     * updates info regarding
     * new consumers and new prices
     * of the distributors for the
     * @param crtMonth representing
     * the current round of the game
     */
    void updateInfo(final int crtMonth) {
        // get the updates corresponding the current month
        MonthlyUpdates thisMonthUpdate = input.getMonthlyUpdates().get(crtMonth);

        List<ConsumerInputData> newConsumers = thisMonthUpdate.getNewConsumers();

        // ad the new consumers to the list of existing consumers
        List<Consumer> allConsumersThisMonth =
                input.getReformattedData().getConsumers();
        for (ConsumerInputData consumer: newConsumers) {
            allConsumersThisMonth.add(new Consumer(consumer.getId(), consumer.getInitialBudget(),
                    consumer.getMonthlyIncome()));
        }

        // modify the prices for the distributors, that changes have been made for
        List<NewDistributorPrices> newPrices = thisMonthUpdate.getDistributorChanges();
        for (NewDistributorPrices newPrice: newPrices) {
            int distributorId = newPrice.getId();
            Distributor distributor = Utils.getDistributor(distributorId);

            distributor.setInfrastructureCost(newPrice.getInfrastructureCost());
        }
    }


    /**
     * give the consumers the monthly income
     * no @param
     */
    void payConsumers() {
        List<Consumer> consumers = input.getReformattedData().getConsumers();
        for (Consumer consumer: consumers) {
            if (!consumer.isBankrupt()) {
                ConsumerFinancialStatus.setNewBudget(consumer);
            }
        }
    }

    /**
     * updates the contract prices for all the distributors
     * and removes the contracts that reached the end
     * no @param
     */
    void updateContracts() {
        List<Distributor> distributors = input.getReformattedData().getDistributors();

        for (Distributor distributor: distributors) {
            if (!distributor.isBankrupt()) {
                contractPriceCalculator.calculate(distributor);
                // now check if there are consumers that have their contract at the end
                List<Contract> distributorsContracts = distributor.getContracts();
                // create a list of contracts that have to be removed
                List<Contract> toRemoveContracts = new ArrayList<>();
                for (Contract contract : distributorsContracts) {
                    if (contract.getRemainedContractMonths() == 0) {
                        toRemoveContracts.add(contract);

                        int consumerId = contract.getConsumerId();
                        Consumer consumer = Utils.getConsumer(consumerId);
                        consumer.setContract(null);
                    }
                }
                distributorsContracts.removeAll(toRemoveContracts);
            }
        }
    }

    /**
     * in case there are consumers with no contract
     * this method will assign them a contract with the
     * cheapest distributor available
     * no @param
     */
    void chooseCheapestDistributor() {
        List<Consumer> allConsumersThisMonth = input.getReformattedData().getConsumers();
        Distributor cheapestDistributor = Utils.getCheapestDistributor();

        int cheapestPrice = cheapestDistributor.getContractCost();
        int duration = cheapestDistributor.getContractLength();
        // CD - CheapestDistributor
        List<Contract> currentContractsCD = cheapestDistributor.getContracts();
        for (Consumer consumer: allConsumersThisMonth) {
            // consumers can change their contract only if they are not bankrupt and if
            // the last contract expired or the distributor is bankrupt
            if (((consumer.getContract() == null))
                    && (!consumer.isBankrupt())) {
                // create the new contract
                Contract newContract = new Contract(consumer.getId(),
                        cheapestPrice, duration, cheapestDistributor.getId());

                consumer.setContract(newContract);

                // assign it also to the distributor
                currentContractsCD.add(newContract);
            }
        }
    }

    /**
     * updates the new budget of the distributor
     * and in case there are consumers which became
     * bankrupt, remove the contracts with them
     * no @param
     */
    void updateDistributorsPayment() {
        List<Distributor> distributors = input.getReformattedData().getDistributors();

        for (Distributor distributor: distributors) {
            if (!distributor.isBankrupt()) {
                monthlyExpensesCalculator.calculate(distributor);

                List<Contract> distributorsContracts = distributor.getContracts();

                List<Contract> toRemoveContracts = new ArrayList<>();
                for (Contract distributorContract: distributorsContracts) {
                    int consumerId = distributorContract.getConsumerId();
                    Consumer crtConsumer = Utils.getConsumer(consumerId);

                    // if consumer is bankrupt the contract will end
                    if (crtConsumer.isBankrupt()) {
                        toRemoveContracts.add(distributorContract);
                        crtConsumer.setContract(null);
                    }
                }
                distributorsContracts.removeAll(toRemoveContracts);

                budgetCalculator.calculate(distributor);

                if (distributor.getCrtBudget() >= 0) {

                    for (Contract contract : distributorsContracts) {
                        // reduce the contract duration by one month
                        contract.setRemainedContractMonths(contract.getRemainedContractMonths()
                                - 1);
                    }
                } else {
                    distributor.setBankrupt(true);
                    noBankruptDistributors++;
                    // consumers are declared free of contract
                    for (Contract contract : distributorsContracts) {
                        contract.setRemainedContractMonths(0);
                    }
                    // producers that deliver energy to this distributor
                    // should remove it from their lists
                    for (Producer distributorsProducers: distributor.getProducers()) {
                        List<HistoryProducerOutput> history = distributorsProducers.
                                getMonthlyHistory();
                        HistoryProducerOutput thisMonthRegister = history.get(history.size() - 1);
                        List<Integer> distributorsIds = thisMonthRegister.getDistributorsIds();
                        distributorsIds.remove(distributor.getId());
                    }
                }
            }
        }
    }

    /**
     * sets the payment status (can pay, inDebt
     * or isBankrupt) for the consumers in the game
     */
    void setConsumersPaymentStatus() {
        List<Consumer> consumers = input.getReformattedData().getConsumers();

        for (Consumer consumer: consumers) {
            ConsumerFinancialStatus.setPaymentStatus(consumer);
        }
    }

    /**
     * solve the current test for the first month
     * as it has some particularities like the profit must
     * be set for each company, not only for those
     * that were made updates for. No need to check
     * so many conditions like in a typical month
     * no @param
     */
    public void solveMonth0() {
        payConsumers();

        List<Consumer> consumers = input.getReformattedData().getConsumers();

        // second step is calculating the contract prices
        List<Distributor> distributors = input.getReformattedData().getDistributors();
        for (Distributor distributor: distributors) {
            contractPriceCalculator.calculate(distributor);
        }

        // third step is that consumers choose their first contracts
        chooseCheapestDistributor();

        // forth step is that every consumer checks
        // if he has enough money to pay the current installment
        // and the payment status of each is set accordingly
        setConsumersPaymentStatus();

        // fifth step is that the distributors pay their debts
        // and recover the installment from the consumers
        updateDistributorsPayment();

    }

    /**
     * iterates through the list of producers and copies
     * the last distributors each producer had previous month, as
     * changes are done rarely and most of the time just minor changes
     * are done during a month
     * @param crtMonth represents the current round of the game
     */
    void updateHistoryProducers(int crtMonth) {
        for (Producer producer: input.getReformattedData().getProducers()) {
            List<HistoryProducerOutput> monthlyHistory = producer.getMonthlyHistory();
            monthlyHistory.add(new HistoryProducerOutput(crtMonth + 1,
                    new ArrayList<>(
                            monthlyHistory.get(monthlyHistory.size() - 1).getDistributorsIds())));
        }
    }

    /**
     * updates the producers' list of distributors
     * and reassigns distributors to best available option
     * distributor from the list they are interested in
     * @param crtMonth represents the current round of the simulation
     */
    void updateProducerDistributorRelationship(int crtMonth) {
        MonthlyUpdates thisMonthUpdate = input.getMonthlyUpdates().get(crtMonth);

        List<UpdateProducer> updatesProducer = thisMonthUpdate.getProducerChanges();
        for (UpdateProducer updateProducer: updatesProducer) {
            Producer producer = Utils.getProducer(updateProducer.getId());
            producer.setEnergyPerDistributor(updateProducer.getEnergyPerDistributor());
            prdUpdates.reassignDistributors(updateProducer.getId());
        }

        for (Distributor distributor: input.getReformattedData().getDistributors()) {
            if (distributor.needsReassignment()) {
                for (Producer producer : distributor.getProducers()) {
                    producer.setCrtNoDistributors(producer.getCrtNoDistributors() - 1);


                    List<HistoryProducerOutput> historyProducer = producer.getMonthlyHistory();

                    // we add one to the current month as the history is counted from month 0,
                    // even though at output we remove it
                    List<Integer> distributorIds = historyProducer.get(crtMonth + 1).
                            getDistributorsIds();

                    distributorIds.remove(distributorIds.indexOf(distributor.getId()));

                }
                // forget distributor's producers
                distributor.setProducers(new ArrayList<>());

                // now reapply the strategy for the distributor
                DistributorStrategy.applyStrategy(distributor);

                // establish the new status as reassignment has been done
                distributor.setNeedReassignment(false);
            }
        }
    }

    /**
     * simulates the game for the month
     * @param crtMonth represents the month in progress
     */
    void solveCrtMonth(final int crtMonth) {
        // every new month, create a new history for the current month
        // for every producer by copying the distributorIds from the prev month
        updateHistoryProducers(crtMonth);

        // step 1 is to check the new updates
        // regarding the new consumers and the new prices
        // adopted by the distributors
        updateInfo(crtMonth);

        // step 2 is to give the wage to the consumers
        // which are not bankrupt because we ignore them
        payConsumers();

        // step 3 is to modify the prices of the contracts for the distributors
        // that are not bankrupt
        updateContracts();

        // step 4 is that the consumers choose the cheapest distributor
        // that is not bankrupt
        chooseCheapestDistributor();

        // fifth step is that every consumer checks
        // if he has enough money to pay the current installment
        // and the payment status of each is set accordingly
        setConsumersPaymentStatus();

        // six step is that the distributors pay their debts
        // and recover the installment from the consumers
        // if after paying the expenses they realise that some
        // consumers are bankrupt remove them from the contracts
        updateDistributorsPayment();

        // update producer by changing the energyPerDistributor
        // and the distributors assigned to it this month
        // also, distributors are reassigned to best available producers
        // they found
        updateProducerDistributorRelationship(crtMonth);

    }

    /**
     * solve the current test
     * and writes the output file
     * no @param
     * @throws IOException if file does not exist
     */
    public void solveTest() throws IOException {
        // month 0 requires a few different actions to be taken
        // than rest of the months
        solveMonth0();

        int noTotalDistributors = input.getReformattedData().getDistributors().size();

        int noMonths = input.getNumberOfTurns();
        int crtMonth = 0;

        while (crtMonth < noMonths) {
            // CHECK THAT ALL DISTRIBUTORS ARE STILL ELIGIBLE
            if (noBankruptDistributors == noTotalDistributors) {
                System.out.println("GAME OVER! ALL DISTRIBUTERS ARE IN BANKRUPT");
                break;
            }

            solveCrtMonth(crtMonth);

            crtMonth++;
        }
         OutputBuilder outputBuilder = new OutputBuilder(input);
         OutputFormat output = outputBuilder.createOutput();
         output.writeOutput(outputFile);
     }
}
