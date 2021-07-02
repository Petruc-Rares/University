package calculatorsconsumer;

import formalities.Contract;
import powergridentities.Consumer;
import powergridentities.Distributor;
import utils.Utils;

/**
 * This class has static methods that set the consumer pay status
 * according to the fields that the consumer has and calculates
 * other monthly changing fields
 */
public final class ConsumerFinancialStatus {

    private ConsumerFinancialStatus() {
    }

    /**
     * This should multiply the current debt of the consumer
     */
    private static final double MULTIPLICATIONDEBT = 1.2;

    /**
     * sets the payment status of a consumer
     * either the consumer is a good payer
     * and money will just be withdrawn from him
     * either the consumer can not pay for the current month
     * and his inDebt status will be turned on, as well as the
     * sum he didn't pay will be saved
     * either the consumer is already in debt and now he must
     * pay more for his debt and for the current month or can't
     * afford to pay that much for this month and becomes bankrupt
     * @param consumer
     */
    public static void setPaymentStatus(final Consumer consumer) {
        boolean isBankrupt = consumer.isBankrupt();

        // if already bankrupt ignore anything else
        if (isBankrupt) {
            return;
        }

        int budgetAfterInstallment;
        boolean inDebt = consumer.isInDebt();
        int crtBudget = consumer.getCrtBudget();
        Contract contract = consumer.getContract();
        int overdueSum = consumer.getOverdueSum();
        // if customer doesn't have liabilities
        if (!inDebt) {
            budgetAfterInstallment = crtBudget - contract.getPrice();
            // if he will remain with no money
            // in debt status will become true
            if (budgetAfterInstallment < 0) {
                consumer.setInDebt(true);
                consumer.setOverdueSum(contract.getPrice());
                consumer.setDebtDistributorId(contract.getDistributorId());
            } else {
                // else simply make the payment
                consumer.setCrtBudget(budgetAfterInstallment);
            }
        } else {
            Distributor distributor = Utils.getDistributor(consumer.getContract().
                    getDistributorId());
            int contractLength = distributor.getContractLength();
            int sumToPay = 0;
            // in case the consumer renews the contract with a new
            // distributor, he can delay the current contract
            boolean flagToDelay = false;

            // check if consumer just began other contract
            if (consumer.getContract().getRemainedContractMonths() == contractLength) {
                // if the consumer renews the contract with the same distributor
                if (!(consumer.getDebtDistributorId() == distributor.getId())) {
                    sumToPay += consumer.getOverdueSum();
                    flagToDelay = true;
                }
            }

            // customer is renewing the contract with the same distributor
            // or simply paying monthly expenses
            if (sumToPay == 0) {
                sumToPay = (int) (Math.round(Math.floor(MULTIPLICATIONDEBT * overdueSum))
                        + contract.getPrice());
            }

            budgetAfterInstallment = crtBudget - sumToPay;

            // if he can't pay the overdue bill plus the current month
            // the consumer will be declared bankrupt and the game is over for him
            if (budgetAfterInstallment < 0) {
                consumer.setBankrupt(true);
            } else {
                // cancel the overdue bills and set the status to
                // a good payer
                if (!(flagToDelay)) {
                    consumer.setInDebt(false);
                    consumer.setOverdueSum(0);
                    // set distributor's id's field to a non valid
                    // field representing that the consumer has no
                    // liabilities for any distributor
                    consumer.setDebtDistributorId(Integer.MIN_VALUE);
                } else {
                    consumer.setOverdueSum(consumer.getContract().getPrice());
                    consumer.setDebtDistributorId(distributor.getId());
                }
            }
        }
    }

    /**
     * sets the new budget of a consumer
     * increases the current budget by the monthly income
     * this function should be called one / month
     * @param consumer
     */
    public static void setNewBudget(final Consumer consumer) {
        consumer.setCrtBudget(consumer.getCrtBudget() + consumer.getMonthlyIncome());
    }
}
