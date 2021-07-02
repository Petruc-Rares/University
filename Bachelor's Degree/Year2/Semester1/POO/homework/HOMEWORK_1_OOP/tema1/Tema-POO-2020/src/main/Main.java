package main;

import checker.Checker;
import checker.Checkstyle;
import common.Constants;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import newfileio.UserData;
import fileio.InputLoader;
import fileio.Writer;
import org.json.simple.JSONArray;
import solution.Solver;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Objects;

/**
 * The entry point to this homework. It runs the checker that tests your implementation.
 */
public final class Main {
    /**
     * for coding style
     */
    private Main() {
    }

    /**
     * Call the main checker and the coding style checker
     *
     * @param args from command line
     * @throws IOException in case of exceptions to reading / writing
     */
    public static void main(final String[] args) throws IOException {
        File directory = new File(Constants.TESTS_PATH);
        Path path = Paths.get(Constants.RESULT_PATH);
        if (!Files.exists(path)) {
            Files.createDirectories(path);
        }

        File outputDirectory = new File(Constants.RESULT_PATH);

        Checker checker = new Checker();
        checker.deleteFiles(outputDirectory.listFiles());

        for (File file : Objects.requireNonNull(directory.listFiles())) {

            String filepath = Constants.OUT_PATH + file.getName();
            File out = new File(filepath);
            boolean isCreated = out.createNewFile();
            if (isCreated) {
                action(file.getAbsolutePath(), filepath);
            }
        }

        checker.iterateFiles(Constants.RESULT_PATH, Constants.REF_PATH, Constants.TESTS_PATH);
        Checkstyle test = new Checkstyle();
        test.testCheckstyle();
    }

    /**
     * @param filePath1 for input file
     * @param filePath2 for output file
     * @throws IOException in case of exceptions to reading / writing
     */
    public static void action(final String filePath1,
                              final String filePath2) throws IOException {
        InputLoader inputLoader = new InputLoader(filePath1);
        fileio.Input input = inputLoader.readData();

        Writer fileWriter = new Writer(filePath2);
        JSONArray arrayResult = new JSONArray();

        // input must be reformatted both movies (in which we add rating)
        // and users (in which we add a map from the films that were rated by
        // the user to the rating given) of form <String, Double>

        // add rating field to both serials and seasons
        List<MovieData> moviesData = MovieData.addRatingField(input.getMovies());
        List<SerialData> serials = SerialData.addRating(input.getSerials());

        // add user rating map
        List<UserData> userData = UserData.addRatingMap(input.getUsers());

        // create the new input
        Input newInput = new Input(input.getActors(), userData, input.getCommands(),
                moviesData, serials);

        // solve the current action
        Solver solver = new Solver(newInput, arrayResult, fileWriter);
        solver.solve();
        fileWriter.closeJSON(arrayResult);
    }
}
