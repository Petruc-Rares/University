package formalities;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

/**
 * This class contains information about the contract
 */
public final class Contract {
    /**
     * The id of the consumer
     */
    private int consumerId;
    /**
     * The price established at the moment
     * of signing the contract
     */
    private int price;
    /**
     * Time remaining the contract is available
     */
    private int remainedContractMonths;
    /**
     * the id of the distributor that delivers
     * energy to the consumers
     */
    private int distributorId;

    public Contract(final int consumerId, final int price, final int remainingMonths,
                    final int distributorId) {
        this.consumerId = consumerId;
        this.price = price;
        this.remainedContractMonths = remainingMonths;
        this.distributorId = distributorId;
    }

    public Contract() {

    }

    public int getRemainedContractMonths() {
        return remainedContractMonths;
    }

    public void setRemainedContractMonths(final int remainingMonths) {
        this.remainedContractMonths = remainingMonths;
    }

    public int getConsumerId() {
        return consumerId;
    }

    public void setConsumerId(final int consumerId) {
        this.consumerId = consumerId;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(final int price) {
        this.price = price;
    }

    @JsonIgnore
    @JsonProperty(value = "distributorId")
    public int getDistributorId() {
        return distributorId;
    }

    public void setDistributorId(int distributorId) {
        this.distributorId = distributorId;
    }
}
