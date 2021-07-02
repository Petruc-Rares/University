package strategies;

import powergridentities.Distributor;
import powergridentities.Producer;
import producerslists.ProducersLists;

import java.util.List;

public final class PriceStrategy implements ProducersStrategy {
    @Override
    public void applyStrategy(Distributor distributor, ProducersLists producersLists) {
        List<Producer> producers = producersLists.getPriceList();
        ChooseProducer.chooseProducer(distributor, producers);
    }

}
