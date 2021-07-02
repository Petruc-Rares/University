package strategies;

import powergridentities.Distributor;
import powergridentities.Producer;
import producerslists.ProducersLists;

import java.util.List;

public final class GreenStrategy implements ProducersStrategy {
    @Override
    public void applyStrategy(Distributor distributor, ProducersLists producersLists) {
        List<Producer> producers = producersLists.getGreenList();
        ChooseProducer.chooseProducer(distributor, producers);
    }
}
