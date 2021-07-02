package calculatorsdistributor;

import powergridentities.Distributor;

/**
 * This interface has a method that
 * can be assigned to do multiple
 * calculation for a specific distributor
 */
public interface CalculateDistributor {
    /**
     * calculates different things
     * based on the class that implements this interface
     * @param distributor
     */
    void calculate(Distributor distributor);
}
