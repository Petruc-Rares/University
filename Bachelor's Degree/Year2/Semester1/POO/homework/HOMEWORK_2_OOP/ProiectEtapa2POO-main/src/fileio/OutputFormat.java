package fileio;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.util.List;

/**
 *  This class respects the pattern of the ref files
 *  and has a method that writes it to the out files
 */
public final class OutputFormat {
    private List<ConsumerOutputData> consumers;
    private List<DistributorOutputData> distributors;
    private List<ProducerOutputData> energyProducers;

    public OutputFormat() {
    }

    public OutputFormat(final List<ConsumerOutputData> consumers,
                        final List<DistributorOutputData> distributors,
                        final List<ProducerOutputData> producers) {
        this.consumers = consumers;
        this.distributors = distributors;
        this.energyProducers = producers;
    }

    public List<ConsumerOutputData> getConsumers() {
        return consumers;
    }

    public void setConsumers(final List<ConsumerOutputData> consumers) {
        this.consumers = consumers;
    }

    public List<DistributorOutputData> getDistributors() {
        return distributors;
    }

    public void setDistributors(final List<DistributorOutputData> distributors) {
        this.distributors = distributors;
    }

    public List<ProducerOutputData> getEnergyProducers() {
        return energyProducers;
    }

    public void setEnergyProducers(List<ProducerOutputData> energyProducers) {
        this.energyProducers = energyProducers;
    }

    /**
        this method is used to write the current object
        in the
        @param outputFile that the solver specifies
     */
    public void writeOutput(final String outputFile) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.writerWithDefaultPrettyPrinter().writeValue(new File(outputFile), this);
    }
}
