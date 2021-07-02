package producerslists;

import comparatorlists.ComparatorGreenList;
import comparatorlists.ComparatorPriceList;
import comparatorlists.ComparatorQuantityList;
import powergridentities.Producer;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * This class holds the lists of different types
 * of producers ordered after specific criteria
 */
public final class ProducersLists {
    private List<Producer> greenList = new ArrayList<>();
    private List<Producer> priceList = new ArrayList<>();
    private List<Producer> quantityList = new ArrayList<>();

    /**
     * This method sorts the lists of producers
     * using the comparators
     */
    public void sortLists() {
        Collections.sort(greenList, new ComparatorGreenList());
        Collections.sort(priceList, new ComparatorPriceList());
        Collections.sort(quantityList, new ComparatorQuantityList());
    }

    /**
     * This method is used for initialising the lists
     * of producers, by adding producers from the input
     * one by one
     * @param producer represents the current producer
     */
    public void addToAll(Producer producer) {
        greenList.add(producer);
        priceList.add(producer);
        quantityList.add(producer);
    }

    /**
     * This method is used for initialising the lists sorted
     * by different criteria using the
     * @param producers representing the producers given from the input
     * @return the different lists (all in one entity)
     */
    public static ProducersLists initialiseLists(List<Producer> producers) {
        ProducersLists producersLists = new ProducersLists();

        for (Producer producer: producers) {
            producersLists.addToAll(producer);
        }

        producersLists.sortLists();
        return producersLists;
    }

    public List<Producer> getGreenList() {
        return greenList;
    }

    public List<Producer> getPriceList() {
        return priceList;
    }

    public List<Producer> getQuantityList() {
        return quantityList;
    }
}
