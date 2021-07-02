package strategies;
import powergridentities.Distributor;
import producerslists.ProducersLists;

public final class Context {
    private ProducersStrategy strategy;
    private Distributor distributor;
    private ProducersLists producersLists;

    public Context(ProducersStrategy strategy, Distributor distributor,
                   ProducersLists producersLists) {
        this.strategy = strategy;
        this.distributor = distributor;
        this.producersLists = producersLists;
    }

    /**
     * method that executes a strategy taking into account
     * the types of producers' lists and the current distributor
     */
    public void executeStrategy() {
        strategy.applyStrategy(distributor, producersLists);
    }
}
