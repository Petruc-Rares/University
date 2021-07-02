package fileio;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;

/**
 * This class reads and parses the data from the tests
 */
public final class InputLoader {
    /**
     * The path to the input file
     */
    private final String inputPath;

    public InputLoader(final String inputPath) {
        this.inputPath = inputPath;
    }

    public String getInputPath() {
        return inputPath;
    }

    /**
     * The method reads the database
     *
     * @return an Input object
     * @throws IOException if file not found
     */
    public Input readData() throws IOException {

        ObjectMapper objectMapper = new ObjectMapper();
        Input input = objectMapper.readValue(new File(inputPath), Input.class);


        return input;
    }

}
