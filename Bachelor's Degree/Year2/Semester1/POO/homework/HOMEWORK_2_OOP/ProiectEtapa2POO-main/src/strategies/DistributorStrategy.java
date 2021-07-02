package strategies;

import powergridentities.Distributor;
import producerslists.ProducersLists;

import java.util.List;

/**
 * This class has a method that applies a strategy for
 * every distributor according to the producer strategy
 */
public final class DistributorStrategy {
    private static ProducersLists producersLists;

    public DistributorStrategy(ProducersLists producersLists) {
        this.producersLists = producersLists;
    }

    /**
     * this method applies a strategy for the given
     * @param distributor
     */
    public static void applyStrategy(Distributor distributor) {
        Context context;
        switch (distributor.getProducerStrategy()) {
            case "GREEN":
                context = new Context(new GreenStrategy(), distributor, producersLists);
                break;
            case "PRICE":
                context = new Context(new PriceStrategy(), distributor, producersLists);
                break;

            case "QUANTITY":
                context = new Context(new QuantityStrategy(), distributor, producersLists);
                break;
            default:
                throw new IllegalStateException(
                        "Unexpected value: " + distributor.getProducerStrategy());
        }
        context.executeStrategy();
    }

    /**
     * initialise an entity of distributorStrategy for
     * every distributor is the following list
     * @param distributors
     */
    public void initialiseDistributors(List<Distributor> distributors) {
        for (Distributor distributor: distributors) {
            applyStrategy(distributor);
        }
    }
}
